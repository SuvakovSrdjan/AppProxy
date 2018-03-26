#include <QCoreApplication>
#include <proxyserver.h>
#include <proxyclient.h>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    ProxyServer server;
    ProxyClient client;
    return a.exec();
}
