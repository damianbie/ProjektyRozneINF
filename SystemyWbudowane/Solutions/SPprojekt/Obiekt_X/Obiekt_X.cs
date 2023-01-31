using System;
using System.Collections.Generic;
using System.Text;
using System.Windows.Forms;
using System.Drawing;
using System.Drawing.Drawing2D;
using System.Globalization;

namespace Obiekty
{
    public class Obiekt_X : PictureBox
    {
        double mult = 60.0;         // Mno¿nik czasu cyklu (przyspieszenie), 60.0*Tc => (1 sekunda symulacji = 1 minuta rzeczyw. obiektu)

        // Obiekt symulacji:
        const double Hmax = 0.5;    // wysokoœæ zbiornika [m]
        double H = 0.0;             // poziom cieczy w zbiorniku [m]
        double Tcie = 20.0;         // temperatura cieczy w zbiorniku [stC]
        double Tzew = 20.0;         // temperatura zewnêtrzna [stC]
        double Ttraf = 25.0;        // temp trafa

        double dV3max = 100;        // maksymalny stopieñ otwarcia Z3
        double q3max;               // maksymalny odp³yw cieczy ze zbiornika przez Z3 [m3/s]

        double mkat = 0;            // k¹t obrotu mieszad³a
        double mx = 50;             // "rzut" mieszad³a na p³aszczyznê ekranu
        bool x1, x2, x3, x4, x5, x6, x7, x8;    // czujniki binarne
        bool TRAFO, BUZZ, Z3, Z4, Z5, M, PWM;         // elementy wykonawcze
        bool TT1, TT2; 
        

        Image imgElektrodaDol;              
        Image imgElektrodaGora;           
        Image imgTrafo;
        Image imgBuz;

        private int x0 = 20, y0 = 20;   // Przesuniêcie obiektów wzglêdem (0,0)
        private bool K1 = false;        // Klawisz K1 (naciœniêty => true)
        private bool K2 = false;        // Klawisz K2 (naciœniêty => true)
        private bool K3 = false;        // Klawisz K3 (naciœniêty => true)
        private bool K4 = false;        // Klawisz K4 (naciœniêty => true)
        private bool K5 = false;        // Klawisz K3 (naciœniêty => true)
        private bool K6 = false;        // Klawisz K4 (naciœniêty => true)
        private int xK1, yK1;           // wspó³rzêdne klawisza K1 wzglêdem (0,0)
        private int xK2, yK2;           // wspó³rzêdne klawisza K2 wzglêdem (0,0)
        private int xK3, yK3;           // wspó³rzêdne klawisza K3 wzglêdem (0,0)
        private int xK4, yK4;           // wspó³rzêdne klawisza K4 wzglêdem (0,0)
        private int xK5, yK5;           // wspó³rzêdne klawisza K3 wzglêdem (0,0)
        private int xK6, yK6;           // wspó³rzêdne klawisza K4 wzglêdem (0,0)

        Pen grayPen3 = new Pen(Color.Gray, 3);                          // Pióro: szary, gruboœæ: 3
        Pen redPen2 = new Pen(Color.Red, 2);                            // Pióro: szary, gruboœæ: 2
        SolidBrush redBrush = new SolidBrush(Color.Red);                // Wype³nienie: czerwony
        SolidBrush darkRedBrush = new SolidBrush(Color.DarkRed);        // Wype³nienie: ciemnoczerwony
        SolidBrush blueBrush = new SolidBrush(Color.Blue);              // Wype³nienie: niebieski
        SolidBrush grayBrush = new SolidBrush(Color.Gray);              // Wype³nienie: szary
        SolidBrush blackBrush = new SolidBrush(Color.Black);            // Wype³nienie: czarny
        SolidBrush whiteBrush = new SolidBrush(Color.White);            // Wype³nienie: bia³y
        FontFamily fontCourier = new FontFamily("Courier New");         // Czcionka: Courier New
        Font fontObiekt;    // = new Font(fontCourier, 12, FontStyle.Regular, GraphicsUnit.Pixel);
        FontFamily fontArial = new FontFamily("Arial");
        //Font fontKlaw;      // = new Font(fontArial, 16, FontStyle.Regular, GraphicsUnit.Pixel);
        NumberFormatInfo nfi = new CultureInfo("en-US", false).NumberFormat;

