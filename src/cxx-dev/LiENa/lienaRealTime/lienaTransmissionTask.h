#ifndef LIENATRANSMISSIONTASK_H
#define LIENATRANSMISSIONTASK_H



#include <QtGlobal>

#ifdef Q_OS_MACOS
    #include <netinet/in.h>
    #include <sys/socket.h>
    #include <arpa/inet.h>
#else
    #include <WinSock2.h>
    #include <Windows.h>
    #include <ws2tcpip.h>
#endif

#include <iostream>
#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <QThread>
#include <QString>
#include <QStringList>
#include <QTimer>
#include <QDebug>
#include <QByteArray>

#include <QTimer>
#include <QThread>

#include "lienaHandShakeMessage.h"
#include "lienaOutputQueue.h"
#include "lienaHandShakeCommitMessage.h"
#include "lienaDatagramEncoder.h"



class lienaTransmissionTask:public QThread{

    Q_OBJECT

public:
    QString ipDetect();
    bool getConnectionState();
    void run();
    void enable();
    void freeze();
    void stop();
    void launch();
    void transmit();
    uint64_t getLatestMessageID();
    void update_socket_descriptor(SOCKET socket);

private:
    int index;
    bool motivate;
    bool standby;
    bool flag;
    bool connectionStatus;

    QString localIP;
    SOCKET connectSocket;
    lienaOutputQueue *outputQueue;
    lienaGlobal *globalParameter;
    lienaDatagramEncoder* encoder;

    int mode;
    unsigned int rtPeriod;
    unsigned int compteur;
    uint64_t lastMessageId;

signals:
    void connectionEstablished();

public:
    lienaTransmissionTask(SOCKET connectSocket, int index, lienaGlobal *globalParameter, lienaOutputQueue *outputQueue);
    ~lienaTransmissionTask();
};

#endif // LIENATRANSMISSIONTASK_H
