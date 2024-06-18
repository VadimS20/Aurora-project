#ifndef FILECLASS_H
#define FILECLASS_H

#include <QTcpSocket>
#include <QObject>
#include <QFile>
#include <iostream>
#include <QDebug>

class fileClass : public QObject
{
    Q_OBJECT
public:
    explicit fileClass(QObject *parent=0);
private:
    QTcpSocket *socket;
    QByteArray Data;
    QString path;
    void fileOpen();
public slots:
    void setPath(QString tempPath);
    QString getPath();
signals:
    void transmitNewText(QString text);

};


#endif // FILECLASS_H
