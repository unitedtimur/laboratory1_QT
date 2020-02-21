#ifndef FILEEXISTS_H
#define FILEEXISTS_H

#include <QObject>
#include <QVector>
#include <QFileInfo>

class FileExists : public QObject
{
    Q_OBJECT
public:
    explicit FileExists(class CheckFile* checkFile, QObject *parent = nullptr);

    void setFiles(const QVector<QFileInfo>& files);

signals:
    void fileDisappeared(const QFileInfo& file);

public slots:
    void check();
    void fileDisappear(const QFileInfo& file);

private:
    QVector<QFileInfo>  files;
    class QTimer*       timer;
    class CheckFile*    checkFile;
};

#endif // FILEEXISTS_H
