#include "client.h"


client::client(QString ip, quint16 port)
{
    this->ip = ip;
    this->port = port;



}

void client::sendToServer(QString str)
{
    socket.connectToHost(ip, port);
    socket.waitForConnected(3000);
    QFile file(str);   //file path
    file.open(QIODevice::ReadOnly);
    QByteArray q = file.readAll();
    socket.write(q);
}

void client::setPath(QString newPath){
    this->path=newPath;
    this->sendToServer(path);
    qDebug()<<"started";
    emit transmitNewText(path+" MyClient");
}
