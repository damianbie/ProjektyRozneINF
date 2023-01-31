using System;
using System.Collections.Generic;
using System.Windows.Forms;
using System.ComponentModel;
using System.IO;
using BazaObiekt;

namespace ObiektSym
{
    static class Program
    {
        /// <summary>
        /// The main entry point for the application.
        /// </summary>
        [STAThread]
        static void Main(string[] args)
        {
            Application.EnableVisualStyles();
            Application.SetCompatibleTextRenderingDefault(false);

            Baza forma = new Baza();
            forma.LoadKonfig(args);

            Application.Run(forma);
        }
    }
}