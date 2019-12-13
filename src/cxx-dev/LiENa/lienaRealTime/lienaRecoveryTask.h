#ifndef LIENARECOVERYTASK_H
#define LIENARECOVERYTASK_H

#include "QObject"
#include "QThread"
#include "QDebug"
#include "lienaTcpClient.h"

#ifdef Q_OS_MACOS
    #include <stdio.h>
    #include <netinet/in.h>
    #include <sys/socket.h>
    #include <arpa/inet.h>
#else
    #include <WinSock2.h>
#endif


class lienaRecoveryTask : public QThread{
    Q_OBJECT

public:
    void run();
    void launch();

private:
    bool flag;
    int timeCount;
    QString addr;
    unsigned short port;

signals:
    void reconnectSuccess(SOCKET socket);

public:
    lienaRecoveryTask(QString addr, unsigned short port);
};

#endif // LIENARECOVERYTASK_H
