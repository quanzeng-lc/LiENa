#ifndef IGTRECEPTIONTSAK_H
#define IGTRECEPTIONTSAK_H

#include <QThread>
#include <QDateTime>
#include <QDebug>

#include "lienaInputQueue.h"
#include "lienaGlobal.h"

#ifdef Q_OS_MACOS
    #include <stdio.h>
    #include <netinet/in.h>
    #include <sys/socket.h>
    #include <arpa/inet.h>
#else
    #pragma comment(lib,"WS2_32")
    #include <WinSock2.h>
    #include "stdio.h"
#endif


/**
 * @brief The igtReceptionTask class
 *
 *
 */
class lienaReceptionTask:public QThread
{
    Q_OBJECT

public:
    void init();
    void run();
    void freeze();
    void enable();
    void launch();
    void stop();
    void changePeriod(unsigned int period);
    void setGlobalDLC(unsigned int dlc);
    void updateSocketDescripter(SOCKET recSoc);
    bool getTaskStatus();
    int getIndex();

private:
    bool rtFlag;
    bool standby;
    unsigned int compteur;
    unsigned int rtPeriod;
    unsigned int dlc;
    long received_frame_count;
    int socketDescriptor;
    int index;
    SOCKET recSoc;
    lienaInputQueue *inputQueue;
    lienaGlobal *globalParameter;
    unsigned int  globalDatagramSize;

    QDateTime start_time;
    QVector<QDateTime> stop_time_sequence;

public:
    lienaReceptionTask(int index, lienaGlobal *globalParameter, SOCKET recSoc,lienaInputQueue* inputQueue);
};

#endif // IGTRECEPTIONTSAK_H
