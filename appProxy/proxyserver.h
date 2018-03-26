#ifndef PROXYSERVER_H
#define PROXYSERVER_H
#include <QtCore/QCoreApplication>
#include <QtNetwork/QTcpServer>
#include <QtNetwork/QTcpSocket>
#include <QtCore/QObject>
#include <QtCore/QDataStream>
#include <QtCore/QFile>
#include "proxyclient.h"

#define USERNAME "Srdjan"
#define PASSWORD "123"


class ProxyServer: public QObject{
    Q_OBJECT
public:
    explicit ProxyServer(QObject *parent = 0);
    void authentication();
    void connectToRemoteHost();
    void authMethodConfirmation();
    QByteArray dataRec;

signals:
    void dataReceived();

public slots:
    void newConnection();
    void disconnected();
    void readyRead();
private:
    bool authenticationDone;
    QTcpServer *server;
    ProxyClient client;
    QHash<QTcpSocket*, QByteArray*> buffers; //We need a buffer to store data until block has completely received
    QHash<QTcpSocket*, qint32*> sizes; //We need to store the size to verify if a block has received completely
};


#endif // PROXYSERVER_H
