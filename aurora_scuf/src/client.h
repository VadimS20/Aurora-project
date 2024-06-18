#ifndef CLIENT_H
#define CLIENT_H

#include <QTcpSocket>
#include <QTcpServer>
#include <QDataStream>
#include <QFile>
#include <QObject>

class client: public QObject
{
    Q_OBJECT
public:
    client(QString ip, quint16 port);
    void sendToServer(QString str);
private:
    QTcpSocket socket;
    QString ip;
    quint16 port;
    QString path;
public slots:
    void setPath(QString);
signals:
    void transmitNewText(QString text);
//public slots:
//   void slotReadyRead();
};

#endif // CLIENT_H
