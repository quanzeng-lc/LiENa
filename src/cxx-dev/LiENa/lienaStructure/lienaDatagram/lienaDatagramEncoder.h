#ifndef IGTDATAGRAMENCODER_H
#define IGTDATAGRAMENCODER_H

#include "lienaDatagram.h"
#include <QString>
#include <QStringList>
#include <QDebug>


//! session manaagement
#include "lienaHelloMessage.h"
#include "lienaHandShakeMessage.h"
#include "lienaHandShakeCommitMessage.h"
#include "lienaChannelOpenedMessage.h"
#include "lienaDisengagementCommitMessage.h"
#include "lienaDisengagementMessage.h"
#include "lienaChannelClosedMessage.h"
#include "lienaRenHandshakeMessage.h"
#include "lienaReHandshakeCommitMessage.h"
#include "lienaChannelReOpened.h"
#include "lienaCustomizedMessage.h"

//! robotic system
#include "lienaCloseSessionMessage.h"


class lienaDatagramEncoder
{
public:
    lienaDatagram* transformHelloMessageToIgtDatagram(lienaHelloMessage* helloMessage);
    lienaDatagram* encode_handshake_message(lienaHandShakeMessage* handshakeMessage);
    lienaDatagram* encode_customized_message(lienaCustomizedMessage* customizedMessage);
    lienaDatagram* transformHandShakeCommitMessageToIgtDatagram(lienaHandShakeCommitMessage* handshakeCommitMessage);
    lienaDatagram* transformCloseSessionMessageToIgtDatagram(lienaCloseSessionMessage * injectionCommand);
    lienaDatagram* transformChannelOpenedMessageToIgtDatagram(lienaChannelOpenedMessage* openChannelMsg);
    lienaDatagram* transformDisengagementCommitMessageToIgtDatagram(lienaDisengagementCommitMessage* disengagementCommitMessage);
    lienaDatagram* transformDisengagementMessageToIgtDatagram(lienaDisengagementMessage* disengagementMessage);
    lienaDatagram* transformchannelClosedMessageToIgtDatagram(lienaChannelClosedMessage* channelClosedMessage);

    lienaDatagram* transformReHandShakeMessageToIgtDatagram(lienaReHandshakeMessage* msg);
    lienaDatagram* transformChannelReOpenedMessageToIgtDatagram(lienaChannelReOpened* msg);
    lienaDatagram* transformReHandShakeCommitMessageToIgtDatagram(lienaReHandshakeCommitMessage* msg);

private:
    lienaGlobal *globalParameter;
    unsigned long long  messageId;
    unsigned int  messageType;

public:
    lienaDatagramEncoder(lienaGlobal *globalParameter);
};

#endif // IGTDATAGRAMENCODER_H
