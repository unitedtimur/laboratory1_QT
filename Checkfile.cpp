#include "Checkfile.h"
#include "Configuration.h"

#include <QTextStream>
#include <QFileInfoList>
#include <QDir>

#include <thread>

CheckFile::CheckFile(QObject *parent) :
    QObject(parent)
{
    commands = {
        QString("add"),
        QString("remove"),
        QString("size"),
        QString("list"),
        QString("help"),
        QString("clear"),
        QString("remall")
    };

    connect(&fileSystemWatcher, &QFileSystemWatcher::fileChanged, [&](const QString& fileName)
    {
       if (QFileInfo(fileName).exists())
       {
            QTextStream cout(stdout);
            cout << flush << "\n\t" << fileName << " was changed!" << endl;
            cout << "\tSize is " << QFileInfo(fileName).size() << " byte" << endl;
            cout << Configuration::MessageInputTheCommand;
       }
    });

    connect(this, &CheckFile::fileAdded, [&](const QString& fileName)
    {
        QTextStream cout(stdout);
        cout << flush << '\t' << fileName << " was added to list" << endl;
        fileSystemWatcher.addPath(fileName);
        fileNames.push_back(fileName);
    });

    connect(this, &CheckFile::fileRemoved, [&](const qint32& index)
    {
        QTextStream cout(stdout);
        cout << flush << '\t' << fileNames[index] << " was removed from list" << endl;
        fileSystemWatcher.removePath(fileNames[index]);
        fileNames.remove(index);
    });

    connect(this, &CheckFile::enteredSize, [&](const qint32& index)
    {
        QTextStream cout(stdout);
        cout << flush << '\t' << fileNames[index] << " size is equal " << QFileInfo(fileNames[index]).size() << " byte" << endl;
    });
}

CheckFile::~CheckFile()
{
    // empty
}

void CheckFile::startTerminalThread()
{
    std::thread inputTerminalThread(&CheckFile::terminal, this);

    inputTerminalThread.detach();
}

void CheckFile::startCheckPropertiesThread()
{
    std::thread checkPropertiesThread(&CheckFile::checkProperties, this);

    checkPropertiesThread.detach();
}

void CheckFile::terminal()
{
    QTextStream cin(stdin), cout(stdout);

    auto printListFiles = [&](){

        qint32 iter = 0;

        foreach (const auto& fileName, fileNames)
        {
            cout << '\t' << ++iter << " ---> " << fileName << endl;
        }
    };

    std::this_thread::sleep_for(std::chrono::milliseconds(100));

    forever
    {
        QString command;

        bool isCommand = false;

        while (!isCommand)
        {
            cout << flush << Configuration::MessageInputTheCommand << flush;
            command = cin.readLine().trimmed().toLower();
            std::find(commands.begin(), commands.end(), command) != commands.end() ? isCommand = true : isCommand = false;

            if (!isCommand && command.isEmpty())
            {
                command = commands[4];
                isCommand = true;
            }
        }

        // command == 'add'
        if (command == commands[0])
        {
            QString pathToFile;
            QFileInfoList list;
            bool isDir = false;
            bool isAdd = false;

            while (!isAdd)
            {
                cout << '\t' << Configuration::MessageAdd << flush;
                pathToFile = cin.readLine().trimmed();

                if (std::find(pathToFile.begin(), pathToFile.end(), '"') != pathToFile.end())
                    pathToFile = pathToFile.mid(1, pathToFile.size() - 2);

                if (QFileInfo(pathToFile).exists())
                {
                    if (QFileInfo(pathToFile).isFile() && std::find(fileNames.begin(), fileNames.end(), pathToFile) == fileNames.end())
                    {
                        isAdd = true;
                    }
                    else
                    {
                        isAdd = false;

                        if (QFileInfo(pathToFile).isDir() && std::find(fileNames.begin(), fileNames.end(), pathToFile) == fileNames.end())
                        {
                            isDir = true;
                            isAdd = true;

                            QDir dir(pathToFile);
                            list = dir.entryInfoList(QDir::Files);

                            if (list.isEmpty())
                            {
                                isDir = false;
                                isAdd = false;
                            }
                        }
                    }
                }
                else
                {
                    isAdd = false;
                }
            }

            if (isDir)
            {
                for (const auto& it : list)
                {
                    emit fileAdded(it.filePath());
                }
            }
            else
            {
                emit fileAdded(pathToFile);
            }

            continue;
        }

        // command == 'remove'
        if (command == commands[1])
        {
            if (!fileNames.isEmpty())
            {
                cout << flush << '\t' << Configuration::MessageRemove << endl;
                printListFiles();

                qint32 number = 0;

                bool isRemove = false;

                while (!isRemove)
                {
                    cout << Configuration::MessageInputNumber << flush;
                    number = cin.readLine().toInt();

                    if (number > 0 && number <= fileNames.size())
                    {
                        emit fileRemoved(--number);
                        isRemove = true;
                    }
                    else
                    {
                        cout << Configuration::MessageFixProblem << endl;
                    }
                }
            }
            else
            {
                cout << flush << '\t' << Configuration::MessageEmptyList << endl;
            }

            continue;
        }

        // command == 'size'
        if (command == commands[2])
        {
            if (!fileNames.isEmpty())
            {
                cout << flush << '\t' << Configuration::MessageSize << endl;
                printListFiles();

                qint32 number = 0;

                bool isSize = false;

                while (!isSize)
                {
                    cout << Configuration::MessageInputNumber << flush;
                    number = cin.readLine().toInt();

                    if (number > 0 && number <= fileNames.size())
                    {
                        emit enteredSize(--number);
                        isSize = true;
                    }
                }
            }
            else
            {
                cout << flush << '\t' << Configuration::MessageEmptyList << endl;
            }

            continue;
        }

        // command == 'list'
        if (command == commands[3])
        {
            if (!fileNames.isEmpty())
            {
                cout << flush << '\t' << Configuration::MessageList << endl;
                printListFiles();
            }
            else
            {
                cout << flush << '\t' << Configuration::MessageEmptyList << endl;
            }

            continue;
        }

        // command == 'help'
        if (command == commands[4])
        {
            QStringList help = Configuration::MessageHelp.split('\n');

            foreach (const auto& it, help)
            {
                cout << '\t' << it << endl;
            }

            continue;
        }

        // command == 'clear'
        if (command == commands[5])
        {
            #ifdef Q_OS_WIN32
                system("cls");
            #endif

            #ifdef Q_OS_LINUX
                system("clear");
            #endif

            continue;
        }

        // command == 'remAll'
        if (command == commands[6])
        {
            if (!fileNames.isEmpty())
            {
                for (qint32 i = fileNames.size(); i > 0; --i)
                {
                    emit fileRemoved(i - 1);
                }

                QTextStream(stdout) << "\tAll files was removed!" << endl;
            }
            else
            {
                cout << flush << '\t' << Configuration::MessageEmptyList << endl;
            }

            continue;
        }
    }
}

void CheckFile::checkProperties()
{
    forever
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(100));

        QTextStream cout(stdout);

        if (!fileNames.isEmpty())
        {
            qint32 iter = 0;
            bool isDelete = false;

            for (const auto& fileName : fileNames)
            {
                if (!QFileInfo(fileName).exists())
                {
                    isDelete = true;
                    cout << endl;
                }
                ++iter;
            }

            if (isDelete)
            {
                cout << flush << Configuration::MessageInputTheCommand;
            }
        }

        cout << flush;
    }
}
