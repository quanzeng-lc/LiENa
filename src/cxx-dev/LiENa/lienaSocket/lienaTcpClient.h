#ifndef LIENATCPCLIENT_H
#define LIENATCPCLIENT_H


#ifdef Q_OS_MACOS
    #include <netinet/in.h>
    #include <sys/socket.h>
    #include <arpa/inet.h>
#else
#define _WINSOCK_DEPRECATED_NO_WARNINGS
    #include <WinSock2.h>
    #include <Windows.h>
    #include <ws2tcpip.h>
#endif

#include <QString>
#include <QByteArray>
#include <QDebug>


class lienaTcpClient
{
public:
    int connectera(int timeout);
    QString getLocalIp();
    SOCKET getSocketCom();

private:
    SOCKET connectSocket;
    SOCKET soc;
    QString addr;
    unsigned short port;

public:
    lienaTcpClient(QString addr, unsigned short port);
    ~lienaTcpClient();
};

#endif // LIENATCPCLIENT_H