        int counter = 0, currentPWMCounter = 0;
        int maxCounter = 10, lastPWM = 0, fanSpeed = 0;

        public Obiekt_X()
        {
            try
            {
                imgElektrodaDol     = System.Drawing.Image.FromFile(@"Img\ElektrodaDol.gif");
                imgElektrodaGora    = System.Drawing.Image.FromFile(@"Img\ElektrodaGora.gif");
                imgTrafo            = System.Drawing.Image.FromFile(@"Img\trafo.gif");
                imgBuz              = System.Drawing.Image.FromFile(@"Img\buz.gif");
            }
            catch (Exception ee)
            {   // Jeœli b³¹d otwarcia pliku
                MessageBox.Show("Brak pliku " + ee.Message, "B³¹d otwarcia pliku", MessageBoxButtons.OK, MessageBoxIcon.Exclamation);
            }
            xK1 = x0 + 20; yK1 = y0 + 10;
            xK2 = x0 + 80; yK2 = y0 + 10;
            xK3 = x0 + 140; yK3 = y0 + 10;
            xK4 = x0 + 200; yK4 = y0 + 10;
            xK5 = x0 + 0; yK5 = y0 + 233;
            xK6 = x0 + 0; yK6 = y0 + 293;
            fontObiekt = new Font(fontCourier, 12, FontStyle.Regular, GraphicsUnit.Pixel);
            q3max = 0.5e-7 * dV3max * Math.Sqrt(2.0 * 9.81 * Hmax);    // maksymalny odp³yw cieczy ze zbiornika przez Z3 [m3/s]
        }

        public void obliczObiekt(double Tc, ushort[] bout, ushort[] binp, ushort[] aout, ushort[] ainp)
        {
            Tc *= mult;                                 // Przyspieszenie symulacji obiektu

            // --- Ustawienie elementów wykonawczych (wyjœæ sterownika - wejœæ obiektu) ---
            TRAFO = (bout[0] & 0x0001) != 0;               // Otwieranie zaworu +Z1/-Z2
            BUZZ = (bout[0] & 0x0002) != 0;               // Zamykanie zaworu +Z1/-Z2
            Z3 = (bout[0] & 0x0004) != 0;               // Otwieranie/zamykanie zaworu Z3
            Z4 = (bout[0] & 0x0008) != 0;               // Otwieranie/zamykanie zaworu Z4
            Z5 = (bout[0] & 0x0010) != 0;               // Otwieranie/zamykanie zaworu Z5
            M  = (bout[0] & 0x0020) != 0;               // W³¹czanie/wy³¹czanie mieszd³a M
            PWM = (bout[0] & 0x0040) != 0;               // W³¹czanie/wy³¹czanie grzejnika G1
            // Podobnie mo¿na obs³u¿yæ pozosta³e wyjœcia sterownika: bout&0x2000 .. bout&0x80000



            if (PWM == true)
            {
               this.Ttraf -= 0.01;
            }
       
            if(TRAFO)
            {
                Ttraf += 0.08;
            }
            if (Ttraf > Tzew)
            {
                Ttraf -= 0.0004;
            }

            if( Ttraf > 28.0)
                TT1 = true;
            else 
                TT1 = false;
            if (Ttraf > 32)
                TT2 = true;
            else
                TT2 = false;

            x5 = TT1;
            x6 = TT2;


            // ----- do wizualizacji -----
            if (fanSpeed>0)                                      // Mieszad³o M
            {
                mkat += 8.0 * (fanSpeed/2000.0);                                    // Obrót o 5 stopni/cykl
                mkat %= 360;                                    // Max 360 stopni na obrót
                mx = 50 * Math.Cos((mkat / 180.0) * 3.14159);   // Rzut ³opatek mieszad³a na p³aszczyznê ekranu
            }

            // ----- Aktualizacja czujników binarnych (wyjœæ obiektu - wejœæ sterownika) -----

            x1 = K1;

            binp[0] = 0;
            if (x1) binp[0] |= 0x0001;
            if (x2) binp[0] |= 0x0002;
            if (x3) binp[0] |= 0x0004;
            if (x4) binp[0] |= 0x0008;
            if (x5) binp[0] |= 0x0010;
            if (x6) binp[0] |= 0x0020;
            if (x7) binp[0] |= 0x0040;
            if (x8) binp[0] |= 0x0080;
            // podobnie mo¿na obs³u¿yæ pozosta³e czujniki x9..x16

            // ----- Aktualizacja czujników analogowych (wyjœæ obiektu - wejœæ sterownika) -----
            ainp[0] = (ushort)(Tcie * 10.0);                    // Temperatura: [0.0, 100.0] => [0, 1000]
            ainp[1] = (ushort)(H * 2000.0);                     // Wysokoœæ s³upa cieczy: [0.0, 0.5] => [0, 1000]
            ainp[2] = aout[2];                                  // Dla testu (mo¿na zmieniæ w razie potrzeby)
            ainp[3] = aout[3];
            ainp[4] = aout[4];
            ainp[5] = aout[5];
            ainp[6] = aout[6];
            ainp[7] = aout[7];
        }

