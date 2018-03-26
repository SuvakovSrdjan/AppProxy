#ifndef PROXYCLIENT_H
#define PROXYCLIENT_H

#ifndef MYTCPCLIENT_H
#define MYTCPCLIENT_H

#include <QtCore>
#include <QtNetwork>

class ProxyClient : public QObject
{
    Q_OBJECT
public:
    explicit ProxyClient(QObject *parent = 0);
     QTcpSocket *socket;
public slots:
    bool connectToHost(QString host, qint16 port);
    bool writeData(QByteArray data);

private:

};
#endif // MYTCPCLIENT_H

#endif // PROXYCLIENT_H
