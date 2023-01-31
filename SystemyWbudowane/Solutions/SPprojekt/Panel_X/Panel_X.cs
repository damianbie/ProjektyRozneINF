using System;
using System.Collections.Generic;
using System.Text;
using System.Windows.Forms;
using System.Drawing;
using System.Drawing.Drawing2D;
using System.Globalization;

namespace Panele
{
    public class Panel_X : PictureBox
    {
        // Panel X
        //private char[] LCD_linia0 = new char[41];
        private string LCD_linia1 = "1234567890123456                ";
        private string LCD_linia2 = "1234567890123456                ";
        private bool K1 = true;         // Klawisz K1 (naciœniêty => false)
        private bool K2 = true;         // Klawisz K2 (naciœniêty => false)
        private bool K3 = true;         // Klawisz K3 (naciœniêty => false)
        private bool K4 = true;         // Klawisz K4 (naciœniêty => false)
        private bool K5 = true;         // Klawisz K5 (naciœniêty => false)
        private bool K6 = true;         // Klawisz K6 (naciœniêty => false)
        private bool K7 = true;         // Klawisz K7 (naciœniêty => false)
        private bool K8 = true;         // Klawisz K8 (naciœniêty => false)
        private bool L1 = false;        // Dioda L1 (true => œwieci)
        private bool L2 = false;        // Dioda L2 (true => œwieci)
        private bool L3 = false;        // Dioda L3 (true => œwieci)
        private bool L4 = false;        // Dioda L4 (true => œwieci)
        private bool L5 = false;        // Dioda L5 (true => œwieci)
        private bool L6 = false;        // Dioda L6 (true => œwieci)
        private bool L7 = false;        // Dioda L7 (true => œwieci)
        private bool L8 = false;        // Dioda L8 (true => œwieci)
        private int x0 = 20, y0 = 20;   // Przesuniêcie panelu wzglêdem (0,0)
        private int xK1, yK1;           // wspó³rzêdne klawisza K1 wzglêdem (0,0)
        private int xK2, yK2;           // wspó³rzêdne klawisza K2 wzglêdem (0,0)
        private int xK3, yK3;           // wspó³rzêdne klawisza K3 wzglêdem (0,0)
        private int xK4, yK4;           // wspó³rzêdne klawisza K4 wzglêdem (0,0)
        private int xK5, yK5;           // wspó³rzêdne klawisza K5 wzglêdem (0,0)
        private int xK6, yK6;           // wspó³rzêdne klawisza K6 wzglêdem (0,0)
        private int xK7, yK7;           // wspó³rzêdne klawisza K7 wzglêdem (0,0)
        private int xK8, yK8;           // wspó³rzêdne klawisza K8 wzglêdem (0,0)

        public Panel_X()
        {
            xK1 = x0 + 20; yK1 = y0 + 160;
            xK2 = x0 + 60; yK2 = y0 + 160;
            xK3 = x0 + 100; yK3 = y0 + 160;
            xK4 = x0 + 140; yK4 = y0 + 160;
            xK5 = x0 + 180; yK5 = y0 + 160;
            xK6 = x0 + 220; yK6 = y0 + 160;
            xK7 = x0 + 260; yK7 = y0 + 160;
            xK8 = x0 + 300; yK8 = y0 + 160;
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
            if (e.X >= xK7 && e.X <= xK7 + 30 && e.Y >= yK7 && e.Y <= yK7 + 30)
            { K7 = K7 ? false : true; }
            if (e.X >= xK8 && e.X <= xK8 + 30 && e.Y >= yK8 && e.Y <= yK8 + 30)
            { K8 = K8 ? false : true; }
        }
        public void czytajKeyDn(object sender, KeyEventArgs e)
        {
            switch (e.KeyCode)
            {
                case Keys.D1: K1 = false; break;    // '1'
                case Keys.D2: K2 = false; break;    // '2'
                case Keys.D3: K3 = false; break;    // '3'
                case Keys.D4: K4 = false; break;    // '4'
                case Keys.D5: K5 = false; break;    // '5'
                case Keys.D6: K6 = false; break;    // '6'
                case Keys.D7: K7 = false; break;    // '7'
                case Keys.D8: K8 = false; break;    // '8'
            }
        }
        public void czytajKeyUp(object sender, KeyEventArgs e)
        {
            switch (e.KeyCode)
            {
                case Keys.D1: K1 = true; break;     // '1'
                case Keys.D2: K2 = true; break;     // '2'
                case Keys.D3: K3 = true; break;     // '3'
                case Keys.D4: K4 = true; break;     // '4'
                case Keys.D5: K5 = true; break;     // '5'
                case Keys.D6: K6 = true; break;     // '6'
                case Keys.D7: K7 = true; break;     // '7'
                case Keys.D8: K8 = true; break;     // '8'
            }
        }