        public void czytajMysz(object sender, MouseEventArgs e)
        {
            if (e.X >= xK1 && e.X <= xK1 + 30 && e.Y >= yK1 && e.Y <= yK1 + 30)
            { K1 = K1 ? false : true; }
            if (e.X >= xK2 && e.X <= xK2 + 30 && e.Y >= yK2 && e.Y <= yK2 + 30)
            { K2 = K2 ? false : true; }
            if (e.X >= xK3 && e.X <= xK3 + 30 && e.Y >= yK3 && e.Y <= yK3 + 30)
            { K3 = K3 ? false : true; }
            if (e.X >= xK4 && e.X <= xK4 + 30 && e.Y >= yK4 && e.Y <= yK4 + 30)
            { K4 = K4 ? false : true; }
            if (e.X >= xK5 && e.X <= xK5 + 30 && e.Y >= yK5 && e.Y <= yK5 + 30)
            { K5 = K5 ? false : true; }
            if (e.X >= xK6 && e.X <= xK6 + 30 && e.Y >= yK6 && e.Y <= yK6 + 30)
            { K6 = K6 ? false : true; }
        }

        public void rysujKlawisz(object sender, PaintEventArgs e, bool Kx, int x, int y)
        {
            Pen keyPen = new Pen(Color.FromArgb(10, 255, 0, 0), 1);   // Pióro: jasno szary, gruboœæ:1
            Pen blackPen = new Pen(Color.Black, 1);                 // Pióro: czarny, gruboœæ:1
            Pen whitePen = new Pen(Color.White, 1);                 // Pióro: bia³y, gruboœæ:1
            Pen grayPen = new Pen(Color.Gray, 1);                   // Pióro: szary, gruboœæ:1

            e.Graphics.DrawRectangle(blackPen, x, y, 30, 30);       // Obrys klawisza

            e.Graphics.DrawLine(grayPen, x + 29, y + 1, x + 29, y + 29);      // Linia:
            e.Graphics.DrawLine(grayPen, x + 1, y + 29, x + 29, y + 29);      // Linia:
            if (Kx)
            {   // Klawisz wciœniêty
                e.Graphics.DrawLine(grayPen, x + 1, y + 1, x + 1, y + 29);    // Linia:
                e.Graphics.DrawLine(grayPen, x + 1, y + 1, x + 29, y + 1);    // Linia:
                e.Graphics.DrawLine(keyPen, x + 28, y + 2, x + 28, y + 28);   // Linia:
                e.Graphics.DrawLine(keyPen, x + 2, y + 28, x + 28, y + 28);   // Linia:
            }
            else
            {   // Klawisz puszczony
                e.Graphics.DrawLine(whitePen, x + 1, y + 1, x + 1, y + 29);   // Linia:
                e.Graphics.DrawLine(whitePen, x + 1, y + 1, x + 29, y + 1);   // Linia:
                e.Graphics.DrawLine(grayPen, x + 28, y + 2, x + 28, y + 28);  // Linia:
                e.Graphics.DrawLine(grayPen, x + 2, y + 28, x + 28, y + 28);  // Linia:
            }
        }

