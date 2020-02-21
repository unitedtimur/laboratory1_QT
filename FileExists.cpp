#include "FileExists.h"
#include "Checkfile.h"

#include <QTimer>
#include <QDir>
#include <iostream>
#include <QDebug>

FileExists::FileExists(CheckFile* checkFile, QObject *parent) :
    QObject(parent),
    checkFile(checkFile)
{
    timer = new QTimer;

    connect(timer, &QTimer::timeout, this, &FileExists::check);
    connect(this, &FileExists::fileDisappeared, this, &FileExists::fileDisappear);
    connect(checkFile, &CheckFile::fileAppeared, [&](const QFileInfo& file)
    {
        std::cout << "File " << file.fileName().toStdString() << " was added!" << std::endl;
        files.push_back(file);
    });

    timer->start(1000);
}

void FileExists::setFiles(const QVector<QFileInfo> &files)
{
    this->files = files;
}

void FileExists::check()
{
    if (!checkFile->getFiles().isEmpty())
    {
        for (const auto& file : checkFile->getFiles())
        {
            if (!file.exists())
            {
                qDebug() << "DELETED";
            }
        }
    }
}

void FileExists::fileDisappear(const QFileInfo &file)
{
    std::cout << "File " << file.fileName().toStdString() << " was dissappear" << std::endl;
}
