#include <QCoreApplication>
#include <QDebug>

#include "Checkfile.h"
#include "Configuration.h"

#include <QDir>
#include <QFileInfoList>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    CheckFile checkFile;

    checkFile.startCheckPropertiesThread();
    checkFile.startTerminalThread();

    //QString string = "E:\Education\Practics\Projects C++(QT)\Laboratory1\src\test1.txt";
    QTextStream cout(stdout);
    //cout << string.split(QRegExp("\"")).join(' ') << endl;

    QDir dir("E:/Education/Practics/Projects C++(QT)/Laboratory1/src");

    dir.setFilter(QDir::Files);
    QFileInfoList list = dir.entryInfoList();

    for (const auto& it : list)
    {
        cout << it.path();
    }

    return a.exec();
}
