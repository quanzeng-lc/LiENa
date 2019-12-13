#ifndef IGTSERVER_H
#define IGTSERVER_H

#include <QThread>
#include <QString>
#include <QByteArray>
#include <QDebug>

#include "lienaGlobal.h"


#ifdef Q_OS_MACOS
    #include <stdio.h>
    #include <netinet/in.h>
    #include <sys/socket.h>
    #include <arpa/inet.h>
#else
    #include <WinSock2.h>
    #include <Windows.h>
    #include <ws2tcpip.h>
    #include <iostream>
    #include <stdio.h>
    #include <assert.h>
#endif


class lienaTcpServer: public QThread
{
    Q_OBJECT

public:
    bool init();
    bool launchServer();
    bool stopServer();
    bool getStatus();
    QString ipDetect();

protected:
    void run();

private:
    SOCKET socketServer;
    lienaGlobal *globalParameter;

    bool isConnected;
    bool flag;

    int incomingClientCount;

signals:
    void localIPDetect(QString locaIP);
    void clientArrived(SOCKET sClient, QString addr);

public:
    lienaTcpServer(lienaGlobal *globalParameter);
    ~lienaTcpServer();
};

#endif // IGTSERVER_H
