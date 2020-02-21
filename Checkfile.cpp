#include "Checkfile.h"
#include "Configuration.h"

#include <QFileInfo>
#include <QTimer>
#include <QDebug>
#include <QThread>
#include <iostream>
#include <QFuture>

#include <future>
#include <thread>
#include <chrono>

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

    timer = new QTimer;

    connect(timer, &QTimer::timeout, this, &CheckFile::inputCommand);

    timer->start(100);



    //std::thread newThread(&inputCommand);
    //newThread.detach();
}

CheckFile::~CheckFile()
{
    delete timer;
}

QVector<QFileInfo> CheckFile::getFiles() const
{
    return files;
}

void CheckFile::inputCommand()
{
    QTextStream cin(stdin);

    QString command;

    while (command.isEmpty())
      {
          std::cout << Configuration::MessageInputTheCommand.toStdString();
          command = cin.readLine();

          command = command.toLower();

          if (std::find(commands.begin(), commands.end(), command) == commands.end())
              command.clear();
      }

      // command == 'add'
      if (command.toLower() == commands[0])
      {
          QString fileName;

          while (fileName.isEmpty())
          {
              std::cout << Configuration::MessageAdd.toStdString();
              fileName = cin.readLine();

              if (!QFileInfo(fileName).exists() || !QFileInfo(fileName).isFile())
                  fileName.clear();
          }

          files.push_back(QFileInfo(fileName));

          //emit fileAppeared(QFileInfo(fileName));
      }

      // command == 'remove'
      if (command.toLower() == commands[1])
      {
          if (files.empty())
          {
              std::cout << Configuration::MessageEmptyList.toStdString() << std::endl;
              return;
          }

          std::cout << Configuration::MessageRemove.toStdString() << std::endl;

          qint32 counter = 0;

          for (const auto& file : files)
          {
              std::cout << "\t\t" << ++counter << " ---> " << file.fileName().toStdString() << std::endl;
          }

          qint32 rem = -1;

          forever
          {
              std::cout << Configuration::MessageInputNumber.toStdString();

              rem = cin.readLine().toInt();

              if (rem >= 0 && rem <= files.size())
              {
                  files.remove(--rem);
                  std::cout << Configuration::MessageAfterRemove.toStdString() << std::endl;
                  break;
              }
              else
              {
                  std::cout << Configuration::MessageFixProblem.toStdString() << std::endl;
              }
          }
      }

      // command = 'size'
      if (command.toLower() == commands[2])
      {
          if (files.empty())
          {
              std::cout << Configuration::MessageEmptyList.toStdString() << std::endl;
              return;
          }

          std::cout << Configuration::MessageSize.toStdString() << std::endl;

          qint32 counter = 0;

          for (const auto& file : files)
          {
              std::cout << "\t" << ++counter << " ---> " << file.fileName().toStdString() << std::endl;
          }

          qint32 index = -1;

          forever
          {
              std::cout << Configuration::MessageInputNumber.toStdString();

              index = cin.readLine().toInt();

              if (--index >= 0 && index <= files.size() && QFileInfo(files[index]).exists())
              {
                  std::cout << "\tThe size if equal " <<  QFileInfo(files[index]).size() << " byte" << std::endl;
                  break;
              }
              else
              {
                  std::cout << Configuration::MessageFixProblem.toStdString() << std::endl;
              }
          }
      }

      // command == 'list'
      if (command.toLower() == commands[3])
      {
          if (files.empty())
          {
              std::cout << Configuration::MessageEmptyList.toStdString() << std::endl;
              return;
          }

          std::cout << Configuration::MessageList.toStdString() << std::endl;

          qint32 counter = 0;

          for (const auto& file : files)
          {
              std::cout << "\t" << ++counter << " ---> " << file.fileName().toStdString() << std::endl;
          }
      }

      // command == 'help'
      if (command.toLower() == commands[4])
      {
          QStringList list = Configuration::MessageHelp.split('\n');

          if (!list.isEmpty())
          {
              for (const auto& it : list)
              {
                  std::cout << "\t" << it.toStdString() << std::endl;
              }
          }
      }

      std::cout << std::endl;
}

