#ifndef IGTDATAGRAMANALYSER_H
#define IGTDATAGRAMANALYSER_H

#include <QByteArray>
#include <QDebug>
#include "lienaDefinition.h"
#include "lienaInputMessageCache.h"
#include "lienaDatagram.h"
#include "lienaHelloMessage.h"
#include "lienaHandShakeMessage.h"
#include "lienaHandShakeCommitMessage.h"
#include "lienaHeartBeatMessage.h"
#include "lienaChannelOpenedMessage.h"
#include "lienaDisengagementMessage.h"
#include "lienaChannelClosedMessage.h"
#include "lienaDisengagementCommitMessage.h"
#include "lienaHeartBeatMessage.h"
#include "lienaReHandshakeCommitMessage.h"
#include "lienaRenHandshakeMessage.h"
#include "lienaChannelReOpened.h"
#include "lienaNetworkQualityMessage.h"


/**
 *
 *  @brief The igtDatagramAnalyser class
 *
 */
class lienaDatagramDecoder : public QObject{

    Q_OBJECT

private:
    int handshakeInstructionCount;
    lienaInputMessageCache* inputMessageCache;
    lienaChannelOpenedMessage * channelOpenedMsg;
    bool motivate;
    unsigned int localDeviceId;
    unsigned int targetId;

public:
    void setInputCache(lienaInputMessageCache* inputMessageCache);

    void analyse(lienaDatagram *datagram);
    void decodeHelloMsg(lienaDatagram *datagram);
    void convertLienaDatagramToHandShakeMessage(lienaDatagram *datagram);
    void convertLienaDatagramToHandShakeCommitMessage(lienaDatagram *datagram);
    void convertLienaDatagramToChannelOpenedMessage(lienaDatagram *datagram);
    void convertLienaDatagramToDisengamentMessage(lienaDatagram *datagram);
    void convertLienaDatagramToDisengamentCommitMessage(lienaDatagram *datagram);
    void convertLienaDatagramToChannelClosedMessage(lienaDatagram *datagram);
    void convertLienaDatagramToHeartBeatMessage(lienaDatagram *datagram);

    void decodeHandShakeCommitMsg(lienaDatagram *datagram);
    void decodeForceTorqueValues(lienaDatagram *datagram);
    void decodeNDIPosition(lienaDatagram *datagram);
    void decodeGuidewireMovingDistance(lienaDatagram *datagram);
    void decodeVelocityIsMsg(lienaDatagram *datagram);
    void convertLienaDatagramToReHandshakeMessage(lienaDatagram *datagram);
    void convertLienaDatagramToReHandshakeCommitMessage(lienaDatagram *datagram);
    void convertLienaDatagramToChannelReopenedMessage(lienaDatagram *datagram);
    void convertLienaDatagramToNetworkQualityMessage(lienaDatagram* datagram);

signals:
    void connectionConfirm();
    void handShakeMessageArrived(lienaHandShakeMessage *handShakeMessage);
    void disengagementMessageArrived(lienaDisengagementMessage *disengagementMessage);
    void channelOpenMsg(lienaChannelOpenedMessage* heartBeatMessage);
    void handShakeCommitMessageArrived();
    void channelClosedMessageArrived(lienaChannelClosedMessage * channelClosedMsg);
    void disengagementCommitMessageArrived(lienaDisengagementCommitMessage*disengagementCommitMessage);
    void heartBeatMessageArrived(lienaHeartBeatMessage* heartBeatMessage);
    void channelReOpenedMessageArrived(lienaChannelReOpened* msg);

    void reHandshakeCommitMessageArrived(lienaReHandshakeCommitMessage* msg);
    void reHandshakeMessageArrived(lienaReHandshakeMessage* msg);

    void motivateNTPClockSynchronizationMessageArrived(lienaNetworkQualityMessage* msg);
    void passiveNTPClockSynchronizationMessageArrived(lienaNetworkQualityMessage* msg);

public:
    lienaDatagramDecoder(bool motivate, unsigned int localDeviceId, unsigned int targetId);
};

#endif // IGTDATAGRAMANALYSER_H