        void rysujWentylator(object sender, PaintEventArgs e, double mx, int x, int y) // Rysowanie mieszad³a
        {
            Pen grayPen = new Pen(Color.Gray, 3);                                   // Pióro: szary, gruboœæ: 3
            SolidBrush grayBrush = new SolidBrush(Color.Gray);                      // Wype³nienie: szary
            e.Graphics.FillEllipse(grayBrush, x, y - (int)mx + 50, 10, (int)mx);    //  ³opatka 2
            e.Graphics.FillEllipse(grayBrush, x, y + 50, 10, (int) mx);              //  ³opatka 1
            
        }

        public void rysujZgrzewarke(object sender, PaintEventArgs e,int obx0, int oby0)
        {
            e.Graphics.SmoothingMode = SmoothingMode.AntiAlias;                     // Wyg³adzanie
            NumberFormatInfo nfi = new CultureInfo("en-US", false).NumberFormat;
            nfi.NumberDecimalSeparator = ".";
            FontFamily fontCourier = new FontFamily("Courier New");                 // Czcionka: Courier New
            Font fontObiekt = new Font(fontCourier, 12, FontStyle.Regular, GraphicsUnit.Pixel);
            SolidBrush redBrush = new SolidBrush(Color.Red);                        // Wype³nienie: czerwony
            SolidBrush darkRedBrush = new SolidBrush(Color.DarkRed);                // Wype³nienie: ciemnoczerwony


            if (imgElektrodaGora != null) e.Graphics.DrawImage(imgElektrodaGora, obx0 + 0, oby0 + ((K1==true)?10:0));      // elektorda
            if (imgElektrodaDol != null) e.Graphics.DrawImage(imgElektrodaDol, obx0 + 0, oby0 + 60);      // elektroda
            e.Graphics.FillRectangle(darkRedBrush, obx0 + 84, oby0 - 5, 15, 120);

            if (imgTrafo!= null) e.Graphics.DrawImage(imgTrafo, obx0 + 100, oby0 + 10);      // trafo

            // krancowka przy elektrodach
            e.Graphics.DrawString("x1", fontObiekt, TT1 ? redBrush : blackBrush, obx0+15, oby0 + 35);
            e.Graphics.FillRectangle(x1 ? redBrush : darkRedBrush, obx0+15, oby0+50, 9, 9);

        }

