#include <QCoreApplication>
#include <QDebug>

#include "Checkfile.h"
#include "Configuration.h"

#include <thread>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    CheckFile checkFile;

    checkFile.startTerminalThread();
    checkFile.startCheckPropertiesThread();

    return a.exec();
}
