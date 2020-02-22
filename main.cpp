#include <QCoreApplication>
#include <QDebug>

#include "Checkfile.h"
#include "Configuration.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    CheckFile checkFile;

    checkFile.startCheckPropertiesThread();
    checkFile.startTerminalThread();

    return a.exec();
}
