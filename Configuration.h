#ifndef CONFIGURATION_H
#define CONFIGURATION_H

#include <QStringList>

namespace Configuration
{
    static const QString        Copyright              = "Console File Manager Application\n(c) UnitedTimurCompany 2020. All rights reserved\n";
    static const QString        MessageInputTheCommand = "Please, input your command: ";
    static const QString        MessageAdd             = "Specify the path to the file: ";
    static const QString        MessageRemove          = "Which file do you want to delete from tracking?";
    static const QString        MessageInputNumber     = "Input number: ";
    static const QString        MessageHelp            = "Add - add your file\nRemove - remove from tracking\nSize - to know the size of file\nList - to show list of files\nClear - clear console\nRemAll - deletes the entire list";
    static const QString        MessageSize            = "Which file do you want to know the size?";
    static const QString        MessageList            = "The list of files: ";
    static const QString        MessageEmptyList       = "List of files is empty\n";
    static const QString        MessageFixProblem      = "Error!";
}

#endif // CONFIGURATION_H
