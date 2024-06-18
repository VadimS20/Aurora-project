#include "fileclass.h"




fileClass::fileClass(QObject *parent):QObject(parent)
{

}

void fileClass::fileOpen()
{
    QFile file(path);
    if(file.open(QIODevice::ReadOnly)){
        std::cout<<"file opened"<<"\n";
        file.close();

    }
    else{
        std::cout<<"Error"<<"\n";

    }
}

void fileClass::setPath(QString tempPath)
{
    path=tempPath;
    qDebug() <<"msg recieved"<<"\n";
    emit transmitNewText(path);
}

QString fileClass::getPath()
{
    if(path==NULL){
        return 0;
    }
    else{
        return path;
    }
}
