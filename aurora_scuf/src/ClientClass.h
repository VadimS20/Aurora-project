#ifndef CLIENTCLASS_H
#define CLIENTCLASS_H

#include <QObject>
#include <chrono>
#include <thread>
#include <iostream>
#include <fstream>
#include "httplib.h"

class ClientClass : public QObject
{
    Q_OBJECT
public:
    ClientClass(QObject *parent=0);
    void upload_file(httplib::Client& client);
    bool download_file(httplib::Client& client);
    Q_INVOKABLE void setUrlPort(QString Url, int Port);
    Q_INVOKABLE void start(QString Path);
private:
    std::string path;
    int port;
    const char* url;
};


#endif // CLIENTCLASS_H
