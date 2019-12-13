#ifndef IGTENCODINGTASK_H
#define IGTENCODINGTASK_H

#include <QThread>
#include "lienaDatagram.h"
#include "lienaDatagramEncoder.h"
#include "lienaOutputQueue.h"
#include "lienaMessageQueue.h"
#include "lienaChannelOpenedMessage.h"
#include "lienaDisengagementCommitMessage.h"
#include "lienaDisengagementMessage.h"
#include "lienaChannelClosedMessage.h"
#include "lienaRenHandshakeMessage.h"
#include "lienaReHandshakeCommitMessage.h"
#include "lienaChannelReOpened.h"
#include "time.h"
#include "math.h"


class lienaEncodingTask : public QThread
{
    Q_OBJECT

private:
    bool flag;
    bool standby;
    uint32_t rtPeriod;
    lienaMessageQueue* msgQ;
    lienaDatagramEncoder *encoder;
    lienaOutputQueue *outputQueue;
    lienaGlobal *globalParameter;
    bool motivate;
    uint32_t targetDeviceId;
    uint32_t originId;

public:
    void setPeriod(uint32_t rtPeriod);
    void setOutputMessageQueue(lienaMessageQueue* msgQ);
    void run();
    void enable();
    void freeze();
    void stop();
    void launch();
    void sendHandShakeCommitMessage();
    void sendDisengagementCommitMessage();
    void sendChannelOpenedMessage();
    void sendHandShakeMessage(QString addr);
    void sendDisengagementMessage();
    void sendChannelClosedMessage();
    void sendReHandshakeMessage();
    void sendReHandshakeCommitMessage();
    void sendChannelRepairedMessage();

public:
    lienaEncodingTask(lienaOutputQueue *outputQueue, lienaGlobal *globalParameter, bool motivate, uint32_t targetDeviceId);
    ~lienaEncodingTask();
};
#endif // IGTENCODINGTASK_H
