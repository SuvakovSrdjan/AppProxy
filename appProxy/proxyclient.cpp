#include "proxyclient.h"

static inline QByteArray IntToArray(qint32 source);

ProxyClient::ProxyClient(QObject *parent) : QObject(parent)
{
    socket = new QTcpSocket(this);
}

bool ProxyClient::connectToHost(QString host, qint16 port)
{
    socket->connectToHost(host, port);
    return socket->waitForConnected();
}

bool ProxyClient::writeData(QByteArray data)
{
    if(socket->state() == QAbstractSocket::ConnectedState)
    {
        qDebug()<<"sending data";
        socket->write(IntToArray(data.size())); //write size of data
        socket->write(data); //write the data itself
        return socket->waitForBytesWritten();
    }
    else
        return false;
}

QByteArray IntToArray(qint32 source) //Use qint32 to ensure that the number have 4 bytes
{
    //Avoid use of cast, this is the Qt way to serialize objects
    QByteArray temp;
    QDataStream data(&temp, QIODevice::ReadWrite);
    data << source;
    return temp;
}
