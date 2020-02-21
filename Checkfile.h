#ifndef CHECKFILE_H
#define CHECKFILE_H

#include <QObject>

class CheckFile : public QObject
{
    Q_OBJECT
public:
    explicit CheckFile(QObject *parent = nullptr);
    ~CheckFile();

    void startTerminalThread();
    void startCheckPropertiesThread();

signals:
    void fileAdded(const QString& fileName);
    void fileRemoved(const qint32& index);
    void enteredSize(const qint32& index);

private slots:
    void terminal();
    void checkProperties();

private:
    QVector<QString> commands;
    QVector<QString> fileNames;
};

#endif // CHECKFILE_H
