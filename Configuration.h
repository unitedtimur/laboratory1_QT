#ifndef CONFIGURATION_H
#define CONFIGURATION_H

#include <QStringList>

namespace Configuration
{
    static const QStringList    pathMain = QString(__FILE__).split('\\');
    static const QString        pathToSrc = pathMain[0] + '/' + pathMain[1] + "/src";
    static const QString        test1 = pathToSrc + "/test1.txt";
    static const QString        test2 = pathToSrc + "/test2.txt";
    static const QString        test3 = pathToSrc + "/test3.txt";

    static const QString        MessageInputTheCommand = "Please, input your command: ";
    static const QString        MessageAdd             = "Specify the path to the file: ";
    static const QString        MessageRemove          = "Which file do you want to delete from tracking?";
    static const QString        MessageInputNumber     = "Input number: ";
    static const QString        MessageAfterRemove     = "File was removed";
    static const QString        MessageHelp            = "Add - add your file\nRemove - remove from tracking\nSize - to know the size of file\nList - to show list of files";
    static const QString        MessageSize            = "Which file do you want to know the size?";
    static const QString        MessageList            = "The list of files: ";
    static const QString        MessageEmptyList       = "List of files is empty\n";
    static const QString        MessageFixProblem      = "Error!";
}

#endif // CONFIGURATION_H
