#ifndef IGTDECODINGTASK_H
#define IGTDECODINGTASK_H

#include <QThread>
#include "lienaInputQueue.h"
#include "lienaAdvancementVelocityIs.h"
#include "lienaDatagramDecoder.h"
#include "lienaMessageQueue.h"
#include "lienaHeartBeatMessage.h"


/**
 * @brief The igtDecodingTask class
 */
class lienaDecodingTask : public QThread{
    Q_OBJECT

public:
    void launch();
    void run();
    void enable();
    void freeze();
    void stop();
    void setPeriod(uint32_t rtPeriod);
    void setInputMessageQueue(lienaMessageQueue* inputMessageQueue);

private:
    bool rtFlag;
    bool standby;
    uint32_t rtPeriod;
    lienaInputQueue *inputQueue;
    lienaDatagramDecoder *decoder;

    bool motivate;
    uint32_t localDeviceid;
    uint32_t targetDeviceId;

signals:
    void handShakeMessageArrived(lienaHandShakeMessage *handShakeMessage);
    void disengagementMessageArrived(lienaDisengagementMessage *disengagementMessage);
    void connexionConfirm();
    void channelOpenMsg(lienaChannelOpenedMessage* Msg);
    void handShakeCommitMessageArrived();
    void channelClosedMessageArrived();
    void disengagementCommitMessageArrived(lienaDisengagementCommitMessage* disengagementCommitMessage);
    void heartBeatMessageArrived(lienaHeartBeatMessage* msg);
    void rehandshakeMsgArrived(lienaReHandshakeMessage* msg);
    void rehandshakeCommitMsgArrived(lienaReHandshakeCommitMessage* msg);
    void reOpenedMsgArrived(lienaChannelReOpened* msg);
    void networkPassiveQualityMessageArrived(lienaNetworkQualityMessage* msg);
    void networkMotivateQualityMessageArrived(lienaNetworkQualityMessage* msg);

public slots:
    void notiftHandShakeMessage(lienaHandShakeMessage *handShakeMessage);
    void notiftDisengagementMessage(lienaDisengagementMessage *disengagementMessage);
    void connectionEstablish();
    void notiftHeartBeatMessage(lienaChannelOpenedMessage* msg);
    void channelOpen();
    void notifyChannelClosedMessage();
    void notiftDisengagementCommitMessage(lienaDisengagementCommitMessage* msg);
    void notifyHeartBeatMessage(lienaHeartBeatMessage* msg);
    void notifyReHandshakeMsg(lienaReHandshakeMessage* msg);
    void notifyReHandshakeCommit(lienaReHandshakeCommitMessage* msg);
    void notifyChannelReOpened(lienaChannelReOpened* msg);
    void notifyPassiveNetworkQualityMessage(lienaNetworkQualityMessage* msg);
    void notifyMotivateNetworkQualityMessage(lienaNetworkQualityMessage* msg);

public:
    explicit lienaDecodingTask(lienaInputQueue *inputQueue,
                               bool motivate,
                               uint32_t localDeviceid,
                               uint32_t targetDeviceId);
    ~lienaDecodingTask();

};

#endif // IGTDECODINGTASK_H
