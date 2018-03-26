// mytcpserver.cpp

#include "proxyserver.h"

static inline qint32 ArrayToInt(QByteArray source);
static inline QByteArray IntToArray(qint32 source);

ProxyServer::ProxyServer(QObject *parent) : QObject(parent)
{
    server = new QTcpServer(this);
    authenticationDone = false;

    // whenever a user connects, it will emit signal
    connect(server, SIGNAL(newConnection()), this, SLOT(newConnection()));
    if(!server->listen(QHostAddress::Any, 9001))
    {
        qDebug() << "Proxy server could not start";
    }
    else
    {
        qDebug() << "Proxy server started!";
        qDebug() << "Listening:";
    }

}

void ProxyServer::authentication()
{
    qDebug()<<"=========================";
    qDebug()<<"Authentication Started!";
    QTcpSocket *socket = static_cast<QTcpSocket*>(sender());
    QString username;
    QString password;
    socket->waitForReadyRead();
    QByteArray data = socket->readAll();
    username.append(data);
    qDebug()<<"Username for verification "<<username;
    if(!username.compare(USERNAME, Qt::CaseSensitive))
    {
        qDebug()<<"Username correct";
        socket->write(IntToArray(1));
        socket->waitForBytesWritten();
        socket->waitForReadyRead();

        data = socket->readAll();
        password.append(data);
        qDebug()<<"Password for verification "<<password;
        if(!password.compare(PASSWORD, Qt::CaseSensitive))
        {
            qDebug()<<"Password correct";
            socket->write(IntToArray(1));
        }
        else
        {
            qDebug()<<"Password incorrect";
            socket->write(IntToArray(0));
        }

    }
    else
    {
        qDebug()<<"Username incorrect";
        socket->write(IntToArray(0));
    }

}

void ProxyServer::newConnection()
{
    while (server->hasPendingConnections())
    {
        qDebug()<<"new Connection accepted!";
        QTcpSocket *socket = server->nextPendingConnection();
        connect(socket, SIGNAL(readyRead()), SLOT(readyRead()));
        connect(socket, SIGNAL(disconnected()), SLOT(disconnected()));
        QByteArray *buffer = new QByteArray();
        qint32 *s = new qint32(0);
        buffers.insert(socket, buffer);
        sizes.insert(socket, s);
        authenticationDone = false;
    }
}

void ProxyServer::connectToRemoteHost()
{
    QTcpSocket *socket = static_cast<QTcpSocket*>(sender());
    QString host;
    qint16 port;
    socket->waitForReadyRead();
    QByteArray data = socket->readAll();
    host.append(data);
    qDebug()<<"Host ip recieved"<<host;
    socket->write(IntToArray(1));
    socket->waitForBytesWritten();
    socket->waitForReadyRead();

    data = socket->readAll();
    port = ArrayToInt(data);

    if(client.connectToHost(host,port))
    {
        socket->write(IntToArray(1));
        authenticationDone = true;
    }
    else
    {
        socket->write(IntToArray(0));
    }

}

void ProxyServer::authMethodConfirmation()
{
    QTcpSocket *socket = static_cast<QTcpSocket*>(sender());
    qint32 method;
    //socket->waitForReadyRead();
    QByteArray data = socket->readAll();
    method = ArrayToInt(data);
     qDebug()<<"Authentication method recieved"<<method;
    if(method == 2)
    {
        qDebug()<<"Method supported";
        socket->write(IntToArray(1));
    }
    else
    {
        qDebug()<<"Method not supported";
        socket->write(IntToArray(0));
    }

}


void ProxyServer::disconnected()
{
    qDebug()<<"disconnecting";
    QTcpSocket *socket = static_cast<QTcpSocket*>(sender());
    QByteArray *buffer = buffers.value(socket);
    qint32 *s = sizes.value(socket);
    client.socket->close();
    socket->deleteLater();
    delete buffer;
    delete s;
}

void ProxyServer::readyRead()
{

    if(!authenticationDone)
    {
        this->authMethodConfirmation();
        this->authentication();
        qDebug()<<"Authentication done";
        qDebug()<<"===================";
        this->connectToRemoteHost();
    }
    qDebug()<<"recieving data";
    QTcpSocket *socket = static_cast<QTcpSocket*>(sender());
    QByteArray *buffer = buffers.value(socket);
    qint32 *s = sizes.value(socket);
    qint32 size = *s;
    QFile file("Test.txt");
    file.open(QIODevice::WriteOnly);
    while (socket->bytesAvailable() > 0)
    {
        buffer->append(socket->readAll());
        while ((size == 0 && buffer->size() >= 4) || (size > 0 && buffer->size() >= size)) //While can process data, process it
        {
            if (size == 0 && buffer->size() >= 4) //if size of data has received completely, then store it on our global variable
            {
                size = ArrayToInt(buffer->mid(0, 4));
                *s = size;
                buffer->remove(0, 4);
            }
            if (size > 0 && buffer->size() >= size) // If data has received completely, then emit our SIGNAL
            {
                QByteArray data = buffer->mid(0, size);
                buffer->remove(0, size);
                size = 0;
                *s = size;
                qDebug()<<"data recieved!";
                client.writeData(data);
                file.write(data);
                file.close();
                emit dataReceived();
            }
        }
    }
}

QByteArray IntToArray(qint32 source) //Use qint32 to ensure that the number have 4 bytes
{
    //Avoid use of cast, this is the Qt way to serialize objects
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
