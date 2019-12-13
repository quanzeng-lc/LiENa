#ifndef LIENAHEARTBEATTASK_H
#define LIENAHEARTBEATTASK_H

#include "lienaOutputQueue.h"
#include "lienaHeartBeatMessage.h"
#include "lienaInputQueue.h"
#include "lienaDatagramEncoder.h"
#include "QThread"
#include "QDebug"
#include "lienaHeartBeatMessage.h"


class lienaHeartBeatTask: public QThread{
    Q_OBJECT

private:
    bool flag;
    bool standby;
    lienaOutputQueue* outputQueue;
    lienaGlobal *globalParameter;

public:
    void launch();
    void stop();
    void run();
    void enable();
    void freeze();
    lienaDatagram* transformHeartbeatMessageToIgtDatagram(lienaHeartBeatMessage* heartBeatMessage);
    void sendHeartBeatMessage();

public:
    explicit lienaHeartBeatTask(lienaOutputQueue* outputQueue, lienaGlobal *globalParameter);
};

#endif // LIENAHEARTBEATTASK_H