        public void rysujObiekt(object sender, PaintEventArgs e)                    // Pojedynczy zbiornik z regulacj¹ przep³ywu
        {
            Pen grayPen = new Pen(Color.Gray, 3);                                   // Pióro: szary, gruboœæ: 3
            Pen BlackPen = new Pen(Color.Black, 1);                                   // Pióro: szary, gruboœæ: 3
            Pen redPen = new Pen(Color.Red, 2);                                     // Pióro: szary, gruboœæ: 2
            SolidBrush redBrush = new SolidBrush(Color.Red);                        // Wype³nienie: czerwony
            SolidBrush darkRedBrush = new SolidBrush(Color.DarkRed);                // Wype³nienie: ciemnoczerwony
            SolidBrush blueBrush = new SolidBrush(Color.Blue);                      // Wype³nienie: niebieski
            SolidBrush grayBrush = new SolidBrush(Color.Gray);                      // Wype³nienie: szary
            SolidBrush blackBrush = new SolidBrush(Color.Black);                    // Wype³nienie: czarny
            SolidBrush whiteBrush = new SolidBrush(Color.White);                    // Wype³nienie: bia³y
            FontFamily fontCourier = new FontFamily("Courier New");                 // Czcionka: Courier New
            Font fontObiekt = new Font(fontCourier, 12, FontStyle.Regular, GraphicsUnit.Pixel);
            FontFamily fontArial = new FontFamily("Arial");
            Font fontKlaw = new Font(fontArial, 16, FontStyle.Regular, GraphicsUnit.Pixel);

            e.Graphics.SmoothingMode = SmoothingMode.AntiAlias;                     // Wyg³adzanie
            NumberFormatInfo nfi = new CultureInfo("en-US", false).NumberFormat;
            nfi.NumberDecimalSeparator = ".";

            //e.Graphics.DrawRectangle(grayPen, 0, 0, 407, 377);                    // Obszar rysowania (tymczasowo!)
            //e.Graphics.FillRectangle(whiteBrush, 0, 0, 407, 377);                 // T³o



            counter++;
            if (PWM == true) currentPWMCounter++;

            if (this.counter >= maxCounter)
            {
                lastPWM = (int)((currentPWMCounter/(float)maxCounter) * 100);
                currentPWMCounter = 0;
                counter = 0;
            }
            fanSpeed = (2000 * lastPWM / 100);

            const int obx0 = 30, oby0 = 0;                                          // Przesuniêcie wzglêdem (0,0)

            // ---------- Elementy animacyjne ----------
            rysujZgrzewarke(sender, e, obx0, 150);
            rysujWentylator(sender, e, mx, obx0 + 200, 150);


            
            Pen cableActive = new Pen(Color.YellowGreen, 2); 
            Pen cableInactive = new Pen(Color.Blue, 2);   

            // kabelki pionowe, trafo
            e.Graphics.DrawLine((TRAFO == true) ? cableActive:cableInactive, 142, 160, 142, 130);
            e.Graphics.DrawLine((TRAFO == true) ? cableActive : cableInactive, 142, 250, 142, 280);
            // kabelki poziome
            e.Graphics.DrawLine((TRAFO == true) ? cableActive : cableInactive, 142, 130, 40, 130);
            e.Graphics.DrawLine((TRAFO == true) ? cableActive : cableInactive, 142, 280, 40, 280);
            // kabelki od elektrod
            e.Graphics.DrawLine((TRAFO == true) ? cableActive : cableInactive, 40, 130, 40, (K1 == true)?160:150);
            e.Graphics.DrawLine((TRAFO == true) ? cableActive : cableInactive, 40, 280, 40, 260);


            // czujniki temp 
            e.Graphics.DrawString("T1", fontObiekt, TT1 ? redBrush : blackBrush, 173, 135);
            e.Graphics.FillRectangle(TT1 ? redBrush : darkRedBrush, 175, 150, 9, 9);

            e.Graphics.DrawString("T2", fontObiekt, TT2 ? redBrush : blackBrush, 198, 135);
            e.Graphics.FillRectangle(TT2 ? redBrush : darkRedBrush, 200, 150, 9, 9);

            // buzer
            if (imgBuz != null) e.Graphics.DrawImage(imgBuz, 300, 140);      // elektorda
            if (BUZZ)
            {
                e.Graphics.DrawArc(grayPen, 302, 125, 45, 15, 180, 180);
                e.Graphics.DrawArc(grayPen, 310, 118, 30, 15, 200, 140);
            }


            // ---------- Elementy statyczne ----------
            //if (img0) e.Graphics.DrawImage(image0, 104, 167);                       // Zbiornik z zaworami
            e.Graphics.DrawString("Tzew=" + Tzew.ToString("0.0", nfi), fontObiekt, blackBrush, obx0 + 250, oby0 + 25);
            e.Graphics.DrawString("Ttra=" + Ttraf.ToString("0.0", nfi), fontObiekt, blackBrush, obx0 + 250, oby0 + 35);
            e.Graphics.DrawString("Fspeed=" + fanSpeed.ToString("0.0", nfi), fontObiekt, blackBrush, obx0 + 250, oby0 + 45);
            e.Graphics.DrawString("pwm=" + lastPWM.ToString("0.0", nfi) + "%", fontObiekt, blackBrush, obx0 + 250, oby0 + 55);
         

            // --- Przyciski (opisy) ---
            e.Graphics.DrawString("Z", fontKlaw, blackBrush, xK1 + 5, yK1 + 6);
            //e.Graphics.DrawString("-", fontKlaw, blackBrush, xK2 + 5, yK2 + 6);

            // --- Przyciski ---
            rysujKlawisz(sender, e, K1, xK1, yK1);        // Przycisk K1
            //rysujKlawisz(sender, e, K2, xK2, yK2);        // Przycisk K2
            //rysujKlawisz(sender, e, K3, xK3, yK3);        // Przycisk K3
            //rysujKlawisz(sender, e, K4, xK4, yK4);        // Przycisk K4
            //rysujKlawisz(sender, e, K5, xK5, yK5);                      // Przycisk K5
            //rysujKlawisz(sender, e, K6, xK6, yK6);                      // Przycisk K6
        }
    }
}
