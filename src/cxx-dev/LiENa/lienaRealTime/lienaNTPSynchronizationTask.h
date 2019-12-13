#ifndef LIENANTPSYNCHRONIZATIONTASK_H
#define LIENANTPSYNCHRONIZATIONTASK_H
#include "lienaOutputQueue.h"
#include "lienaNetworkQualityMessage.h"
#include "lienaDatagram.h"

#include <QThread>
#include "QMutex"
#include "QDebug"
#include <time.h>
#include "iostream"
#include "sys/timeb.h"


class lienaNTPSynchronizationTask : public QThread{
    Q_OBJECT

private:
    bool flag;
    bool standby;

    unsigned long rtPeriod;
    int loopNumber;

    QVector<lienaNetworkQualityMessage*> msgReturned;

    lienaOutputQueue* outputQueue;
    lienaGlobal *globalParameter;
    unsigned int targetDeviceId;
    unsigned int originId;
    lienaNetworkQualityMessage* networkQualityMessage;

public:
    void launch();
    void stop();
    void run();
    void enable();
    void freeze();
    void terminate();
    void setLoopNumber(int loopNumber);
    void sendNetworkQualityMessage(int count);
    void sendBackNetworkQualityMessage(lienaNetworkQualityMessage* networkQualityMessage);
    lienaDatagram* encodeNetworkQualityMessage(lienaNetworkQualityMessage* networkQualityMessage);
    void receive(lienaNetworkQualityMessage* msg);


public:
    lienaNTPSynchronizationTask(lienaOutputQueue* outputQueue, lienaGlobal *globalParameter, unsigned int targetDeviceId);
};

#endif // LIENANTPSYNCHRONIZATIONTASK_H
