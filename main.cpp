#include <QCoreApplication>
#include <QDebug>

#include "Checkfile.h"
#include "FileExists.h"
#include "Configuration.h"

#include <QThread>
#include <thread>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    CheckFile checkFile;
    FileExists exists(&checkFile);
    //std::thread newThread(checkFile.inputCommand);

    return a.exec();
}
