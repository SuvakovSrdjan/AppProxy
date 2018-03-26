#ifndef MYTCPCLIENT_H
#define MYTCPCLIENT_H

#include <QtCore>
#include <QtNetwork>
#include <unistd.h>

class MyTcpClient : public QObject
{
    Q_OBJECT
public:
    explicit MyTcpClient(QObject *parent = 0);
    bool isAuthenticated();
    void sendUsername(QString username, QString password);
    bool authentication(QString username, QString password);
    bool authMedtodRequest(qint32 method);
    bool connectToRemoteHost(QString host, qint32 port);
signals:
    void usernameCorrect(QString);

public slots:
    bool connectToHost(QString host);
    bool writeData(QByteArray data);
    void sendPassword( QString password);
    //void disconnect();
private:
    QTcpSocket *socket;
    bool authenticated;
};
#endif // MYTCPCLIENT_H