        public void obliczPanel(ref ushort keys, ushort leds, char[] buf1, char[] buf2, byte[] seg7)
        {
            ushort tmp = 0;
            if (K1) tmp |= 0x01;
            if (K2) tmp |= 0x02;
            if (K3) tmp |= 0x04;
            if (K4) tmp |= 0x08;
            if (K5) tmp |= 0x10;
            if (K6) tmp |= 0x20;
            if (K7) tmp |= 0x40;
            if (K8) tmp |= 0x80;
            keys = tmp;
            L1 = (leds & 0x01) != 0 ? true : false;
            L2 = (leds & 0x02) != 0 ? true : false;
            L3 = (leds & 0x04) != 0 ? true : false;
            L4 = (leds & 0x08) != 0 ? true : false;
            L5 = (leds & 0x10) != 0 ? true : false;
            L6 = (leds & 0x20) != 0 ? true : false;
            L7 = (leds & 0x40) != 0 ? true : false;
            L8 = (leds & 0x80) != 0 ? true : false;

            try { LCD_linia1 = new string(buf1, 0, 20); }
            catch (Exception ex) { MessageBox.Show(ex.ToString()); }
            try { LCD_linia2 = new string(buf2, 0, 20); }
            catch (Exception ex) { MessageBox.Show(ex.ToString()); }
        }

        public void rysujKlawisz(object sender, PaintEventArgs e, bool Kx, int x, int y)
        {
            Pen keyPen = new Pen(Color.FromArgb(60, 255, 0, 0), 1); // Pióro: jasno ró¿owy, gruboœæ:1
            Pen blackPen = new Pen(Color.Black, 1);                 // Pióro: czarny, gruboœæ:1
            Pen whitePen = new Pen(Color.White, 1);                 // Pióro: bia³y, gruboœæ:1
            Pen grayPen = new Pen(Color.Gray, 1);                   // Pióro: szary, gruboœæ:1

            e.Graphics.DrawRectangle(blackPen, x, y, 30, 30);       // Obrys klawisza

            e.Graphics.DrawLine(grayPen, x + 29, y + 1, x + 29, y + 29);      // Linia:
            e.Graphics.DrawLine(grayPen, x + 1, y + 29, x + 29, y + 29);      // Linia:
            if (Kx)
            {   // Klawisz puszczony
                e.Graphics.DrawLine(whitePen, x + 1, y + 1, x + 1, y + 29);   // Linia:
                e.Graphics.DrawLine(whitePen, x + 1, y + 1, x + 29, y + 1);   // Linia:
                e.Graphics.DrawLine(grayPen, x + 28, y + 2, x + 28, y + 28);  // Linia:
                e.Graphics.DrawLine(grayPen, x + 2, y + 28, x + 28, y + 28);  // Linia:
            }
            else
            {   // Klawisz wciœniêty
                e.Graphics.DrawLine(grayPen, x + 1, y + 1, x + 1, y + 29);    // Linia:
                e.Graphics.DrawLine(grayPen, x + 1, y + 1, x + 29, y + 1);    // Linia:
                e.Graphics.DrawLine(keyPen, x + 28, y + 2, x + 28, y + 28);   // Linia:
                e.Graphics.DrawLine(keyPen, x + 2, y + 28, x + 28, y + 28);   // Linia:
            }
        }
        public void rysujLED(object sender, PaintEventArgs e, bool Lx, int x, int y)
        {
            SolidBrush redBrush = new SolidBrush(Color.Red);            // Czerwony
            SolidBrush darkRedBrush = new SolidBrush(Color.DarkRed);    // Ciemnoczerwony
            Pen blackPen = new Pen(Color.Black, 1);                     // Pióro: czarny, gruboœæ:1

            if (Lx) e.Graphics.FillEllipse(redBrush, x, y, 12, 12);     // Œwiec¹cy LED
            else e.Graphics.FillEllipse(darkRedBrush, x, y, 12, 12);    // Zgaszony LED

            e.Graphics.DrawEllipse(blackPen, x, y, 12, 12);             // Czarny obrys
        }

