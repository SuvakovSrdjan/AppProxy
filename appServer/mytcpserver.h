#ifndef MYTCPSERVER_H
#define MYTCPSERVER_H

#include <QtCore/QCoreApplication>
#include <QtNetwork/QTcpServer>
#include <QtNetwork/QTcpSocket>
#include <QtCore/QObject>
#include <QtCore/QDataStream>
#include <QtCore/QFile>


class MyTcpServer: public QObject{
    Q_OBJECT
public:
    explicit MyTcpServer(QObject *parent = 0);

signals:
    void dataReceived(QByteArray);

public slots:
    void newConnection();
    void disconnected();
    void readyRead();

private:
    QTcpServer *server;
    QHash<QTcpSocket*, QByteArray*> buffers; //We need a buffer to store data until block has completely received
    QHash<QTcpSocket*, qint32*> sizes; //We need to store the size to verify if a block has received completely
};

#endif //MYTCPSERVER_H
