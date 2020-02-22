#include <QCoreApplication>
#include <QDebug>

#include "Checkfile.h"
#include "Configuration.h"

#include <iostream>
#include <windows.h>
#include <conio.h>
#include <dos.h>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    QTextStream(stdout) << Configuration::Copyright << endl;

    CheckFile checkFile;

    checkFile.startTerminalThread();
    checkFile.startCheckPropertiesThread();

    //HANDLE  hConsole;
    //  int k;
    //
    //hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    //// you can loop k higher to see more color choices
    //for(k = 1; k < 255; k++)
    //{
    //  // pick the colorattribute k you want
    //  SetConsoleTextAttribute(hConsole, k);
    //  QTextStream(stdout) << k << " I want to be nice today!" << endl;
    //}

    return a.exec();
}
