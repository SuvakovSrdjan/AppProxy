#include <QCoreApplication>
#include "mytcpclient.h"

#define FILENAME "../File.txt"
#define BAD_SERVER_ADDRESS "1111.1111.1111.1111"
#define SERVER_ADDRESS "127.0.0.1"
#define REMOTE_SERVER_ADDRESS "127.0.0.1"
#define REMOTE_SERVER_PORT 9021

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    MyTcpClient client;
    QFile file(FILENAME);
    if(!file.exists())
    {
        qDebug()<<"File "<<FILENAME<< " does not exist!";
    }
    if(!file.open(QIODevice::ReadOnly)){
        qDebug()<<"File "<<FILENAME << " can't be opened";
    }

    QByteArray data = file.readAll();

    if(data.size() != 0){
        if(client.connectToHost(SERVER_ADDRESS))
        {
            if(client.authMedtodRequest(2))
            {
                if(client.authentication("Srdjan","123"))
                {
                    if(client.connectToRemoteHost(REMOTE_SERVER_ADDRESS, REMOTE_SERVER_PORT))
                    {
                        client.writeData(data);
                        qDebug()<<"data sent";

                    }
                    else
                    {
                        qDebug()<<"Failure to connect to remote host";
                    }

                }
                else
                    qDebug()<<"Failure to authenticate";
            }
            else
                qDebug()<<"Authentication method unsuported";
        }
        else
            qDebug()<<"Failure to connect";

    }
    else
        qDebug()<<"There is no data to send!";


    file.close();
    exit(1);
    return a.exec();
}
