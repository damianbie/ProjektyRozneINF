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
        double Tc = 0.1 * 60.0;     // Czas cyklu obliczeniowego (100ms * 60) => 60 x szybciej (1 sekunda symulacji = 1 minuta rzeczyw. obiektu)
        double mult = 60.0;         // Mno�nik czasu cyklu (przyspieszenie), 60.0*Tc => (1 sekunda symulacji = 1 minuta rzeczyw. obiektu)

        // Obiekt symulacji:
        const double Hmax = 0.5;    // wysoko�� zbiornika [m]
        double R = 0.1;             // promie� podstawy zbiornika [m]
        double S_zb = 3.14 * 0.01;  // powierzchnia zbiornika [m2]
        double H = 0.0;             // poziom cieczy w zbiorniku [m]
        double V = 0.0;             // obj�to�� cieczy w zbiorniku [m3]
        double P_G1nom = 300;       // nominalna moc grza�ki G3 [W]
        double P_G1;                // moc grza�ki G1 [W]
        double Pel1;                // moc wydzielana przez grza�k� [W]
        //double Pgrz = 0.0;          // moc cieplna doprowadzana do grza�ki [W]
        double Pcie = 0.0;          // moc cieplna doprowadzana do zbiornika [W]
        double Pzew = 0.0;          // moc cieplna odprowadzana do otoczenia [W]
        double T1 = 20.0;           // temperatura cieczy z +Z1/-Z2 [stC]
        double Tgrz = 20.0;         // temperatura cieczy w zbiorniku [stC]
        double Tcie = 20.0;         // temperatura cieczy w zbiorniku [stC]
        double Tzew = 20.0;         // temperatura zewn�trzna [stC]
        double Cgrz = 41.0;         // Pojemno�� cieplna grza�ki [J/K]
        double ro_c = 4.180e6;      // iloczyn g�sto�ci cieczy i ciep�a w�a�ciwego (ro=1000[kg/m3], c=4180 [J/(kg*K)])
        double kc1 = 25.0;          // wsp�czynnik przenikania ciep�a [W/K] - otwarty zbiornik
        double kc2 = 2.0;           // wsp�czynnik przenikania ciep�a [W/K] - 2 cm styropianu
        double Kgrpo = 0.03;        // Przewodno�� cieplna: grza�ka - powietrze [W/K]
        double Kgrci = 1.2;         // Przewodno�� cieplna: grza�ka - ciecz [W/K]
        double kH = 0.0;            // Wsp�czynnik zanurzenia grza�ki w cieczy
        double T_on = 60.0f;        // Temperatura za��czaj�ca czujnik

        double dV1 = 0;             // stopie� otwarcia zaworu Z1 [0..100]%
        double dV2 = 0;             // stopie� otwarcia zaworu Z2 [0..100]%
        double q12;                 // dop�yw cieczy z Z1+Z2 [m3/s]
        double dV3 = 0;             // stopie� otwarcia zaworu Z3 [m3/s]
        double S_Z3;                // powierzchnia czynna zaworu Z3 [m2]
        double dV3max = 100;        // maksymalny stopie� otwarcia Z3
        double q3;                  // odp�yw cieczy ze zbiornika przez Z3 [m3/s]
        double q3max;               // maksymalny odp�yw cieczy ze zbiornika przez Z3 [m3/s]
        double dV;                  // przyrost obj�to�ci cieczy w zbiorniku
        bool ovZ1;                  // znacznik przepe�nienia zbiornika

        int h;                      // wysoko�� s�upa cieczy (w pikselach)
        bool uszkG1 = false;        // uszkodzenie grza�ki G1
        double mkat = 0;            // k�t obrotu mieszad�a
        double mx = 20;             // "rzut" mieszad�a na p�aszczyzn� ekranu
        bool x1, x2, x3, x4, x5, x6, x7, x8;    // czujniki binarne
        bool Z1, Z2, Z3, Z4, Z5, M, G1;         // elementy wykonawcze
        //Image image0;               // zbiornik z zaworami (t�o)
        Image image1;               // czerwony zaw�r
        Image image2;               // s�o�ce
        Image image3;               // deszcz
        Image imgZbiornik2;         // pojedynczy zbiornik wi�kszy
        Image imgZawor;             // zaw�r (z lewej strony)
        Image imgZawor2v;           // zaw�r (z prawej strony)
        Image imgZawor2hB;          // zaw�r poziomy (blue)
        Image imgZawor2vB;          // zaw�r pionowy (blue)
        Image imgKolanko1;          // kolanko1 |^
        Image imgKolanko2;          // kolanko2 ^|
        Image imgKolanko3;          // kolanko3 |_
        Image imgKolanko4;          // kolanko4 _|
        Image imgRura1h;            // rura 1 pozioma
        Image imgRura1v;            // rura 1 pionowa
        Image imgMop;               // mop

        private int x0 = 20, y0 = 20;   // Przesuni�cie obiekt�w wzgl�dem (0,0)
        private bool K1 = false;        // Klawisz K1 (naci�ni�ty => true)
        private bool K2 = false;        // Klawisz K2 (naci�ni�ty => true)
        private bool K3 = false;        // Klawisz K3 (naci�ni�ty => true)
        private bool K4 = false;        // Klawisz K4 (naci�ni�ty => true)
        private bool K5 = false;        // Klawisz K3 (naci�ni�ty => true)
        private bool K6 = false;        // Klawisz K4 (naci�ni�ty => true)
        private int xK1, yK1;           // wsp�rz�dne klawisza K1 wzgl�dem (0,0)
        private int xK2, yK2;           // wsp�rz�dne klawisza K2 wzgl�dem (0,0)
        private int xK3, yK3;           // wsp�rz�dne klawisza K3 wzgl�dem (0,0)
        private int xK4, yK4;           // wsp�rz�dne klawisza K4 wzgl�dem (0,0)
        private int xK5, yK5;           // wsp�rz�dne klawisza K3 wzgl�dem (0,0)
        private int xK6, yK6;           // wsp�rz�dne klawisza K4 wzgl�dem (0,0)

        Pen grayPen3 = new Pen(Color.Gray, 3);                          // Pi�ro: szary, grubo��: 3
        Pen redPen2 = new Pen(Color.Red, 2);                            // Pi�ro: szary, grubo��: 2
        SolidBrush redBrush = new SolidBrush(Color.Red);                // Wype�nienie: czerwony
        SolidBrush darkRedBrush = new SolidBrush(Color.DarkRed);        // Wype�nienie: ciemnoczerwony
        SolidBrush blueBrush = new SolidBrush(Color.Blue);              // Wype�nienie: niebieski
        SolidBrush grayBrush = new SolidBrush(Color.Gray);              // Wype�nienie: szary
        SolidBrush blackBrush = new SolidBrush(Color.Black);            // Wype�nienie: czarny
        SolidBrush whiteBrush = new SolidBrush(Color.White);            // Wype�nienie: bia�y
        FontFamily fontCourier = new FontFamily("Courier New");         // Czcionka: Courier New
        Font fontObiekt;    // = new Font(fontCourier, 12, FontStyle.Regular, GraphicsUnit.Pixel);
        FontFamily fontArial = new FontFamily("Arial");
        //Font fontKlaw;      // = new Font(fontArial, 16, FontStyle.Regular, GraphicsUnit.Pixel);
        NumberFormatInfo nfi = new CultureInfo("en-US", false).NumberFormat;

        public Obiekt_X()
        {
            try
            {
                //image0 = System.Drawing.Image.FromFile(@"Img\KaskadaX.gif");
                image1 = System.Drawing.Image.FromFile(@"Img\Zaw�r2r.gif");
                image2 = System.Drawing.Image.FromFile(@"Img\sun.png");
                image3 = System.Drawing.Image.FromFile(@"Img\rain.png");

                imgZbiornik2 = System.Drawing.Image.FromFile(@"Img\Pipes\Zbiornik2.gif");
                imgZawor = System.Drawing.Image.FromFile(@"Img\Pipes\Zaw�r2h.gif");
                imgKolanko1 = System.Drawing.Image.FromFile(@"Img\Pipes\Kolanko1.gif");
                imgKolanko2 = System.Drawing.Image.FromFile(@"Img\Pipes\Kolanko2.gif");
                imgKolanko3 = System.Drawing.Image.FromFile(@"Img\Pipes\Kolanko3.gif");
                imgKolanko4 = System.Drawing.Image.FromFile(@"Img\Pipes\Kolanko4.gif");
                imgRura1h = System.Drawing.Image.FromFile(@"Img\Pipes\Rura1h.gif");
                imgRura1v = System.Drawing.Image.FromFile(@"Img\Pipes\Rura1v.gif");
                imgZawor2v = System.Drawing.Image.FromFile(@"Img\Pipes\Zaw�r2v.gif");
                imgZawor2hB = System.Drawing.Image.FromFile(@"Img\Pipes\Zaw�r2hB.gif");
                imgZawor2vB = System.Drawing.Image.FromFile(@"Img\Pipes\Zaw�r2vB.gif");
                imgMop = System.Drawing.Image.FromFile(@"Img\Pipes\Mop.gif");
            }
            catch (Exception ee)
            {   // Je�li b��d otwarcia pliku
                MessageBox.Show("Brak pliku " + ee.Message, "B��d otwarcia pliku", MessageBoxButtons.OK, MessageBoxIcon.Exclamation);
            }
            xK1 = x0 + 20; yK1 = y0 + 10;
            xK2 = x0 + 80; yK2 = y0 + 10;
            xK3 = x0 + 140; yK3 = y0 + 10;
            xK4 = x0 + 200; yK4 = y0 + 10;
            xK5 = x0 + 0; yK5 = y0 + 233;
            xK6 = x0 + 0; yK6 = y0 + 293;
            fontObiekt = new Font(fontCourier, 12, FontStyle.Regular, GraphicsUnit.Pixel);
            q3max = 0.5e-7 * dV3max * Math.Sqrt(2.0 * 9.81 * Hmax);    // maksymalny odp�yw cieczy ze zbiornika przez Z3 [m3/s]
        }

        public void obliczObiekt(double Tc, ushort[] bout, ushort[] binp, ushort[] aout, ushort[] ainp)
        {
            Tc *= mult;                                 // Przyspieszenie symulacji obiektu

            // --- Ustawienie element�w wykonawczych (wyj�� sterownika - wej�� obiektu) ---
            Z1 = (bout[0] & 0x0001) != 0;               // Otwieranie zaworu +Z1/-Z2
            Z2 = (bout[0] & 0x0002) != 0;               // Zamykanie zaworu +Z1/-Z2
            Z3 = (bout[0] & 0x0004) != 0;               // Otwieranie/zamykanie zaworu Z3
            Z4 = (bout[0] & 0x0008) != 0;               // Otwieranie/zamykanie zaworu Z4
            Z5 = (bout[0] & 0x0010) != 0;               // Otwieranie/zamykanie zaworu Z5
            M  = (bout[0] & 0x0020) != 0;               // W��czanie/wy��czanie mieszd�a M
            G1 = (bout[0] & 0x0040) != 0;               // W��czanie/wy��czanie grzejnika G1
            // Podobnie mo�na obs�u�y� pozosta�e wyj�cia sterownika: bout&0x2000 .. bout&0x80000

            if(aout[0]>500 && aout[0]<=1000)            // Przyk�ad odczytu aout[] (wyj�cia analogowego sterownika)
                dV3max = aout[0]/10.0f;                 // Regulacja max otwarcia zaworu Z3
            
            T_on = aout[1]/10.0f;                       // podobnie dla wyj�� 1..7 (np. do parametryzacji obiektu)

            // -------- Otwarcie zawor�w --------
            dV1 = Z1 ? 100.0 : 0.0;                     // Zaw�r Z1 [0 / 100]%
            dV2 = Z2 ? 100.0 : 0.0;                     // Zaw�r Z2 [0 / 100]%
            //if (Z1) dV1 += 2.0;                         // Otwieranie +Z1/-Z2 [0...100]%
            //if (Z2) dV1 -= 2.0;                         // Zamykanie +Z1/-Z2
            //if (dV1 > 100.0) dV1 = 100.0;               // Nie wi�cej ni� 100%
            //if (dV1 < 0.0) dV1 = 0.0;                   // Nie mniej ni� 0%
            
            if (Z3) dV3 = dV3max;                       // Opr�nianie zbiornika
            else dV3 = 0;

            // -------- Temperatura zewn�trzna --------
            if (K1) Tzew += 0.1;                        // Zwi�kszanie temperatury
            if (K2) Tzew -= 0.1;                        // Zmniejszanie temperatury
            if (Tzew > 40.0) Tzew = 40.0;               // Nie wi�cej ni� 40 stC
            if (Tzew < 0.0) Tzew = 0.0;                 // Nie mniej ni� 0 stC
            if (K5) P_G1nom += 10.0;                    // Zwi�kszanie mocy grza�ki G3 (symulacja pokr�t�a)
            if (K6) P_G1nom -= 10.0;                    // Zmniejszanie mocy grza�ki G3 (symulacja pokr�t�a)
            if (P_G1nom > 800.0) P_G1nom = 800.0;       // Nie wi�cej ni� 800W
            if (P_G1nom < 50.0) P_G1nom = 50.0;         // Nie mniej ni� 50W

            // ----- Bilans obj�to�ci -----
            q12 = 0.5e-7 * (dV1 + dV2);                 // Przep�yw cieczy przez Z1+Z2
            if (K3 && H > 0.0) q12 -= 0.5e-7 * 25.0;    // S�o�ce
            if (K4 && H <= 100.0) q12 += 0.5e-7 * 25.0; // Deszcz
            S_Z3 = 0.5e-7 * dV3;                        // Powierzchnia czynna zaworu Z3
            q3 = S_Z3 * Math.Sqrt(2.0 * 9.81 * H);      // Przep�yw cieczy przez Z3
            //qZ4 = sZ4max * (Z4 ? Math.Sqrt(2.0 * 9.81 * H2) : 0.0);  // Przep�yw cieczy przez Z4 (swobodny wyp�yw)
            H = H + Tc * (q12 - q3) / S_zb;             // Wysoko�� s�upa cieczy w zbiorniku [m]
            if (H > Hmax) { H = Hmax; ovZ1 = true; }    // Maksymalny poziom (wysoko��)
            if (H < 0.0) H = 0.0;                       // Minimalny poziom (wysoko��)
            dV = (H * S_zb - V);                        // Przyrost obj�to�ci cieczy w zbiorniku
            V = H * S_zb;                               // Obj�to�� cieczy w zbiorniku
            kH = H / Hmax > 0.1 ? 1.0 : 10 * H / Hmax;  // Wsp�czynnik zanurzenia grza�ki w cieczy

            // ----- Bilans ciep�a -----
            Tgrz += Tc / Cgrz * P_G1;                   // Temperatura grza�ki
            if (ro_c * V > 5 * Tc)
                Tcie += Tc / (ro_c * V) * Pcie;         // Temperatura cieczy w zbiorniku
            else
                if (H < 0.02 * Hmax)
                    Tcie = Tzew;                        // Brak cieczy, czujnik mierzy temperatur� powietrza
                else
                    Tcie += Tc / 5.0 * Pcie;            // Ma�o cieczy, tylko cz�ciowe zanurzenie grza�ki

            if (Tgrz > 800.0) uszkG1 = true;            // Uszkodzenie grza�ki G1
            if (G1 && !uszkG1) Pel1 = P_G1nom;          // W��czona grza�ka G1
            else Pel1 = 0.0;                            // Wy��czona lub uszkodzona grza�ka G1
            
            if (Tcie > 100.0) Tcie = 100.0;             // Ograniczenie temperatury cieczy do 100 stC (np. dla wody)

            P_G1 = Pel1 + (1.0 - kH) * Kgrpo * (Tzew - Tgrz) + kH * Kgrci * (Tcie - Tgrz);    // Moc cieplna obudowy grza�ki
            Pcie = kH * Kgrci * (Tgrz - Tcie) + ro_c * q12 * (T1 - Tcie) + (kc1 * S_zb + kc2 * 6.28 * R * H) * (Tzew - Tcie);  // Moc cieplna doprowadzona do zbiornika
            Pzew = (1.0 - kH) * Kgrpo * (Tgrz - Tzew) + (kc1 * S_zb + kc2 * 6.28 * R * H) * (Tcie - Tzew);  // Moc cieplna doprowadzona do zbiornika
            
            // ----- do wizualizacji -----
            h = (int)(80.0 * H / Hmax);                         // Wysoko�� s�upa cieczy (w pikselach)
            if (h >= 72.0) x3 = true; else x3 = false;          // Czujnik x1
            if (h >= 40.0) x2 = true; else x2 = false;          // Czujnik x2
            if (h >= 8.0) x1 = true; else x1 = false;           // Czujnik x3
            if (Tcie >= T_on) x8 = true; else x8 = false;       // Czujnik T
            x4 = x5 = x6 = x7 = false;                          // Nieu�ywane

            if (M == true)                                      // Mieszad�o M
            {
                mkat += 5.0;                                    // Obr�t o 5 stopni/cykl
                mkat %= 360;                                    // Max 360 stopni na obr�t
                mx = 20 * Math.Cos((mkat / 180.0) * 3.14159);   // Rzut �opatek mieszad�a na p�aszczyzn� ekranu
            }

            // ----- Aktualizacja czujnik�w binarnych (wyj�� obiektu - wej�� sterownika) -----
            binp[0] = 0;
            if (x1) binp[0] |= 0x0001;
            if (x2) binp[0] |= 0x0002;
            if (x3) binp[0] |= 0x0004;
            if (x4) binp[0] |= 0x0008;
            if (x5) binp[0] |= 0x0010;
            if (x6) binp[0] |= 0x0020;
            if (x7) binp[0] |= 0x0040;
            if (x8) binp[0] |= 0x0080;
            // podobnie mo�na obs�u�y� pozosta�e czujniki x9..x16

            // ----- Aktualizacja czujnik�w analogowych (wyj�� obiektu - wej�� sterownika) -----
            ainp[0] = (ushort)(Tcie * 10.0);                    // Temperatura: [0.0, 100.0] => [0, 1000]
            ainp[1] = (ushort)(H * 2000.0);                     // Wysoko�� s�upa cieczy: [0.0, 0.5] => [0, 1000]
            ainp[2] = aout[2];                                  // Dla testu (mo�na zmieni� w razie potrzeby)
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
            Pen keyPen = new Pen(Color.FromArgb(10, 0, 0, 0), 1);   // Pi�ro: jasno szary, grubo��:1
            Pen blackPen = new Pen(Color.Black, 1);                 // Pi�ro: czarny, grubo��:1
            Pen whitePen = new Pen(Color.White, 1);                 // Pi�ro: bia�y, grubo��:1
            Pen grayPen = new Pen(Color.Gray, 1);                   // Pi�ro: szary, grubo��:1

            e.Graphics.DrawRectangle(blackPen, x, y, 30, 30);       // Obrys klawisza

            e.Graphics.DrawLine(grayPen, x + 29, y + 1, x + 29, y + 29);      // Linia:
            e.Graphics.DrawLine(grayPen, x + 1, y + 29, x + 29, y + 29);      // Linia:
            if (Kx)
            {   // Klawisz wci�ni�ty
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

        void rysujMieszadlo(object sender, PaintEventArgs e, double mx, int x, int y) // Rysowanie mieszad�a
        {
            Pen grayPen = new Pen(Color.Gray, 3);                                   // Pi�ro: szary, grubo��: 3
            SolidBrush grayBrush = new SolidBrush(Color.Gray);                      // Wype�nienie: szary
            e.Graphics.DrawLine(grayPen, x, y, x, y + 80);                          // Mieszad�o:
            e.Graphics.FillEllipse(grayBrush, x, y + 70, (int)mx, 10);              //  �opatka 1
            e.Graphics.FillEllipse(grayBrush, x - (int)mx, y + 70, (int)mx, 10);    //  �opatka 2
        }

        void rysujGrzejnik(object sender, PaintEventArgs e, bool G, int x, int y)   // Rysowanie grzejnika
        {
            Pen grayPen = new Pen(Color.Gray, 3);                                   // Pi�ro: szary, grubo��: 3
            SolidBrush redBrush = new SolidBrush(Color.Red);                        // Wype�nienie: czerwony
            SolidBrush grayBrush = new SolidBrush(Color.Gray);                      // Wype�nienie: szary
            e.Graphics.DrawLine(grayPen, x, y + 20, x, y);                          // --
            e.Graphics.DrawLine(grayPen, x + 1, y + 1, x + 5, y + 1);               // |
            e.Graphics.DrawLine(grayPen, x + 25, y + 1, x + 28, y + 1);             //       --
            e.Graphics.DrawLine(grayPen, x + 29, y + 20, x + 29, y);                //        |
            e.Graphics.FillRectangle(G ? redBrush : grayBrush, x + 5, y - 4, 20, 10); // Grzejnik G1
        }

        void rysujGrzejnik(object sender, PaintEventArgs e, int x, int y, bool G, bool uszkG)   // Rysowanie grzejnika
        {
            e.Graphics.DrawLine(grayPen3, x, y + 20, x, y);                             // --
            e.Graphics.DrawLine(grayPen3, x + 1, y + 1, x + 4, y + 1);                  // |
            e.Graphics.DrawLine(grayPen3, x + 24, y + 1, x + 27, y + 1);                //       --
            e.Graphics.DrawLine(grayPen3, x + 28, y + 20, x + 28, y);                   //        |
            e.Graphics.FillRectangle(G ? redBrush : grayBrush, x + 4, y - 4, 20, 10);   // Grzejnik G1
            if (uszkG)                                                                  // Je�li uszkodzony grzejnik G1
            {
                e.Graphics.FillRectangle(blackBrush, x + 3, y - 4, 20, 10);             // Czarny prostok�t G1
                e.Graphics.DrawLine(redPen2, x + 3, y - 10, x + 23, y + 10);            //         \
                e.Graphics.DrawLine(redPen2, x + 3, y + 10, x + 23, y - 10);            //         /
            }
        }

        public void rysujZbiornik(object sender, PaintEventArgs e, int obx0, int oby0) // Zbiornik z mieszad�em i grza�k�
        {
            e.Graphics.SmoothingMode = SmoothingMode.AntiAlias;                     // Wyg�adzanie
            NumberFormatInfo nfi = new CultureInfo("en-US", false).NumberFormat;
            nfi.NumberDecimalSeparator = ".";

            // ---------- Elementy statyczne ----------
            // ----- Zbiornik -----
            if (imgRura1h != null) e.Graphics.DrawImage(imgRura1h, obx0 + 10, oby0 + 30);       // Rura
            if (imgKolanko2 != null) e.Graphics.DrawImage(imgKolanko2, obx0 + 40, oby0 + 30);   // Kolanko
            if (imgKolanko1 != null) e.Graphics.DrawImage(imgKolanko1, obx0 + 140, oby0 + 30);  // Kolanko
            if (imgRura1h != null) e.Graphics.DrawImage(imgRura1h, obx0 + 180, oby0 + 30);      // Rura
            if (imgZbiornik2 != null) e.Graphics.DrawImage(imgZbiornik2, obx0 + 40, oby0 + 62); // Zbiornik 2 (dolny)
            if (imgRura1v != null) e.Graphics.DrawImage(imgRura1v, obx0 + 94, oby0 + 142);      // Rura
            if (imgZawor2v != null) e.Graphics.DrawImage(imgZawor2v, obx0 + 92, oby0 + 152);    // Zaw�r dolny
            if (imgRura1v != null) e.Graphics.DrawImage(imgRura1v, obx0 + 94, oby0 + 172);      // Rura
            //if (imgRura1v != null) e.Graphics.DrawImage(imgRura1v, obx0 + 134, oby0 + 142);      // Rura
            //if (imgZawor2v != null) e.Graphics.DrawImage(imgZawor2v, obx0 + 132, oby0 + 152);    // Zaw�r dolny
            //if (imgRura1v != null) e.Graphics.DrawImage(imgRura1v, obx0 + 134, oby0 + 172);      // Rura

            // ----- Czujniki -----
            e.Graphics.DrawLine(grayPen3, obx0 + 30, oby0 + 68, obx0 + 39, oby0 + 68);          // x5
            e.Graphics.DrawLine(grayPen3, obx0 + 30, oby0 + 100, obx0 + 39, oby0 + 100);        // x6
            e.Graphics.DrawLine(grayPen3, obx0 + 30, oby0 + 132, obx0 + 39, oby0 + 132);        // x7
            e.Graphics.DrawLine(grayPen3, obx0 + 160, oby0 + 132, obx0 + 169, oby0 + 132);      // x8 (T)

            // ---------- Elementy animacyjne ----------
            // ----- Zaw�r Z1 (g�rny) -----
            e.Graphics.DrawString("Z1", fontObiekt, Z1 ? redBrush : blackBrush, obx0 + 23, oby0 + 5);
            if (Z1)
            {
                if (imgZawor2hB != null) e.Graphics.DrawImage(imgZawor2hB, obx0 + 20, oby0 + 22, 20, 20);   // Zaw�r Z1 (blue)
                e.Graphics.FillRectangle(blueBrush, obx0 + 51, oby0 + 50, 7, 88);                           // Strumie� z Z1
            }
            else
                if (imgZawor != null) e.Graphics.DrawImage(imgZawor, obx0 + 20, oby0 + 22);                 // Zaw�r Z1 (gray)

            // ----- Zaw�r Z2 (g�rny) -----
            e.Graphics.DrawString("Z2", fontObiekt, Z2 ? redBrush : blackBrush, obx0 + 163, oby0 + 5);
            if (Z2)
            {
                if (imgZawor2hB != null) e.Graphics.DrawImage(imgZawor2hB, obx0 + 160, oby0 + 22, 20, 20);  // Zaw�r Z2 (blue)
                e.Graphics.FillRectangle(blueBrush, obx0 + 141, oby0 + 50, 7, 88);                          // Strumie� z Z2
            }
            else
                if (imgZawor != null) e.Graphics.DrawImage(imgZawor, obx0 + 160, oby0 + 22);                // Zaw�r Z2 (gray)

            // ----- Wype�nienie zbiornika ciecz�  -----
            e.Graphics.FillRectangle(blueBrush, obx0 + 42, oby0 + 60 + 80 - (int)h, 115, (int)h);           // Zbiornik (ciecz)

            // ----- Zaw�r Z3 (dolny) -----
            e.Graphics.DrawString("Z3", fontObiekt, Z3 ? redBrush : blackBrush, obx0 + 115, oby0 + 153);
            if (Z3)
            {
                if (imgZawor2vB != null) e.Graphics.DrawImage(imgZawor2vB, obx0 + 92, oby0 + 152, 20, 20);  // Zaw�r Z3 (blue)
                //e.Graphics.FillRectangle(blueBrush, obx0 + 95, oby0 + 181, (int)((q3 / q3max)*8), 20);    // Strumie� z Z3
                e.Graphics.FillRectangle(blueBrush, obx0 + 95, oby0 + 181, 8, 20);                          // Strumie� z Z3
            }
            else
                if (imgZawor2v != null) e.Graphics.DrawImage(imgZawor2v, obx0 + 92, oby0 + 152, 20, 20);    // Zaw�r Z3 (szary)

            // ----- Czujnik x7 -----
            e.Graphics.DrawString("x3", fontObiekt, x7 ? redBrush : blackBrush, obx0 + 0, oby0 + 60);
            e.Graphics.FillRectangle(x3 ? redBrush : darkRedBrush, obx0 + 21, oby0 + 64, 9, 9);             // Czujnik x3

            // ----- Czujnik x6 -----
            e.Graphics.DrawString("x2", fontObiekt, x6 ? redBrush : blackBrush, obx0 + 0, oby0 + 92);
            e.Graphics.FillRectangle(x2 ? redBrush : darkRedBrush, obx0 + 21, oby0 + 96, 9, 9);             // Czujnik x2

            // ----- Czujnik x5 -----
            e.Graphics.DrawString("x1", fontObiekt, x5 ? redBrush : blackBrush, obx0 + 0, oby0 + 124);
            e.Graphics.FillRectangle(x1 ? redBrush : darkRedBrush, obx0 + 21, oby0 + 128, 9, 9);            // Czujnik x1

            // ----- Czujnik x8 -----
            e.Graphics.DrawString("T", fontObiekt, x8 ? redBrush : blackBrush, obx0 + 182, oby0 + 124);
            e.Graphics.DrawString(Tcie.ToString("0.0", nfi) + "\u00B0C", fontObiekt, x8 ? redBrush : blackBrush, obx0 + 162, oby0 + 140);
            e.Graphics.FillRectangle(x8 ? redBrush : darkRedBrush, obx0 + 169, oby0 + 128, 9, 9);           // Czujnik x8

            // ----- Czujnik H -----
            e.Graphics.DrawString("H=" + H.ToString("0.00", nfi) + "m", fontObiekt, x7 ? redBrush : blackBrush, obx0 + 164, oby0 + 48);
            e.Graphics.DrawLine(grayPen3, obx0 + 154, oby0 + 55, obx0 + 154, oby0 + 138);                   // Czujnik poziomu H
            e.Graphics.DrawLine(grayPen3, obx0 + 154, oby0 + 55, obx0 + 164, oby0 + 55);                    //

            // ----- Mieszad�o -----
            rysujMieszadlo(sender, e, mx, obx0 + 99, oby0 + 55);                                            // Mieszad�o
            e.Graphics.DrawString("M", fontObiekt, M ? redBrush : blackBrush, obx0 + 95, oby0 + 40);

            // ----- Grzejnik G1 -----
            rysujGrzejnik(sender, e, obx0 + 46, oby0 + 130, G1, uszkG1);                                    // Grzejnik
            e.Graphics.DrawString("G1", fontObiekt, G1 ? redBrush : blackBrush, obx0 + 52, oby0 + 142);     // G1

            // ----- Grzejnik G2 -----
            //rysujGrzejnik(sender, e, obx0 + 122, oby0 + 130, G2, uszkG2);                                   // Grzejnik
            //e.Graphics.DrawString("G2", fontObiekt, G2 ? redBrush : blackBrush, obx0 + 128, oby0 + 142);    // G2

            // ----- Przepe�nienie zbiornika -----
            if (ovZ1 && imgMop != null)
                e.Graphics.DrawImage(imgMop, obx0 + 85, oby0 + 86, 30, 30);                                 // Mop
        }

        public void rysujObiekt(object sender, PaintEventArgs e)                    // Pojedynczy zbiornik z regulacj� przep�ywu
        {
            Pen grayPen = new Pen(Color.Gray, 3);                                   // Pi�ro: szary, grubo��: 3
            Pen redPen = new Pen(Color.Red, 2);                                     // Pi�ro: szary, grubo��: 2
            SolidBrush redBrush = new SolidBrush(Color.Red);                        // Wype�nienie: czerwony
            SolidBrush darkRedBrush = new SolidBrush(Color.DarkRed);                // Wype�nienie: ciemnoczerwony
            SolidBrush blueBrush = new SolidBrush(Color.Blue);                      // Wype�nienie: niebieski
            SolidBrush grayBrush = new SolidBrush(Color.Gray);                      // Wype�nienie: szary
            SolidBrush blackBrush = new SolidBrush(Color.Black);                    // Wype�nienie: czarny
            SolidBrush whiteBrush = new SolidBrush(Color.White);                    // Wype�nienie: bia�y
            FontFamily fontCourier = new FontFamily("Courier New");                 // Czcionka: Courier New
            Font fontObiekt = new Font(fontCourier, 12, FontStyle.Regular, GraphicsUnit.Pixel);
            FontFamily fontArial = new FontFamily("Arial");
            Font fontKlaw = new Font(fontArial, 16, FontStyle.Regular, GraphicsUnit.Pixel);

            e.Graphics.SmoothingMode = SmoothingMode.AntiAlias;                     // Wyg�adzanie
            NumberFormatInfo nfi = new CultureInfo("en-US", false).NumberFormat;
            nfi.NumberDecimalSeparator = ".";

            //e.Graphics.DrawRectangle(grayPen, 0, 0, 407, 377);                    // Obszar rysowania (tymczasowo!)
            //e.Graphics.FillRectangle(whiteBrush, 0, 0, 407, 377);                 // T�o

            const int obx0 = 30, oby0 = 0;                                          // Przesuni�cie wzgl�dem (0,0)

            // ---------- Elementy animacyjne ----------
            rysujZbiornik(sender, e, obx0 + 74, 150);

            // ---------- Elementy statyczne ----------
            //if (img0) e.Graphics.DrawImage(image0, 104, 167);                       // Zbiornik z zaworami
            e.Graphics.DrawString("Tgrz=" + Tgrz.ToString("0.0", nfi) + "\u00B0C", fontObiekt, x8 ? redBrush : blackBrush, obx0 + 250, oby0 + 25);
            e.Graphics.DrawString("Tcie=" + Tcie.ToString("0.0", nfi) + "\u00B0C", fontObiekt, x8 ? redBrush : blackBrush, obx0 + 250, oby0 + 45);
            e.Graphics.DrawString("Tzew=" + Tzew.ToString("0.0", nfi) + "\u00B0C", fontObiekt, x8 ? redBrush : blackBrush, obx0 + 250, oby0 + 65);
            e.Graphics.DrawString("Pgrz=" + P_G1.ToString("0.0", nfi) + "W", fontObiekt, x8 ? redBrush : blackBrush, obx0 + 250, oby0 + 85);
            e.Graphics.DrawString("Pcie=" + Pcie.ToString("0.0", nfi) + "W", fontObiekt, x8 ? redBrush : blackBrush, obx0 + 250, oby0 + 105);
            e.Graphics.DrawString("Pzew=" + Pzew.ToString("0.0", nfi) + "W", fontObiekt, x8 ? redBrush : blackBrush, obx0 + 250, oby0 + 125);
            e.Graphics.DrawString("PG1=" + P_G1nom.ToString("0.0", nfi) + "W", fontObiekt, blackBrush, xK5, yK5 + 38);

            // --- Przyciski (opisy) ---
            e.Graphics.DrawString("+", fontKlaw, blackBrush, xK1 + 5, yK1 + 6);
            e.Graphics.DrawString("-", fontKlaw, blackBrush, xK2 + 5, yK2 + 6);
            e.Graphics.DrawString("S", fontKlaw, blackBrush, xK3 + 5, yK3 + 6);
            e.Graphics.DrawString("D", fontKlaw, blackBrush, xK4 + 5, yK4 + 6);
            e.Graphics.DrawString("+", fontKlaw, blackBrush, xK5 + 5, yK5 + 6);
            e.Graphics.DrawString("-", fontKlaw, blackBrush, xK6 + 5, yK6 + 6);
            // --- Przyciski ---
            rysujKlawisz(sender, e, K1, xK1, yK1);        // Przycisk K1
            rysujKlawisz(sender, e, K2, xK2, yK2);        // Przycisk K2
            rysujKlawisz(sender, e, K3, xK3, yK3);        // Przycisk K3
            rysujKlawisz(sender, e, K4, xK4, yK4);        // Przycisk K4
            if (K3 == true && image2 != null) e.Graphics.DrawImage(image2, xK3 - 10, yK3 + 60, 50, 50);        // S�o�ce
            if (K4 == true && image3 != null) e.Graphics.DrawImage(image3, xK4 - 10, yK4 + 60, 50, 50);        // Deszcz
            rysujKlawisz(sender, e, K5, xK5, yK5);                      // Przycisk K5
            rysujKlawisz(sender, e, K6, xK6, yK6);                      // Przycisk K6
        }
    }
}
