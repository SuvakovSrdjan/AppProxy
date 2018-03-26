#include "mytcpclient.h"


static inline QByteArray IntToArray(qint32 source);
static inline qint32 ArrayToInt(QByteArray source);
static inline QByteArray StrToArray(QString source);

MyTcpClient::MyTcpClient(QObject *parent) : QObject(parent)
{
    socket = new QTcpSocket(this);
    authenticated = false;
}

bool MyTcpClient::connectToHost(QString host)
{
    socket->connectToHost( host , 9001);
    return socket->waitForConnected();
}

bool MyTcpClient::isAuthenticated()
{
  return authenticated;
}

bool MyTcpClient::authentication(QString username, QString password)
{
    this->sendUsername(username, password);
    if(isAuthenticated())
    {
        qDebug()<<"Authentication Success! ";
        return true;
    }
    else
    {
        qDebug()<<"Authentication Fail!! ";
        return false;
    }
}

bool MyTcpClient::authMedtodRequest(qint32 method)
{
    qDebug()<<"Sending authentication method";
    socket->write(IntToArray(method));
    socket->waitForBytesWritten();
    socket->waitForReadyRead();
    QByteArray buffer = socket->readAll();
    qint32 response = ArrayToInt(buffer);
    qDebug()<<"Response from proxy => "<<response;
    if(response == 1)
    {
        qDebug()<<"Method supported";
        return true;
    }
    else
    {
        qDebug()<<"Method not supported";
        return false;
    }

}
bool MyTcpClient::connectToRemoteHost(QString host, qint32 port)
{
    socket->write(host.toLocal8Bit());
    socket->waitForBytesWritten();
    socket->waitForReadyRead();
    QByteArray buffer = socket->readAll();
    qint32 response = ArrayToInt(buffer);
    qDebug()<<"Response from proxy => "<<response;
    if(response == 1)
    {
        qDebug()<<"Address sent";
        socket->write(IntToArray(port));
        socket->waitForBytesWritten();
        socket->waitForReadyRead();
        buffer = socket->readAll();
        response = ArrayToInt(buffer);
        qDebug()<<"Response from proxy => "<<response;
        if(response == 1)
        {
            qDebug()<<"connected to remote host";
            return true;
        }
        else
        {
            qDebug()<<"could not connect to remote host";
            return false;
        }

    }
    else
    {
        qDebug()<<"could not send remote host address";
        return false;
    }

}

void MyTcpClient::sendUsername(QString username, QString password)
{
    connect(this, SIGNAL(usernameCorrect(QString)), this, SLOT(sendPassword(QString)));
    socket->write(username.toLocal8Bit());
    socket->waitForBytesWritten();
    socket->waitForReadyRead();
    QByteArray buffer = socket->readAll();
    qint32 response = ArrayToInt(buffer);
    qDebug()<<"Response from proxy => "<<response;
    if(response == 1)
    {
        qDebug()<<"Username ok";
        emit usernameCorrect(password);
    }
    else
    {
        qDebug()<<"Bad username";
        authenticated = false;
    }
}


void MyTcpClient::sendPassword(QString password)
{
    socket->write(password.toLocal8Bit());
    socket->waitForBytesWritten();
    socket->waitForReadyRead();
    QByteArray buffer = socket->readAll();
    qint32 response = ArrayToInt(buffer);
    qDebug()<<"Response from proxy => "<<response;
    if(response == 1)
    {
        qDebug()<<"Password ok";
        authenticated = true;
    }
    else
    {
        qDebug()<<"Bad password";
        authenticated = false;
    }
}


bool MyTcpClient::writeData(QByteArray data)
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
    QByteArray temp;
    QDataStream data(&temp, QIODevice::ReadWrite);
    data << source;
    return temp;
}

QByteArray StrToArray(QString source) //Use qint32 to ensure that the number have 4 bytes
{
    QByteArray temp;
    QDataStream data(&temp, QIODevice::ReadWrite);
    data << source;
    return temp;
}

qint32 ArrayToInt(QByteArray source)
{
    qint32 temp;
    QDataStream data(&source, QIODevice::ReadWrite);
    data >> temp;
    return temp;
}