        public void rysujPanel(object sender, PaintEventArgs e)
        {
            e.Graphics.PageScale = 1.0F;
            e.Graphics.TranslateTransform(0.0F, 0.0F);
            e.Graphics.SmoothingMode = SmoothingMode.AntiAlias;     // Wyg³adzanie
            Pen blackPen = new Pen(Color.Black, 1);                 // Pióro: czarny, gruboœæ:1
            SolidBrush blackBrush = new SolidBrush(Color.Black);    // Czarny
            SolidBrush blueBrush = new SolidBrush(Color.SkyBlue);   // B³êkitny
            SolidBrush LCDBrush = new SolidBrush(Color.FromArgb(100, 0, 255, 0));  // Jasnozielony(alfa,R,G,B)

            FontFamily fontFamily = new FontFamily("Arial");
            Font font = new Font(fontFamily, 16, FontStyle.Regular, GraphicsUnit.Pixel);

            //e.Graphics.DrawRectangle(blackPen, 0, 0, 340, 340);     // Obszar rysowania (tymczasowo!)

            //----- Elementy sta³e (mo¿na zast¹piæ obrazem) -----
            // T³o
            e.Graphics.FillRectangle(blackBrush, x0 + 0, y0 + 0, 350, 230); // Czarny spód
            e.Graphics.FillRectangle(blueBrush, x0 + 5, y0 + 5, 340, 220);  // B³êkitny œrodek
            e.Graphics.FillRectangle(LCDBrush, x0 + 45, y0 + 30, 260, 50);  // Pole LCD
            e.Graphics.DrawRectangle(blackPen, x0 + 45, y0 + 30, 260, 50);  // Obrys LCD    

            // --- Diody LED (opisy) ---
            e.Graphics.DrawString("L1", font, blackBrush, xK1 + 4, y0 + 100);
            e.Graphics.DrawString("L2", font, blackBrush, xK2 + 4, y0 + 100);
            e.Graphics.DrawString("L3", font, blackBrush, xK3 + 4, y0 + 100);
            e.Graphics.DrawString("L4", font, blackBrush, xK4 + 4, y0 + 100);
            e.Graphics.DrawString("L5", font, blackBrush, xK5 + 4, y0 + 100);
            e.Graphics.DrawString("L6", font, blackBrush, xK6 + 4, y0 + 100);
            e.Graphics.DrawString("L7", font, blackBrush, xK7 + 4, y0 + 100);
            e.Graphics.DrawString("L8", font, blackBrush, xK8 + 4, y0 + 100);

            // --- Przyciski (opisy) ---
            e.Graphics.DrawString("K1", font, blackBrush, xK1 + 3, yK1 + 6);
            e.Graphics.DrawString("K2", font, blackBrush, xK2 + 3, yK2 + 6);
            e.Graphics.DrawString("K3", font, blackBrush, xK3 + 3, yK3 + 6);
            e.Graphics.DrawString("K4", font, blackBrush, xK4 + 3, yK4 + 6);
            e.Graphics.DrawString("K5", font, blackBrush, xK5 + 3, yK5 + 6);
            e.Graphics.DrawString("K6", font, blackBrush, xK6 + 3, yK6 + 6);
            e.Graphics.DrawString("K7", font, blackBrush, xK7 + 3, yK7 + 6);
            e.Graphics.DrawString("K8", font, blackBrush, xK8 + 3, yK8 + 6);

            //---------- Elementy animacyjne ----------

            // --- Diody LED ---
            rysujLED(sender, e, L1, x0 + 30, y0 + 120);   // Dioda L1
            rysujLED(sender, e, L2, x0 + 70, y0 + 120);   // Dioda L2
            rysujLED(sender, e, L3, x0 + 110, y0 + 120);  // Dioda L3
            rysujLED(sender, e, L4, x0 + 150, y0 + 120);  // Dioda L4
            rysujLED(sender, e, L5, x0 + 190, y0 + 120);  // Dioda L5
            rysujLED(sender, e, L6, x0 + 230, y0 + 120);  // Dioda L6
            rysujLED(sender, e, L7, x0 + 270, y0 + 120);  // Dioda L7
            rysujLED(sender, e, L8, x0 + 310, y0 + 120);  // Dioda L8

            // --- Przyciski ---
            rysujKlawisz(sender, e, K1, xK1, yK1);        // Przycisk K1
            rysujKlawisz(sender, e, K2, xK2, yK2);        // Przycisk K2
            rysujKlawisz(sender, e, K3, xK3, yK3);        // Przycisk K3
            rysujKlawisz(sender, e, K4, xK4, yK4);        // Przycisk K4
            rysujKlawisz(sender, e, K5, xK5, yK5);        // Przycisk K1
            rysujKlawisz(sender, e, K6, xK6, yK6);        // Przycisk K2
            rysujKlawisz(sender, e, K7, xK7, yK7);        // Przycisk K3
            rysujKlawisz(sender, e, K8, xK8, yK8);        // Przycisk K4

            // --- Wyœwietlacz LCD ---
            FontFamily fontCourier = new FontFamily("Courier New");
            Font fontLCD = new Font(fontCourier, 20, FontStyle.Regular, GraphicsUnit.Pixel);
            //e.Graphics.DrawString(LCD_linia0.ToString(), fontLCD, blackBrush, x0 + 54, y0 + 35);
            e.Graphics.DrawString(LCD_linia1, fontLCD, blackBrush, x0 + 54, y0 + 35);
            e.Graphics.DrawString(LCD_linia2, fontLCD, blackBrush, x0 + 54, y0 + 55);
        }
    }
}
