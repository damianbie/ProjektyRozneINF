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
        double Tc = 0.1 * 60.0; // Czas cyklu obliczeniowego (100ms * 60) => 60 x szybciej (1 sekunda symulacji = 1 minuta rzeczyw. obiektu)
        double mult = 60.0;     // Mno¿nik czasu cyklu (przyspieszenie), 60.0*Tc => (1 sekunda symulacji = 1 minuta rzeczyw. obiektu)

        // ========== Obiekt symulacji ==========
        // Zbiornik 1 (lewy górny)
        const double H1max = 0.3;               // wysokoœæ zbiornika [m]
        const double R1 = 0.08;                 // promieñ podstawy zbiornika [m]
        const double S_zb1 = 3.14 * R1 * R1;    // powierzchnia zbiornika [m2]
        double H1 = 0.0;                        // poziom cieczy w zbiorniku [m]
        double V1 = 0.0;                        // objêtoœæ cieczy w zbiorniku [m3]

        // Zbiornik 2 (prawy górny)
        const double H2max = 0.3;               // wysokoœæ zbiornika [m]
        const double R2 = 0.08;                 // promieñ podstawy zbiornika [m]
        const double S_zb2 = 3.14 * R2 * R2;    // powierzchnia zbiornika [m2]
        double H2 = 0.0;                        // poziom cieczy w zbiorniku [m]
        double V2 = 0.0;                        // objêtoœæ cieczy w zbiorniku [m3]

        // Zbiornik 3 (dolny)
        const double H3max = 0.5;               // wysokoœæ zbiornika [m]
        const double R3 = 0.1;                  // promieñ podstawy zbiornika [m]
        const double S_zb3 = 3.14 * R3 * R3;    // powierzchnia zbiornika [m2]
        double H3 = 0.0;                        // poziom cieczy w zbiorniku [m]
        double V3 = 0.0;                        // objêtoœæ cieczy w zbiorniku [m3]

        double P_G1nom = 150;                   // nominalna moc grza³ki G1 [W]
        double P_G2nom = 150;                   // nominalna moc grza³ki G2 [W]
        double P_G3nom = 250;                   // nominalna moc grza³ki G3 [W]
        double P_G4nom = 250;                   // nominalna moc grza³ki G4 [W]
        double Pel1, Pel2, Pel3, Pel4;          // moc elektryczna wydzielana przez grza³ki G1..G4 [W]
        double P_G1, P_G2, P_G3, P_G4;          // moc cieplna doprowadzana do grza³ek G1..G4 [W]
        double Pzb1, Pzb2, Pzb3;                // moc cieplna doprowadzana do zbiorników 1..3 [W]
        double Pzew = 0.0;                      // moc cieplna odprowadzana do otoczenia [W]
        bool uszkG1, uszkG2, uszkG3, uszkG4;    // uszkodzenie grza³ki G1..G4
        double Twe1 = 14.0;                     // temperatura cieczy doprowadzanej przez Z1 [*C]
        double Twe2 = 14.0;                     // temperatura cieczy doprowadzanej przez Z3 [*C]
        double Tzb1 = 20.0;                     // temperatura cieczy w zbiorniku 1 [*C]
        double Tzb2 = 20.0;                     // temperatura cieczy w zbiorniku 2 [*C]
        double Tzb3 = 20.0;                     // temperatura cieczy w zbiorniku 3 [*C]
        double T_G1 = 20.0;                     // temperatura grza³ki w zbiorniku 1 [*C]
        double T_G2 = 20.0;                     // temperatura grza³ki w zbiorniku 2 [*C]
        double T_G3 = 20.0;                     // temperatura grza³ki w zbiorniku 3 [*C]
        double T_G4 = 20.0;                     // temperatura grza³ki w zbiorniku 4 [*C]
        double kH1, kH2, kH3, kH4;              // Wspó³czynniki zanurzenia grza³ek G1..G4 w cieczy

        double Tzew = 20.0;         // temperatura zewnêtrzna [*C]
        double Cgrz = 41.0;         // Pojemnoœæ cieplna grza³ki [J/K]
        double ro_c = 4.180e6;      // iloczyn gêstoœci cieczy i ciep³a w³aœciwego (ro=1000[kg/m3], c=4180 [J/(kg*K)])
        double kc1 = 25.0;          // wspó³czynnik przenikania ciep³a [W/K] - otwarty zbiornik
        double kc2 = 2.0;           // wspó³czynnik przenikania ciep³a [W/K] - 2 cm styropianu
        double Kgrpo = 0.03;        // Przewodnoœæ cieplna: grza³ka - powietrze [W/K]
        double Kgrci = 1.2;         // Przewodnoœæ cieplna: grza³ka - ciecz [W/K]

        const double sZ1max = 3.14 * 0.0005 * 0.0005;     // maksymalna powierzchnia czynna zaworu Z1 [m2] (pi*r^2)
        const double sZ2max = 3.14 * 0.0005 * 0.0005;     // maksymalna powierzchnia czynna zaworu Z2 [m2] (pi*r^2)
        const double sZ3max = 3.14 * 0.0005 * 0.0005;     // maksymalna powierzchnia czynna zaworu Z3 [m2] (pi*r^2)
        const double sZ4max = 3.14 * 0.0005 * 0.0005;     // maksymalna powierzchnia czynna zaworu Z4 [m2] (pi*r^2)
        const double sZ5max = 3.14 * 0.001 * 0.001;     // maksymalna powierzchnia czynna zaworu Z5 [m2] (pi*r^2)
        const double vZ1max = 4.0;                      // maksymalna prêdkoœæ przep³ywu cieczy przez Z1 [m/s]
        const double vZ3max = 4.0;                      // maksymalna prêdkoœæ przep³ywu cieczy przez Z3 [m/s]
        double qZ1max = sZ1max * vZ1max;                // maksymalny przep³yw cieczy przez zawór Z1 [m3/s]
        double qZ2max = sZ2max * Math.Sqrt(2.0 * 9.81 * H1max); // maksymalny przep³yw cieczy przez zawór Z3 [m3/s]
        double qZ3max = sZ3max * vZ3max;                // maksymalny przep³yw cieczy przez zawór Z3 [m3/s]
        double qZ4max = sZ4max * Math.Sqrt(2.0 * 9.81 * H2max); // maksymalny przep³yw cieczy przez zawór Z3 [m3/s]
        double qZ5max = sZ5max * Math.Sqrt(2.0 * 9.81 * H3max); // maksymalny przep³yw cieczy przez zawór Z3 [m3/s]
        double qZ1, qZ2, qZ3, qZ4, qZ5;                 // przep³yw cieczy przez zawory Z1..Z5 [m3/s]
        bool ovZ1, ovZ2, ovZ3;                          // znacznik przepe³nienia zbiornika

        double dV1 = 0;             // stopieñ otwarcia zaworu Z1 [0..100]%
        double dV2 = 0;             // stopieñ otwarcia zaworu Z2 [0..100]%
        double dV3 = 0;             // stopieñ otwarcia zaworu Z3 [m3/s]
        double dV3max = 100;        // maksymalny stopieñ otwarcia Z3

        int h1;                      // wysokoœæ s³upa cieczy (w pikselach)
        int h2;                      // wysokoœæ s³upa cieczy (w pikselach)
        int h3;                      // wysokoœæ s³upa cieczy (w pikselach)
        double mkat = 0;            // k¹t obrotu mieszad³a
        double mx = 20;             // "rzut" mieszad³a na p³aszczyznê ekranu
        bool x1, x2, x3, x4, x5, x6, x7, x8, x9, x10;   // czujniki binarne (sensory)
        bool Z1, Z2, Z3, Z4, Z5, M, G1, G2, G3, G4;     // elementy wykonawcze (aktuator/efektor)
        Image image0;               // zbiornik z zaworami (t³o)
        Image image1;               // czerwony zawór
        Image image2;               // s³oñce
        Image image3;               // deszcz
        Image imgZbiornik1;         // pojedynczy zbiornik mniejszy
        Image imgZbiornik2;         // pojedynczy zbiornik wiêkszy
        Image imgZawor;             // zawór (z lewej strony)
        Image imgZawor2v;           // zawór (z prawej strony)
        Image imgZawor2hB;          // zawór poziomy (blue)
        Image imgZawor2vB;          // zawór pionowy (blue)
        Image imgKolanko1;          // kolanko1 |^
        Image imgKolanko2;          // kolanko2 ^|
        Image imgKolanko3;          // kolanko3 |_
        Image imgKolanko4;          // kolanko4 _|
        Image imgRura1h;            // rura 1 pozioma
        Image imgRura1v;            // rura 1 pionowa
        Image imgMop;               // mop

        private int x0 = 20, y0 = 10;   // Przesuniêcie obiektów wzglêdem (0,0)
        private bool K1 = false;        // Klawisz K1 (naciœniêty => true)
        private bool K2 = false;        // Klawisz K2 (naciœniêty => true)
        private bool K3 = false;        // Klawisz K3 (naciœniêty => true)
        private bool K4 = false;        // Klawisz K4 (naciœniêty => true)
        private int xK1, yK1;           // wspó³rzêdne klawisza K1 wzglêdem (0,0)
        private int xK2, yK2;           // wspó³rzêdne klawisza K2 wzglêdem (0,0)
        private int xK3, yK3;           // wspó³rzêdne klawisza K3 wzglêdem (0,0)
        private int xK4, yK4;           // wspó³rzêdne klawisza K4 wzglêdem (0,0)
        ushort bout0;

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
        Font fontKlaw;      // = new Font(fontArial, 16, FontStyle.Regular, GraphicsUnit.Pixel);
        NumberFormatInfo nfi = new CultureInfo("en-US", false).NumberFormat;

        public Obiekt_X()
        {
            try
            {
                image0 = System.Drawing.Image.FromFile(@"Img\KaskadaX.gif");// img0 = true;
                image1 = System.Drawing.Image.FromFile(@"Img\Zawór2r.gif");// img1 = true;
                image2 = System.Drawing.Image.FromFile(@"Img\sun.png");// img2 = true;
                image3 = System.Drawing.Image.FromFile(@"Img\rain.png");// img3 = true;
                imgZbiornik1 = System.Drawing.Image.FromFile(@"Img\Pipes\Zbiornik1.gif");
                imgZbiornik2 = System.Drawing.Image.FromFile(@"Img\Pipes\Zbiornik2.gif");
                imgZawor = System.Drawing.Image.FromFile(@"Img\Pipes\Zawór2h.gif");
                imgKolanko1 = System.Drawing.Image.FromFile(@"Img\Pipes\Kolanko1.gif");
                imgKolanko2 = System.Drawing.Image.FromFile(@"Img\Pipes\Kolanko2.gif");
                imgKolanko3 = System.Drawing.Image.FromFile(@"Img\Pipes\Kolanko3.gif");
                imgKolanko4 = System.Drawing.Image.FromFile(@"Img\Pipes\Kolanko4.gif");
                imgRura1h = System.Drawing.Image.FromFile(@"Img\Pipes\Rura1h.gif");
                imgRura1v = System.Drawing.Image.FromFile(@"Img\Pipes\Rura1v.gif");
                imgZawor2v = System.Drawing.Image.FromFile(@"Img\Pipes\Zawór2v.gif");
                imgZawor2hB = System.Drawing.Image.FromFile(@"Img\Pipes\Zawór2hB.gif");
                imgZawor2vB = System.Drawing.Image.FromFile(@"Img\Pipes\Zawór2vB.gif");
                imgMop = System.Drawing.Image.FromFile(@"Img\Pipes\Mop.gif");
            }
            catch (Exception ee)
            {   // Jeœli b³¹d otwarcia pliku
                MessageBox.Show("Brak pliku " + ee.Message, "B³¹d otwarcia pliku", MessageBoxButtons.OK, MessageBoxIcon.Exclamation);
            }

            fontObiekt = new Font(fontCourier, 12, FontStyle.Regular, GraphicsUnit.Pixel);
            fontKlaw = new Font(fontArial, 16, FontStyle.Regular, GraphicsUnit.Pixel);
            nfi.NumberDecimalSeparator = ".";

            //xK1 = x0 + 20; yK1 = y0 + 0;
            xK1 = x0 + 100; yK1 = y0 + 0;
            //xK2 = x0 + 80; yK2 = y0 + 0;
            xK2 = x0 + 240; yK2 = y0 + 0;
            //xK3 = x0 + 140; yK3 = y0 + 0;
            //xK4 = x0 + 200; yK4 = y0 + 0;
            xK3 = x0 + 0; yK3 = y0 + 220;
            xK4 = x0 + 0; yK4 = y0 + 280;
        }

        public void obliczObiekt(double Tc, ushort[] bout, ushort[] binp, ushort[] aout, ushort[] ainp)
        {
            Tc *= mult;                                 // Przyspieszenie symulacji obiektu

            // --- Ustawienie elementów wykonawczych (wyjœæ sterownika - wejœæ obiektu) ---
            Z1 = (bout[0] & 0x0001) != 0;               // Otwieranie zaworu +Z1/-Z2
            Z2 = (bout[0] & 0x0002) != 0;               // Zamykanie zaworu +Z1/-Z2
            Z3 = (bout[0] & 0x0004) != 0;               // Otwieranie/zamykanie zaworu Z3
            Z4 = (bout[0] & 0x0008) != 0;               // Otwieranie/zamykanie zaworu Z4
            Z5 = (bout[0] & 0x0010) != 0;               // Otwieranie/zamykanie zaworu Z5
            M  = (bout[0] & 0x0020) != 0;               // W³¹czanie/wy³¹czanie mieszd³a M
            G1 = (bout[0] & 0x0040) != 0;               // W³¹czanie/wy³¹czanie grzejnika G1
            G2 = (bout[0] & 0x0080) != 0;               // W³¹czanie/wy³¹czanie grzejnika G2
            G3 = (bout[0] & 0x0100) != 0;               // W³¹czanie/wy³¹czanie grzejnika G3
            G4 = (bout[0] & 0x0200) != 0;               // W³¹czanie/wy³¹czanie grzejnika G4
            // Podobnie mo¿na obs³u¿yæ pozosta³e wyjœcia sterownika: bout&0x2000 .. bout&0x80000
            bout0 = bout[0];

            if(aout[0]>50 && aout[0]<=100)              // Przyk³ad odczytu aout[] (wyjœcia analogowego sterownika)
                dV3max = aout[0];                       // Regulacja max otwarcia zaworu Z3
            
            // ... = aout[1];                           // podobnie dla wyjœæ 1..7 (np. do parametryzacji obiektu)

            // -------- Temperatura zewnêtrzna --------
            if (K1) Tzew += 0.1;                        // Zwiêkszanie temperatury
            if (K2) Tzew -= 0.1;                        // Zmniejszanie temperatury
            if (Tzew > 40.0) Tzew = 40.0;               // Nie wiêcej ni¿ 40 stC
            if (Tzew < 0.0) Tzew = 0.0;                 // Nie mniej ni¿ 0 stC
            if (K3) P_G3nom += 10.0;                    // Zwiêkszanie mocy grza³ki G3 (symulacja pokrêt³a)
            if (K4) P_G3nom -= 10.0;                    // Zmniejszanie mocy grza³ki G3 (symulacja pokrêt³a)
            if (P_G3nom > 1000.0) P_G3nom = 1000.0;     // Nie wiêcej ni¿ 500W
            if (P_G3nom < 50.0) P_G3nom = 50.0;         // Nie mniej ni¿ 50W

            // ----- Bilans objêtoœci -----
            // Zbiornik 1 (lewy górny)
            qZ1 = qZ1max * (Z1 ? 1.0 : 0.0);            // Przep³yw cieczy przez Z1
            qZ2 = sZ2max * (Z2 ? Math.Sqrt(2.0 * 9.81 * H1) : 0.0);  // Przep³yw cieczy przez Z2 (swobodny wyp³yw)
            H1 = H1 + Tc * (qZ1 - qZ2) / S_zb1;         // Wysokoœæ s³upa cieczy w zbiorniku [m]
            if (H1 > H1max) H1 = H1max;                 // Maksymalny poziom (wysokoœæ)
            if (H1 < 0.0) H1 = 0.0;                     // Minimalny poziom (wysokoœæ)
            dV1 = (H1 * S_zb1 - V1);                    // Przyrost objêtoœci cieczy w zbiorniku
            V1 = H1 * S_zb1;                            // Objêtoœæ cieczy w zbiorniku
            if (H1 >= H1max) ovZ1 = true;               // Przepe³nienie zbiornika
            kH1 = H1 / H1max > 0.1 ? 1.0 : 10 * H1 / H1max;  // Wspó³czynnik zanurzenia grza³ki w cieczy

            // Zbiornik 2 (prawy górny)
            qZ3 = qZ3max * (Z3 ? 1.0 : 0.0);            // Przep³yw cieczy przez Z3
            qZ4 = sZ4max * (Z4 ? Math.Sqrt(2.0 * 9.81 * H2) : 0.0);  // Przep³yw cieczy przez Z4 (swobodny wyp³yw)
            H2 = H2 + Tc * (qZ3 - qZ4) / S_zb2;         // Wysokoœæ s³upa cieczy w zbiorniku [m]
            if (H2 > H2max) H2 = H2max;                 // Maksymalny poziom (wysokoœæ)
            if (H2 < 0.0) H2 = 0.0;                     // Minimalny poziom (wysokoœæ)
            dV2 = (H2 * S_zb2 - V2);                    // Przyrost objêtoœci cieczy w zbiorniku
            V2 = H2 * S_zb2;                            // Objêtoœæ cieczy w zbiorniku
            if (H2 >= H2max) ovZ2 = true;               // Przepe³nienie zbiornika
            kH2 = H2 / H2max > 0.1 ? 1.0 : 10 * H2 / H2max;  // Wspó³czynnik zanurzenia grza³ki w cieczy

            // Zbiornik 3 (dolny)
            qZ5 = sZ5max * (Z5 ? Math.Sqrt(2.0 * 9.81 * H3) : 0.0);  // Przep³yw cieczy przez Z5 (swobodny wyp³yw)
            H3 = H3 + Tc * (qZ2 + qZ4 - qZ5) / S_zb3;   // Wysokoœæ s³upa cieczy w zbiorniku [m]
            if (H3 > H3max) H3 = H3max;                 // Maksymalny poziom (wysokoœæ)
            if (H3 < 0.0) H3 = 0.0;                     // Minimalny poziom (wysokoœæ)
            dV3 = (H3 * S_zb3 - V3);                    // Przyrost objêtoœci cieczy w zbiorniku
            V3 = H3 * S_zb3;                            // Objêtoœæ cieczy w zbiorniku
            kH3 = H3 / H3max > 0.1 ? 1.0 : 10 * H3 / H3max;  // Wspó³czynnik zanurzenia grza³ki w cieczy
            kH4 = H3 / H3max > 0.1 ? 1.0 : 10 * H3 / H3max;  // Wspó³czynnik zanurzenia grza³ki w cieczy
            if (H3 >= H3max) ovZ3 = true;               // Przepe³nienie zbiornika

            // ========== Bilans ciep³a ==========
            // ----- Temperatury grza³ek -----
            T_G1 += Tc / Cgrz * P_G1;                   // Temperatura grza³ki G1
            T_G2 += Tc / Cgrz * P_G2;                   // Temperatura grza³ki G2
            T_G3 += Tc / Cgrz * P_G3;                   // Temperatura grza³ki G3
            T_G4 += Tc / Cgrz * P_G4;                   // Temperatura grza³ki G4

            // ----- Temperatury cieczy w zbiornikach -----
            if (ro_c * V1 > 5 * Tc)
                Tzb1 += Tc / (ro_c * V1) * Pzb1;        // Temperatura cieczy w zbiorniku 3
            else
                if (H1 < 0.02 * H1max) Tzb1 = Tzew;     // Brak cieczy, czujnik mierzy temperaturê powietrza
                else Tzb1 += Tc / 5.0 * Pzb1;           // Ma³o cieczy, tylko czêœciowe zanurzenie grza³ki

            if (ro_c * V2 > 5 * Tc)
                Tzb2 += Tc / (ro_c * V2) * Pzb2;        // Temperatura cieczy w zbiorniku 3
            else
                if (H2 < 0.02 * H2max) Tzb2 = Tzew;     // Brak cieczy, czujnik mierzy temperaturê powietrza
                else Tzb2 += Tc / 5.0 * Pzb2;           // Ma³o cieczy, tylko czêœciowe zanurzenie grza³ki

            if (ro_c * V3 > 5 * Tc)
                Tzb3 += Tc / (ro_c * V3) * Pzb3;        // Temperatura cieczy w zbiorniku 3
            else
                if (H3 < 0.02 * H3max) Tzb3 = Tzew;     // Brak cieczy, czujnik mierzy temperaturê powietrza
                else Tzb3 += Tc / 5.0 * Pzb3;           // Ma³o cieczy, tylko czêœciowe zanurzenie grza³ki

            if (Tzb1 > 99.0) Tzb1 = 99.0;               // Ograniczenie temperatury cieczy do 100 stC (np. dla wody)
            if (Tzb2 > 99.0) Tzb2 = 99.0;               // Ograniczenie temperatury cieczy do 100 stC (np. dla wody)
            if (Tzb3 > 99.0) Tzb3 = 99.0;               // Ograniczenie temperatury cieczy do 100 stC (np. dla wody)

            // ----- Moc cieplna grza³ek -----
            if (T_G1 > 800.0) uszkG1 = true;            // Uszkodzenie grza³ki G1
            if (G1 && !uszkG1) Pel1 = P_G1nom;          // W³¹czona grza³ka G1
            else Pel1 = 0.0;                            // Wy³¹czona lub uszkodzona grza³ka G1

            if (T_G2 > 800.0) uszkG2 = true;            // Uszkodzenie grza³ki G2
            if (G2 && !uszkG2) Pel2 = P_G2nom;          // W³¹czona grza³ka G2
            else Pel2 = 0.0;                            // Wy³¹czona lub uszkodzona grza³ka G2

            if (T_G3 > 800.0) uszkG3 = true;            // Uszkodzenie grza³ki G1
            if (G3 && !uszkG3) Pel3 = P_G3nom;          // W³¹czona grza³ka G1
            else Pel3 = 0.0;                            // Wy³¹czona lub uszkodzona grza³ka G1

            if (T_G4 > 800.0) uszkG4 = true;            // Uszkodzenie grza³ki G4
            if (G4 && !uszkG4) Pel4 = P_G4nom;          // W³¹czona grza³ka G4
            else Pel4 = 0.0;                            // Wy³¹czona lub uszkodzona grza³ka G4

            // ----- Bilans mocy dla cieczy z zbiornikach -----
            P_G1 = Pel1 + (1.0 - kH1) * Kgrpo * (Tzew - T_G1) + kH1 * Kgrci * (Tzb1 - T_G1);    // Moc cieplna obudowy grza³ki G1
            P_G2 = Pel2 + (1.0 - kH2) * Kgrpo * (Tzew - T_G2) + kH2 * Kgrci * (Tzb2 - T_G2);    // Moc cieplna obudowy grza³ki G2
            P_G3 = Pel3 + (1.0 - kH3) * Kgrpo * (Tzew - T_G3) + kH3 * Kgrci * (Tzb3 - T_G3);    // Moc cieplna obudowy grza³ki G3
            P_G4 = Pel4 + (1.0 - kH4) * Kgrpo * (Tzew - T_G4) + kH4 * Kgrci * (Tzb3 - T_G4);    // Moc cieplna obudowy grza³ki G4

            Pzb1 = kH1 * Kgrci * (T_G1 - Tzb1) + ro_c * qZ1 * (Twe1 - Tzb1) + 
                (kc1 * S_zb1 + kc2 * (S_zb1 + 6.28 * R1 * H1)) * (Tzew - Tzb1);     // Moc cieplna doprowadzona do zbiornika 1
            Pzb2 = kH2 * Kgrci * (T_G2 - Tzb2) + ro_c * qZ3 * (Twe2 - Tzb2) + 
                (kc1 * S_zb2 + kc2 * (S_zb1 + 6.28 * R2 * H2)) * (Tzew - Tzb2);     // Moc cieplna doprowadzona do zbiornika 2
            Pzb3 = kH3 * Kgrci * (T_G3 - Tzb3) + kH4 * Kgrci * (T_G4 - Tzb3) + 
                ro_c * qZ2 * (Tzb1 - Tzb3) + ro_c * qZ4 * (Tzb2 - Tzb3) + 
                (kc1 * S_zb3 + kc2 * (S_zb3 + 6.28 * R3 * H3)) * (Tzew - Tzb3);     // Moc cieplna doprowadzona do zbiornika 3
            Pzew = (1.0 - kH1) * Kgrpo * (T_G1 - Tzew) + (1.0 - kH2) * Kgrpo * (T_G2 - Tzew) +
                (1.0 - kH3) * Kgrpo * (T_G3 - Tzew) + (1.0 - kH4) * Kgrpo * (T_G4 - Tzew) + 
                (kc1 * S_zb1 + kc2 * (S_zb1 + 6.28 * R1 * H1)) * (Tzb1 - Tzew) + 
                (kc1 * S_zb2 + kc2 * (S_zb2 + 6.28 * R2 * H2)) * (Tzb2 - Tzew) + 
                (kc1 * S_zb3 + kc2 * (S_zb3 + 6.28 * R3 * H3)) * (Tzb3 - Tzew);     // Moc cieplna doprowadzona do otoczenia
            
            // ----- do wizualizacji -----
            h1 = (int)(58.0 * H1 / H1max);                          // Wysokoœæ s³upa cieczy (w pikselach)
            if (h1 >= 12.0) x1 = true; else x1 = false;             // Czujnik x1
            if (h1 >= 48.0) x2 = true; else x2 = false;             // Czujnik x2
            if (Tzb1 >= 60.0) x8 = true; else x8 = false;           // Czujnik T

            h2 = (int)(58.0 * H2 / H2max);                          // Wysokoœæ s³upa cieczy (w pikselach)
            if (h2 >= 12.0) x3 = true; else x3 = false;             // Czujnik x3
            if (h2 >= 48.0) x4 = true; else x4 = false;             // Czujnik x4
            if (Tzb2 >= 60.0) x9 = true; else x9 = false;           // Czujnik T

            h3 = (int)(78.0 * H3 / H3max);                          // Wysokoœæ s³upa cieczy (w pikselach)
            if (h3 >= 8.0) x5 = true; else x5 = false;              // Czujnik x5
            if (h3 >= 40.0) x6 = true; else x6 = false;             // Czujnik x6
            if (h3 >= 72.0) x7 = true; else x7 = false;             // Czujnik x7
            if (Tzb3 >= 60.0) x10 = true; else x10 = false;         // Czujnik T

            if (M == true)                                          // Mieszad³o M
            {
                mkat += 5.0;                                        // Obrót o 5 stopni/cykl
                mkat %= 360;                                        // Max 360 stopni na obrót
                mx = 20 * Math.Cos((mkat / 180.0) * 3.14159);       // Rzut ³opatek mieszad³a na p³aszczyznê ekranu
            }

            // ----- Aktualizacja czujników binarnych (wyjœæ obiektu - wejœæ sterownika) -----
            binp[0] = 0;
            if (x1) binp[0] |= 0x0001;
            if (x2) binp[0] |= 0x0002;
            if (x3) binp[0] |= 0x0004;
            if (x4) binp[0] |= 0x0008;
            if (x5) binp[0] |= 0x0010;
            if (x6) binp[0] |= 0x0020;
            if (x7) binp[0] |= 0x0040;
            if (x8) binp[0] |= 0x0080;
            if (x9) binp[0] |= 0x0100;
            if (x10) binp[0] |= 0x0200;
            // podobnie mo¿na obs³u¿yæ pozosta³e czujniki x9..x16

            // ----- Aktualizacja czujników analogowych (wyjœæ obiektu - wejœæ sterownika) -----
            ainp[0] = (ushort)(Tzb1 * 10.0);                    // Temperatura: [0.0, 100.0] => [0, 1000]
            ainp[1] = (ushort)(Tzb2 * 10.0);                    // Temperatura: [0.0, 100.0] => [0, 1000]
            ainp[2] = (ushort)(Tzb3 * 10.0);                    // Temperatura: [0.0, 100.0] => [0, 1000]
            ainp[3] = (ushort)(H1 * 2000.0);                    // Wysokoœæ s³upa cieczy: [0.0, 0.3] => [0, 600]
            ainp[4] = (ushort)(H2 * 2000.0);                    // Wysokoœæ s³upa cieczy: [0.0, 0.3] => [0, 600]
            ainp[5] = (ushort)(H3 * 2000.0);                    // Wysokoœæ s³upa cieczy: [0.0, 0.5] => [0, 1000]
            ainp[6] = aout[6];                                  // Dla testu (mo¿na zmieniæ w razie potrzeby)aout[6];
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
        }

        public void rysujKlawisz(object sender, PaintEventArgs e, bool Kx, int x, int y)
        {
            Pen keyPen = new Pen(Color.FromArgb(10, 0, 0, 0), 1);   // Pióro: jasno szary, gruboœæ:1
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

        void rysujMieszadlo(object sender, PaintEventArgs e, double mx, int x, int y)   // Rysowanie mieszad³a
        {
            e.Graphics.DrawLine(grayPen3, x, y, x, y + 80);                             // Mieszad³o:
            e.Graphics.FillEllipse(grayBrush, x, y + 70, (int)mx, 10);                  // - ³opatka 1
            e.Graphics.FillEllipse(grayBrush, x - (int)mx, y + 70, (int)mx, 10);        // - ³opatka 2
        }

        void rysujGrzejnik(object sender, PaintEventArgs e, int x, int y, bool G, bool uszkG)   // Rysowanie grzejnika
        {
            e.Graphics.DrawLine(grayPen3, x, y + 20, x, y);                             // --
            e.Graphics.DrawLine(grayPen3, x + 1, y + 1, x + 4, y + 1);                  // |
            e.Graphics.DrawLine(grayPen3, x + 24, y + 1, x + 27, y + 1);                //       --
            e.Graphics.DrawLine(grayPen3, x + 28, y + 20, x + 28, y);                   //        |
            e.Graphics.FillRectangle(G ? redBrush : grayBrush, x + 4, y - 4, 20, 10);   // Grzejnik G1
            if (uszkG)                                                                  // Jeœli uszkodzony grzejnik G1
            {
                e.Graphics.FillRectangle(blackBrush, x + 3, y - 4, 20, 10);             // Czarny prostok¹t G1
                e.Graphics.DrawLine(redPen2, x + 3, y - 10, x + 23, y + 10);            //         \
                e.Graphics.DrawLine(redPen2, x + 3, y + 10, x + 23, y - 10);            //         /
            }
        }

        public void rysujZbiornikGorny1(object sender, PaintEventArgs e, int obx0, int oby0)    // Zbiornik z grza³k¹
        {
            e.Graphics.SmoothingMode = SmoothingMode.AntiAlias;                                 // Wyg³adzanie

            // ---------- Elementy statyczne ----------
            // ----- Zbiornik -----
            if (imgZbiornik1 != null) e.Graphics.DrawImage(imgZbiornik1, obx0 + 40, oby0 + 60); // Zbiornik 1 (lewy-górny)
            if (imgRura1h != null) e.Graphics.DrawImage(imgRura1h, obx0 + 10, oby0 + 30);       // Rura
            if (imgKolanko2 != null) e.Graphics.DrawImage(imgKolanko2, obx0 + 40, oby0 + 30);   // Kolanko
            if (imgRura1v != null) e.Graphics.DrawImage(imgRura1v, obx0 + 110, oby0 + 119);     // Rura
            if (imgRura1v != null) e.Graphics.DrawImage(imgRura1v, obx0 + 110, oby0 + 148);     // Rura

            // ----- Czujniki -----
            e.Graphics.DrawLine(grayPen3, obx0 + 31, oby0 + 72, obx0 + 39, oby0 + 72);          // x2
            e.Graphics.DrawLine(grayPen3, obx0 + 31, oby0 + 108, obx0 + 39, oby0 + 108);        // x1
            e.Graphics.DrawLine(grayPen3, obx0 + 130, oby0 + 108, obx0 + 138, oby0 + 108);      // xT

            // ---------- Elementy animacyjne ----------
            // ----- Zawór Z1 (górny) -----
            e.Graphics.DrawString("Z1", fontObiekt, Z1 ? redBrush : blackBrush, obx0 + 23, oby0 + 5);
            if (Z1)
            {
                if (imgZawor2hB != null) e.Graphics.DrawImage(imgZawor2hB, obx0 + 20, oby0 + 22, 20, 20);   // Zawór Z1 (blue)
                e.Graphics.FillRectangle(blueBrush, obx0 + 51, oby0 + 50, 7, 68);                           // Strumieñ z Z1
            }
            else
                if (imgZawor != null) e.Graphics.DrawImage(imgZawor, obx0 + 20, oby0 + 22);                 // Zawór Z1 (gray)

            // ----- Wype³nienie zbiornika ciecz¹  -----
            e.Graphics.FillRectangle(blueBrush, obx0 + 42, oby0 + 118 - (int)h1, 85, (int)h1);              // Zbiornik (ciecz)
            //e.Graphics.DrawString("P1=" + Pzb1.ToString("0.0", nfi) + "W", fontObiekt, x8 ? redBrush : blackBrush, obx0 + 65, oby0 + 8);
            //e.Graphics.DrawString("T1=" + Tzb1.ToString("0.0", nfi) + "\u00B0C", fontObiekt, x8 ? redBrush : blackBrush, obx0 + 65, oby0 + 23);
            //e.Graphics.DrawString("H1=" + H1.ToString("0.00", nfi) + "m", fontObiekt, x2 ? redBrush : blackBrush, obx0 + 65, oby0 + 38);

            // ----- Zawór Z2 (dolny) -----
            e.Graphics.DrawString("Z2", fontObiekt, Z2 ? redBrush : blackBrush, obx0 + 130, oby0 + 128);
            if (Z2)
            {
                if (imgZawor2vB != null) e.Graphics.DrawImage(imgZawor2vB, obx0 + 108, oby0 + 128, 20, 20); // Zawór Z3 (blue)
                e.Graphics.FillRectangle(blueBrush, obx0 + 111, oby0 + 158, (int)(qZ2 / qZ2max * 8), 20);   // Strumieñ z Z2
            }
            else
                if (imgZawor2v != null) e.Graphics.DrawImage(imgZawor2v, obx0 + 108, oby0 + 128);           // Zawór Z3 (gray)

            // ----- Czujnik x2 -----
            e.Graphics.DrawString("x2", fontObiekt, x2 ? redBrush : blackBrush, obx0 + 0, oby0 + 64);
            e.Graphics.FillRectangle(x2 ? redBrush : darkRedBrush, obx0 + 21, oby0 + 68, 9, 9);             // Czujnik x2

            // ----- Czujnik x1 -----
            e.Graphics.DrawString("x1", fontObiekt, x1 ? redBrush : blackBrush, obx0 + 0, oby0 + 100);
            e.Graphics.FillRectangle(x1 ? redBrush : darkRedBrush, obx0 + 21, oby0 + 104, 9, 9);            // Czujnik x1

            // ----- Czujnik xT -----
            e.Graphics.DrawString("T1", fontObiekt, x8 ? redBrush : blackBrush, obx0 + 133, oby0 + 90);
            e.Graphics.FillRectangle(x8 ? redBrush : darkRedBrush, obx0 + 139, oby0 + 104, 9, 9);           // Czujnik xT
            e.Graphics.DrawString(Tzb1.ToString("0", nfi) + "\u00B0", fontObiekt, x8 ? redBrush : blackBrush, obx0 + 132, oby0 + 113);

            // ----- Grzejnik G1 -----
            rysujGrzejnik(sender, e, obx0 + 46, oby0 + 108, G1, uszkG1);                                    // Grzejnik
            e.Graphics.DrawString("G1", fontObiekt, G1 ? redBrush : blackBrush, obx0 + 52, oby0 + 121);     // G1

            // ----- Przepe³nienie zbiornika -----
            if (ovZ1 && imgMop != null)
                e.Graphics.DrawImage(imgMop, obx0 + 70, oby0 + 66, 30, 30);                                 // Mop
        }

        public void rysujZbiornikGorny2(object sender, PaintEventArgs e, int obx0, int oby0)    // Zbiornik z grza³k¹
        {
            e.Graphics.SmoothingMode = SmoothingMode.AntiAlias;                                 // Wyg³adzanie

            // ---------- Elementy statyczne ----------
            // ----- Zbiornik -----
            if (imgKolanko1 != null) e.Graphics.DrawImage(imgKolanko1, obx0 + 110, oby0 + 30);  // Kolanko
            if (imgRura1h != null) e.Graphics.DrawImage(imgRura1h, obx0 + 150, oby0 + 30);      // Rura
            if (imgZbiornik1 != null) e.Graphics.DrawImage(imgZbiornik1, obx0 + 40, oby0 + 60); // Zbiornik 2 (prawy-górny)
            if (imgRura1v != null) e.Graphics.DrawImage(imgRura1v, obx0 + 50, oby0 + 119);      // Rura
            if (imgRura1v != null) e.Graphics.DrawImage(imgRura1v, obx0 + 50, oby0 + 148);      // Rura
            // ----- Czujniki -----
            e.Graphics.DrawLine(grayPen3, obx0 + 130, oby0 + 72, obx0 + 138, oby0 + 72);        // x2
            e.Graphics.DrawLine(grayPen3, obx0 + 130, oby0 + 108, obx0 + 138, oby0 + 108);      // x1
            e.Graphics.DrawLine(grayPen3, obx0 + 31, oby0 + 108, obx0 + 39, oby0 + 108);        // x8 (xT)

            // ---------- Elementy animacyjne ----------
            // ----- Zawór Z3 (górny) -----
            e.Graphics.DrawString("Z3", fontObiekt, Z3 ? redBrush : blackBrush, obx0 + 133, oby0 + 5);
            if (Z3)
            {
                if (imgZawor2hB != null) e.Graphics.DrawImage(imgZawor2hB, obx0 + 130, oby0 + 22, 20, 20);  // Zawór Z3 (blue)
                e.Graphics.FillRectangle(blueBrush, obx0 + 111, oby0 + 50, 7, 68);                          // Strumieñ z Z3
            }
            else
                if (imgZawor != null) e.Graphics.DrawImage(imgZawor, obx0 + 130, oby0 + 22);                // Zawór Z3 (gray)

            // ----- Wype³nienie zbiornika ciecz¹  -----
            e.Graphics.FillRectangle(blueBrush, obx0 + 42, oby0 + 118 - (int)h2, 85, (int)h2);              // Zbiornik (ciecz)
            //e.Graphics.DrawString("P2=" + Pzb2.ToString("0.0", nfi) + "W", fontObiekt, x9 ? redBrush : blackBrush, obx0 + 40, oby0 + 8);
            //e.Graphics.DrawString("T2=" + Tzb2.ToString("0.0", nfi) + "\u00B0C", fontObiekt, x9 ? redBrush : blackBrush, obx0 + 40, oby0 + 23);
            //e.Graphics.DrawString("H2=" + H2.ToString("0.00", nfi) + "m", fontObiekt, x4 ? redBrush : blackBrush, obx0 + 40, oby0 + 38);
            
            // ----- Zawór Z4 (dolny) -----
            e.Graphics.DrawString("Z4", fontObiekt, Z4 ? redBrush : blackBrush, obx0 + 70, oby0 + 128);
            if (Z4)
            {
                if (imgZawor2vB != null) e.Graphics.DrawImage(imgZawor2vB, obx0 + 48, oby0 + 128, 20, 20);  // Zawór Z4 (blue)
                e.Graphics.FillRectangle(blueBrush, obx0 + 51, oby0 + 158, (int)(qZ4 / qZ4max * 8), 20);    // Strumieñ z Z4
            }
            else
                if (imgZawor2v != null) e.Graphics.DrawImage(imgZawor2v, obx0 + 48, oby0 + 128, 20, 20);    // Zawór Z4 (gray)
            
            // ----- Czujnik x4 -----
            e.Graphics.DrawString("x4", fontObiekt, x4 ? redBrush : blackBrush, obx0 + 150, oby0 + 64);
            e.Graphics.FillRectangle(x4 ? redBrush : darkRedBrush, obx0 + 139, oby0 + 68, 9, 9);            // Czujnik x4

            // ----- Czujnik x3 -----
            e.Graphics.DrawString("x3", fontObiekt, x3 ? redBrush : blackBrush, obx0 + 150, oby0 + 100);
            e.Graphics.FillRectangle(x3 ? redBrush : darkRedBrush, obx0 + 139, oby0 + 104, 9, 9);           // Czujnik x3

            // ----- Czujnik xT -----
            e.Graphics.DrawString("T2", fontObiekt, x9 ? redBrush : blackBrush, obx0 + 19, oby0 + 90);
            e.Graphics.FillRectangle(x8 ? redBrush : darkRedBrush, obx0 + 21, oby0 + 104, 9, 9);            // Czujnik xT
            e.Graphics.DrawString(Tzb2.ToString("0", nfi) + "\u00B0", fontObiekt, x9 ? redBrush : blackBrush, obx0 + 12, oby0 + 113);

            // ----- Grzejnik G2 -----
            rysujGrzejnik(sender, e, obx0 + 95, oby0 + 108, G2, uszkG2);                                    // Grzejnik
            e.Graphics.DrawString("G2", fontObiekt, G2 ? redBrush : blackBrush, obx0 + 101, oby0 + 121);    // G2

            // ----- Przepe³nienie zbiornika -----
            if (ovZ2 && imgMop != null)
                e.Graphics.DrawImage(imgMop, obx0 + 70, oby0 + 66, 30, 30);                                 // Mop
        }

        public void rysujZbiornikDolny(object sender, PaintEventArgs e, int obx0, int oby0) // Zbiornik z mieszad³em i grza³k¹
        {
            //FontFamily fontCourier = new FontFamily("Courier New");                 // Czcionka: Courier New
            //Font fontObiekt = new Font(fontCourier, 12, FontStyle.Regular, GraphicsUnit.Pixel);
            //FontFamily fontArial = new FontFamily("Arial");
            //Font fontKlaw = new Font(fontArial, 16, FontStyle.Regular, GraphicsUnit.Pixel);

            e.Graphics.SmoothingMode = SmoothingMode.AntiAlias;                     // Wyg³adzanie
            NumberFormatInfo nfi = new CultureInfo("en-US", false).NumberFormat;
            nfi.NumberDecimalSeparator = ".";

            // ---------- Elementy statyczne ----------
            // ----- Zbiornik -----
            if (imgZbiornik2 != null) e.Graphics.DrawImage(imgZbiornik2, obx0 + 40, oby0 + 62); // Zbiornik 2 (dolny)
            if (imgRura1v != null) e.Graphics.DrawImage(imgRura1v, obx0 + 94, oby0 + 142);      // Rura
            if (imgZawor2v != null) e.Graphics.DrawImage(imgZawor2v, obx0 + 92, oby0 + 152);    // Zawór dolny
            if (imgRura1v != null) e.Graphics.DrawImage(imgRura1v, obx0 + 94, oby0 + 172);      // Rura
            // ----- Czujniki -----
            e.Graphics.DrawLine(grayPen3, obx0 + 30, oby0 + 68, obx0 + 39, oby0 + 68);          // x5
            e.Graphics.DrawLine(grayPen3, obx0 + 30, oby0 + 100, obx0 + 39, oby0 + 100);        // x6
            e.Graphics.DrawLine(grayPen3, obx0 + 30, oby0 + 132, obx0 + 39, oby0 + 132);        // x7
            e.Graphics.DrawLine(grayPen3, obx0 + 160, oby0 + 132, obx0 + 169, oby0 + 132);      // x8 (T)

            // ---------- Elementy animacyjne ----------
            // ----- Strumieñ z zaworu Z4 (prawy) -----
            if (Z2)
                e.Graphics.FillRectangle(blueBrush, obx0 + 51, oby0 + 60, (int)(qZ2 / qZ2max * 8), 80);     // Strumieñ Z2
            if (Z4)
                e.Graphics.FillRectangle(blueBrush, obx0 + 141, oby0 + 60, (int)(qZ4 / qZ4max * 8), 80);    // Strumieñ Z4

            // ----- Wype³nienie zbiornika ciecz¹  -----
            e.Graphics.FillRectangle(blueBrush, obx0 + 42, oby0 + 60 + 80 - (int)h3, 115, (int)h3);         // Zbiornik (ciecz)

            // ----- Zawór Z5 (dolny) -----
            e.Graphics.DrawString("Z5", fontObiekt, Z5 ? redBrush : blackBrush, obx0 + 115, oby0 + 153);
            if (Z5)
            {
                if (imgZawor2vB != null) e.Graphics.DrawImage(imgZawor2vB, obx0 + 92, oby0 + 152, 20, 20);  // Zawór Z5 (blue)
                e.Graphics.FillRectangle(blueBrush, obx0 + 95, oby0 + 181, (int)(qZ5 / qZ5max * 8), 20);    // Strumieñ z Z5
            }
            else
                if (imgZawor2v != null) e.Graphics.DrawImage(imgZawor2v, obx0 + 92, oby0 + 152, 20, 20);    // Zawór Z5 (szary)

            // ----- Czujnik x7 -----
            e.Graphics.DrawString("x7", fontObiekt, x7 ? redBrush : blackBrush, obx0 + 0, oby0 + 60);
            e.Graphics.FillRectangle(x7 ? redBrush : darkRedBrush, obx0 + 21, oby0 + 64, 9, 9);             // Czujnik x7

            // ----- Czujnik x6 -----
            e.Graphics.DrawString("x6", fontObiekt, x6 ? redBrush : blackBrush, obx0 + 0, oby0 + 92);
            e.Graphics.FillRectangle(x6 ? redBrush : darkRedBrush, obx0 + 21, oby0 + 96, 9, 9);             // Czujnik x6

            // ----- Czujnik x5 -----
            e.Graphics.DrawString("x5", fontObiekt, x5 ? redBrush : blackBrush, obx0 + 0, oby0 + 124);
            e.Graphics.FillRectangle(x5 ? redBrush : darkRedBrush, obx0 + 21, oby0 + 128, 9, 9);            // Czujnik x5

            // ----- Czujnik x8 -----
            //e.Graphics.DrawString("T3=" + Tzb3.ToString("0.0", nfi) + "\u00B0C", fontObiekt, x10 ? redBrush : blackBrush, obx0 + 182, oby0 + 124);
            e.Graphics.DrawString("T3", fontObiekt, x10 ? redBrush : blackBrush, obx0 + 182, oby0 + 124);
            e.Graphics.DrawString(Tzb3.ToString("0.0", nfi) + "\u00B0C", fontObiekt, x10 ? redBrush : blackBrush, obx0 + 162, oby0 + 140);
            e.Graphics.FillRectangle(x10 ? redBrush : darkRedBrush, obx0 + 169, oby0 + 128, 9, 9);           // Czujnik x8

            // ----- Czujnik H -----
            e.Graphics.DrawString("H3=" + H3.ToString("0.00", nfi) + "m", fontObiekt, x7 ? redBrush : blackBrush, obx0 + 164, oby0 + 48);
            e.Graphics.DrawLine(grayPen3, obx0 + 154, oby0 + 55, obx0 + 154, oby0 + 138);                   // Czujnik poziomu H
            e.Graphics.DrawLine(grayPen3, obx0 + 154, oby0 + 55, obx0 + 164, oby0 + 55);                    //

            // ----- Mieszad³o -----
            rysujMieszadlo(sender, e, mx, obx0 + 99, oby0 + 55);                                            // Mieszad³o
            e.Graphics.DrawString("M", fontObiekt, M ? redBrush : blackBrush, obx0 + 95, oby0 + 40);

            // ----- Grzejnik G3 -----
            rysujGrzejnik(sender, e, obx0 + 46, oby0 + 130, G3, uszkG3);                                    // Grzejnik
            e.Graphics.DrawString("G3", fontObiekt, G3 ? redBrush : blackBrush, obx0 + 52, oby0 + 142);     // G3

            // ----- Grzejnik G4 -----
            rysujGrzejnik(sender, e, obx0 + 122, oby0 + 130, G4, uszkG4);                                    // Grzejnik
            e.Graphics.DrawString("G4", fontObiekt, G4 ? redBrush : blackBrush, obx0 + 128, oby0 + 142);    // G4

            // ----- Przepe³nienie zbiornika -----
            if (ovZ3 && imgMop != null)
                e.Graphics.DrawImage(imgMop, obx0 + 85, oby0 + 86, 30, 30);                                 // Mop
        }

        public void rysujObiekt(object sender, PaintEventArgs e)                            // Kaskada zbiorników
        {
            e.Graphics.SmoothingMode = SmoothingMode.AntiAlias;                     // Wyg³adzanie
            NumberFormatInfo nfi = new CultureInfo("en-US", false).NumberFormat;
            nfi.NumberDecimalSeparator = ".";

            //e.Graphics.DrawRectangle(grayPen, 0, 0, 407, 377);                    // Obszar rysowania (tymczasowo!)
            //e.Graphics.FillRectangle(whiteBrush, 0, 0, 407, 377);                 // T³o

            const int obx0 = 30, oby0 = 0;                                       // Przesuniêcie wzglêdem (0,0)

            // ---------- Elementy animacyjne ----------
            // ----- Zbiorniki -----
            rysujZbiornikGorny1(sender, e, obx0 + 14, 52);
            rysujZbiornikGorny2(sender, e, obx0 + 164, 52);
            rysujZbiornikDolny(sender, e, obx0 + 74, 170);
            e.Graphics.DrawString("Tzewn=" + Tzew.ToString("0.0", nfi) + "\u00B0C", fontObiekt, blackBrush, 160, 18);
            e.Graphics.DrawString("PG3=" + P_G3nom.ToString("0.0", nfi) + "W", fontObiekt, blackBrush, xK3, yK3 + 38);
            /*
            e.Graphics.DrawString("bout=" + bout0, fontObiekt, blackBrush, obx0 + 10, oby0 - 135);
            e.Graphics.DrawString("P=" + (P_G1 + P_G2 + P_G3 + P_G4 + Pzb1 + Pzb2 + Pzb3 + Pzew).ToString("0.0", nfi) + " W", fontObiekt, blackBrush, obx0 + 10, oby0 - 120);
            e.Graphics.DrawString("Pzew=" + Pzew.ToString("0.0", nfi) + " W", fontObiekt, blackBrush, obx0 + 134, oby0 + 35);
            e.Graphics.DrawString("Tzb3=" + Tzb3.ToString("0.0", nfi) + "\u00B0C", fontObiekt, x10 ? redBrush : blackBrush, obx0 + 254, oby0 + 250);
            e.Graphics.DrawString("Pzb3=" + Pzb3.ToString("0.0", nfi) + " W", fontObiekt, x10 ? redBrush : blackBrush, obx0 + 254, oby0 + 265);
            e.Graphics.DrawString("T_G1=" + T_G1.ToString("0.0", nfi) + "\u00B0C", fontObiekt, x8 ? redBrush : blackBrush, obx0 + 24, oby0 + 185);
            e.Graphics.DrawString("P_G1=" + P_G1.ToString("0.0", nfi) + " W", fontObiekt, x8 ? redBrush : blackBrush, obx0 + 24, oby0 + 200);
            e.Graphics.DrawString("T_G2=" + T_G2.ToString("0.0", nfi) + "\u00B0C", fontObiekt, x9 ? redBrush : blackBrush, obx0 + 264, oby0 + 185);
            e.Graphics.DrawString("P_G2=" + P_G2.ToString("0.0", nfi) + " W", fontObiekt, x9 ? redBrush : blackBrush, obx0 + 264, oby0 + 200);
            e.Graphics.DrawString("T_G3=" + T_G3.ToString("0.0", nfi) + "\u00B0C", fontObiekt, x10 ? redBrush : blackBrush, obx0 + 64, oby0 + 335);
            e.Graphics.DrawString("P_G3=" + P_G3.ToString("0.0", nfi) + " W", fontObiekt, x10 ? redBrush : blackBrush, obx0 + 64, oby0 + 350);
            e.Graphics.DrawString("T_G4=" + T_G4.ToString("0.0", nfi) + "\u00B0C", fontObiekt, x10 ? redBrush : blackBrush, obx0 + 214, oby0 + 335);
            e.Graphics.DrawString("P_G4=" + P_G4.ToString("0.0", nfi) + " W", fontObiekt, x10 ? redBrush : blackBrush, obx0 + 214, oby0 + 350);
            */
            // --- Przyciski (opisy) ---
            e.Graphics.DrawString("+", fontKlaw, blackBrush, xK1 + 5, yK1 + 6);
            e.Graphics.DrawString("-", fontKlaw, blackBrush, xK2 + 5, yK2 + 6);
            //e.Graphics.DrawString("S", fontKlaw, blackBrush, xK3 + 5, yK3 + 6);
            //e.Graphics.DrawString("D", fontKlaw, blackBrush, xK4 + 5, yK4 + 6);
            e.Graphics.DrawString("+", fontKlaw, blackBrush, xK3 + 5, yK3 + 6);
            e.Graphics.DrawString("-", fontKlaw, blackBrush, xK4 + 5, yK4 + 6);
            // --- Przyciski ---
            rysujKlawisz(sender, e, K1, xK1, yK1);        // Przycisk K1
            rysujKlawisz(sender, e, K2, xK2, yK2);        // Przycisk K2
            rysujKlawisz(sender, e, K3, xK3, yK3);                      // Przycisk K3
            rysujKlawisz(sender, e, K4, xK4, yK4);                      // Przycisk K4
            //if (K4 == true && image3 != null) e.Graphics.DrawImage(image3, xK4 - 60, yK4 + 40, 50, 50);        // Deszcz
            //if (K3 == true && image2 != null) e.Graphics.DrawImage(image2, xK3 - 20, yK3 + 40, 50, 50);        // S³oñce
        }

    }
}
