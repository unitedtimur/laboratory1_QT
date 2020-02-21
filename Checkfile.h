#ifndef CHECKFILE_H
#define CHECKFILE_H

#include <QObject>
#include <QFileInfo>
#include <QThread>
#include <thread>

class CheckFile : public QObject
{
    Q_OBJECT
public:
    explicit CheckFile(QObject *parent = nullptr);
    ~CheckFile();

    QVector<QFileInfo> getFiles() const;

signals:
    void fileDisappeared(const QFileInfo& index);
    void fileAppeared(const QFileInfo& file);
    void addedFile();
    void commandEntered();

public slots:
    void inputCommand();

private:
    QVector<QString>    commands;
    QVector<QFileInfo>  files;
    CheckFile*          ptr;
    class QTimer*       timer;
};


#endif // CHECKFILE_H
