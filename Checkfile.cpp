#include "Checkfile.h"
#include "Configuration.h"

#include <QTextStream>
#include <QFileInfo>
#include <thread>

CheckFile::CheckFile(QObject *parent) :
    QObject(parent)
{
    commands = {
        QString("add"),
        QString("remove"),
        QString("size"),
        QString("list"),
        QString("help")
    };


    connect(this, &CheckFile::fileAdded, [&](const QString& fileName)
    {
        QTextStream cout(stdout);
        cout << '\t' << fileName << " was added to list" << endl;
        fileNames.push_back(fileName);
    });

    connect(this, &CheckFile::fileRemoved, [&](const qint32& index)
    {
        QTextStream cout(stdout);
        cout << '\t' << fileNames[index] << " was removed from list" << endl;
        fileNames.remove(index);
    });

    connect(this, &CheckFile::enteredSize, [&](const qint32& index)
    {
        QTextStream cout(stdout);
        cout << '\t' << fileNames[index] << " size is equal " << QFileInfo(fileNames[index]).size() << " byte" << endl;
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
            cout << Configuration::MessageInputTheCommand << flush;
            command = cin.readLine().trimmed().toLower();
            std::find(commands.begin(), commands.end(), command) != commands.end() ? isCommand = true : isCommand = false;
        }

        // command == 'add'
        if (command == commands[0])
        {
            QString pathToFile;

            bool isAdd = false;

            while (!isAdd)
            {
                cout << Configuration::MessageAdd << flush;
                pathToFile = cin.readLine().trimmed();
                QFileInfo(pathToFile).exists() && QFileInfo(pathToFile).isFile() ? isAdd = true : isAdd = false;
            }

            emit fileAdded(pathToFile);

            continue;
        }

        // command == 'remove'
        if (command == commands[1])
        {
            if (!fileNames.isEmpty())
            {
                cout << Configuration::MessageRemove << endl;
                printListFiles();

                qint32 number = 0;

                bool isRemove = false;

                while (!isRemove)
                {
                    cout << Configuration::MessageInputNumber << flush;
                    number = cin.readLine().toInt();

                    if (number <= fileNames.size())
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
                cout << Configuration::MessageEmptyList << endl;
            }

            continue;
        }

        // command == 'size'
        if (command == commands[2])
        {
            if (!fileNames.isEmpty())
            {
                cout << Configuration::MessageSize << endl;
                printListFiles();

                qint32 number = 0;

                bool isSize = false;

                while (!isSize)
                {
                    cout << Configuration::MessageInputNumber << flush;
                    number = cin.readLine().toInt();

                    if (number <= fileNames.size())
                    {
                        emit enteredSize(--number);
                        isSize = true;
                    }
                }
            }
            else
            {
                cout << Configuration::MessageEmptyList << endl;
            }

            continue;
        }

        // command == 'list'
        if (command == commands[3])
        {
            if (!fileNames.isEmpty())
            {
                cout << Configuration::MessageList << endl;
                printListFiles();
            }
            else
            {
                cout << Configuration::MessageEmptyList << endl;
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

    }
}

void CheckFile::checkProperties()
{
    forever
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(100));

        if (!fileNames.isEmpty())
        {
            for (const auto& fileName : fileNames)
            {
                if (!QFileInfo(fileName).exists())
                {
                    QTextStream cout(stdout);
                    cout << "\n\t" << fileName << " was removed from list" << endl;

                    if (std::find(fileNames.begin(), fileNames.end(), fileName) != fileNames.end())
                        fileNames.erase(std::find(fileNames.begin(), fileNames.end(), fileName));
                }
            }
        }
    }
}

