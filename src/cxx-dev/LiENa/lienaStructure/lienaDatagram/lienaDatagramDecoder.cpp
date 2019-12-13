#include "lienaDatagramDecoder.h"


/**
 * @brief igtDatagramAnalyser::igtDatagramAnalyser
 * @param dataware
 */
lienaDatagramDecoder::lienaDatagramDecoder(bool motivate, unsigned int localDeviceId, unsigned int targetId){
    this->handshakeInstructionCount = 0;
    this->motivate = motivate;
    this->localDeviceId = localDeviceId;
    this->targetId = targetId;
    this->inputMessageCache = new lienaInputMessageCache();
}

//! -------------------------------------------------------------------------------------------------------------------
//!
//! \brief igtDatagramAnalyser::analyse
//! \param datagram
//!
void lienaDatagramDecoder::analyse(lienaDatagram *datagram){
    uint64_t customrizedMessageId = datagram->getMessageID();
    qDebug()<<"customrizedMessageId:"<<customrizedMessageId;
    if(customrizedMessageId == LIENA_SESSION_MANAGEMENT_HANDSHAKE_MESSAGE){
        this->convertLienaDatagramToHandShakeMessage(datagram);
    }
    else if(customrizedMessageId == LIENA_SESSION_MANAGEMENT_HANDSHAKECOMMIT_MESSAGE){
        this->convertLienaDatagramToHandShakeCommitMessage(datagram);
    }
    else if(customrizedMessageId == LIENA_SESSION_MANAGEMENT_CHANNELOPENNED_MESSAGE){
        this->convertLienaDatagramToChannelOpenedMessage(datagram);
    }
    else if(customrizedMessageId == LIENA_SESSION_MANAGEMENT_DISENGAGEMENT_MESSAGE){
        this->convertLienaDatagramToDisengamentMessage(datagram);
    }
    else if(customrizedMessageId == LIENA_SESSION_MANAGEMENT_DISENGAGEMENTCOMMIT_MESSAGE){
        this->convertLienaDatagramToDisengamentCommitMessage(datagram);
    }
    else if(customrizedMessageId == LIENA_SESSION_MANAGEMENT_HEARTBEAT_MESSAGE){
        this->convertLienaDatagramToHeartBeatMessage(datagram);
    }
    else if(customrizedMessageId == LIENA_SESSION_MANAGEMENT_CHANNELCLOSED_MESSAGE){
        this->convertLienaDatagramToChannelClosedMessage(datagram);
    }
    else if(customrizedMessageId == LIENA_SESSION_MANAGEMENT_REHANDSHAKE_MESSAGE){
        this->convertLienaDatagramToReHandshakeMessage(datagram);
    }
    else if(customrizedMessageId == LIENA_SESSION_MANAGEMENT_REHANDSHAKECOMMIT_MESSAGE){
        this->convertLienaDatagramToReHandshakeCommitMessage(datagram);
    }
    else if(customrizedMessageId == LIENA_SESSION_MANAGEMENT_REOPENED_MESSAGE){
        this->convertLienaDatagramToChannelReopenedMessage(datagram);
    }
    else if(customrizedMessageId == LIENA_SESSION_MANAGMENT_NTP_SYNCHRONIZATION_MESSAGE){
        this->convertLienaDatagramToNetworkQualityMessage(datagram);
    }
}

//!
//! \brief lienaDatagramDecoder::convertLienaDatagramToNetworkQualityMessage
//! \param datagram
//!
void lienaDatagramDecoder::convertLienaDatagramToNetworkQualityMessage(lienaDatagram *datagram){
    char *body = datagram->getBody();
    if(DEBUG){
        qDebug()<<"lienaDatagramDecoder::convertLienaDatagramToNetworkQualityMessage";
    }

    if(datagram->getMessageID() == LIENA_SESSION_MANAGMENT_NTP_SYNCHRONIZATION_MESSAGE){
        qDebug()<<datagram->getOriginId()<<this->localDeviceId;
        if(datagram->getOriginId() == this->localDeviceId){
            int index = int(body[0]);

            uint64_t t1 = uint64_t(uchar(body[1])*pow(256, 7)) +
                          uint64_t(uchar(body[2])*pow(256, 6)) +
                          uint64_t(uchar(body[3])*pow(256, 5)) +
                          uint64_t(uchar(body[4])*pow(256, 4)) +
                          uint64_t(uchar(body[5])*pow(256, 3)) +
                          uint64_t(uchar(body[6])*pow(256, 2)) +
                          uint64_t(uchar(body[7])*pow(256, 1)) +
                          uint64_t(uchar(body[8])*pow(256, 0));

            uint64_t t2 = uint64_t(uchar(body[9])*pow(256, 7)) +
                          uint64_t(uchar(body[10])*pow(256, 6)) +
                          uint64_t(uchar(body[11])*pow(256, 5)) +
                          uint64_t(uchar(body[12])*pow(256, 4)) +
                          uint64_t(uchar(body[13])*pow(256, 3)) +
                          uint64_t(uchar(body[14])*pow(256, 2)) +
                          uint64_t(uchar(body[15])*pow(256, 1)) +
                          uint64_t(uchar(body[16])*pow(256, 0));

            uint64_t t3 = uint64_t(uchar(body[17])*pow(256, 7)) +
                          uint64_t(uchar(body[18])*pow(256, 6)) +
                          uint64_t(uchar(body[19])*pow(256, 5)) +
                          uint64_t(uchar(body[20])*pow(256, 4)) +
                          uint64_t(uchar(body[21])*pow(256, 3)) +
                          uint64_t(uchar(body[22])*pow(256, 2)) +
                          uint64_t(uchar(body[23])*pow(256, 1)) +
                          uint64_t(uchar(body[24])*pow(256, 0));

            uint64_t t4 = uint64_t(uchar(body[25])*pow(256, 7)) +
                          uint64_t(uchar(body[26])*pow(256, 6)) +
                          uint64_t(uchar(body[27])*pow(256, 5)) +
                          uint64_t(uchar(body[28])*pow(256, 4)) +
                          uint64_t(uchar(body[29])*pow(256, 3)) +
                          uint64_t(uchar(body[30])*pow(256, 2)) +
                          uint64_t(uchar(body[31])*pow(256, 1)) +
                          uint64_t(uchar(body[32])*pow(256, 0));

            lienaNetworkQualityMessage * networkQualityMessage = new lienaNetworkQualityMessage(datagram->getMessageID(), datagram->getTargetId(), datagram->getOriginId(), datagram->getTimeStampes(), datagram->getDLC());
            networkQualityMessage->set_index(index);
            networkQualityMessage->set_t1(t1);
            networkQualityMessage->set_t2(t2);
            networkQualityMessage->set_t3(t3);
            networkQualityMessage->set_t4(t4);
            emit motivateNTPClockSynchronizationMessageArrived(networkQualityMessage);
        }
        else{
            int index = int(body[0]);
            uint64_t t1 = uint64_t(uchar(body[1])*pow(256, 7)) +
                          uint64_t(uchar(body[2])*pow(256, 6)) +
                          uint64_t(uchar(body[3])*pow(256, 5)) +
                          uint64_t(uchar(body[4])*pow(256, 4)) +
                          uint64_t(uchar(body[5])*pow(256, 3)) +
                          uint64_t(uchar(body[6])*pow(256, 2)) +
                          uint64_t(uchar(body[7])*pow(256, 1)) +
                          uint64_t(uchar(body[8])*pow(256, 0));
            qDebug()<<"passive : "<<datagram->getMessageID();
            lienaNetworkQualityMessage * networkQualityMessage = new lienaNetworkQualityMessage(datagram->getMessageID(), datagram->getTargetId(),datagram->getOriginId(),  datagram->getTimeStampes(), datagram->getDLC());
            networkQualityMessage->set_index(index);
            networkQualityMessage->set_t1(t1);
            emit passiveNTPClockSynchronizationMessageArrived(networkQualityMessage);
        }
    }
}

//!
//! \brief lienaDatagramDecoder::convertLienaDatagramToReHandshakeMessage
//! \param datagram
//!
void lienaDatagramDecoder::convertLienaDatagramToReHandshakeMessage(lienaDatagram *datagram){
    if(DEBUG){
        qDebug()<<"lienaDatagramDecoder::convertLienaDatagramToReHandshakeMessage";
    }
    lienaReHandshakeMessage * reHandshakeMessage = new lienaReHandshakeMessage(datagram->getMessageID(), datagram->getTargetId(), datagram->getOriginId(), datagram->getTimeStampes(), datagram->getDLC());
    emit reHandshakeMessageArrived(reHandshakeMessage);
}

//! -------------------------------------------------------------------------------------------------------------------
//!
//! \brief lienaDatagramDecoder::convertLienaDatagramToReHandshakeCommitMessage
//! \param datagram
//!
void lienaDatagramDecoder::convertLienaDatagramToReHandshakeCommitMessage(lienaDatagram *datagram){
    if(DEBUG){
        qDebug()<<"lienaDatagramDecoder::convertLienaDatagramToReHandshakeCommitMessage";
    }

    lienaReHandshakeCommitMessage * reHandshakeCommitMessage = new lienaReHandshakeCommitMessage(datagram->getMessageID(), datagram->getTargetId(), datagram->getOriginId(), datagram->getTimeStampes(), datagram->getDLC());
    emit reHandshakeCommitMessageArrived(reHandshakeCommitMessage);
}

//! -------------------------------------------------------------------------------------------------------------------
//!
//! \brief lienaDatagramDecoder::convertLienaDatagramToChannelReopenedMessage
//! \param datagram
//!
void lienaDatagramDecoder::convertLienaDatagramToChannelReopenedMessage(lienaDatagram *datagram){
    if(DEBUG){
       qDebug()<<"lienaDatagramDecoder::convertLienaDatagramToChannelReopenedMessage";
    }

    lienaChannelReOpened * channelReOpenedMessage = new lienaChannelReOpened(datagram->getMessageID(), datagram->getTargetId(), datagram->getOriginId(), datagram->getTimeStampes(), datagram->getDLC());
    emit channelReOpenedMessageArrived(channelReOpenedMessage);
}

//! -------------------------------------------------------------------------------------------------------------------
//!
//! \brief lienaDatagramDecoder::convertLienaDatagramToDisengamentMessage
//! \param datagram
//!
void lienaDatagramDecoder::convertLienaDatagramToDisengamentMessage(lienaDatagram *datagram){
    if(DEBUG){
        qDebug()<<"lienaDatagramDecoder::convertLienaDatagramToDisengamentMessage";
    }
    lienaDisengagementMessage * disengagementMessage = new lienaDisengagementMessage(datagram->getMessageID(), datagram->getTargetId(),datagram->getOriginId(),  datagram->getTimeStampes(), datagram->getDLC());
    emit disengagementMessageArrived(disengagementMessage);
}

//! -------------------------------------------------------------------------------------------------------------------
//!
//! \brief lienaDatagramDecoder::convertLienaDatagramToDisengamentCommitMessage
//! \param datagram
//!
void lienaDatagramDecoder::convertLienaDatagramToDisengamentCommitMessage(lienaDatagram *datagram){
    if(DEBUG){
        qDebug()<<"lienaDatagramDecoder::convertLienaDatagramToDisengamentCommitMessage";
    }

    lienaDisengagementCommitMessage * disengagementCommitMessage = new lienaDisengagementCommitMessage(datagram->getMessageID(), datagram->getTargetId(), datagram->getOriginId(),  datagram->getTimeStampes(), datagram->getDLC());
    emit disengagementCommitMessageArrived(disengagementCommitMessage);
}

//! -------------------------------------------------------------------------------------------------------------------
//!
//! \brief lienaDatagramDecoder::convertLienaDatagramToChannelClosedMessage
//! \param datagram
//!
void lienaDatagramDecoder::convertLienaDatagramToChannelClosedMessage(lienaDatagram *datagram){
    if(DEBUG){
        qDebug()<<"lienaDatagramDecoder::convertLienaDatagramToChannelClosedMessage";
    }

    lienaChannelClosedMessage * channelClosedMsg = new lienaChannelClosedMessage(datagram->getMessageID(), datagram->getTargetId(), datagram->getOriginId(),  datagram->getTimeStampes(), datagram->getDLC());
    emit channelClosedMessageArrived(channelClosedMsg);
}

//! ---------------------------------------------------------------------------------------------------------------------------------
//!
//! \brief lienaDatagramDecoder::convertLienaDatagramToHandShakeCommitMessage
//! \param datagram
//!
void lienaDatagramDecoder::convertLienaDatagramToHandShakeCommitMessage(lienaDatagram *datagram){
    if(DEBUG){
        qDebug()<<"lienaDatagramDecoder::convertLienaDatagramToHandShakeCommitMessage";
    }

    lienaHandShakeCommitMessage *handShakeCommitMessage = new lienaHandShakeCommitMessage(datagram->getMessageID(),
                                                                                          datagram->getTargetId(),
                                                                                          datagram->getOriginId(),
                                                                                          datagram->getTimeStampes(),
                                                                                          datagram->getDLC());
    emit handShakeCommitMessageArrived();
    //handshakeInstructionCount ++;
    return;
}

//! -------------------------------------------------------------------------------------------------------------------
//!
//! \brief lienaDatagramDecoder::convertLienaDatagramToChannelOpenedMessage
//! \param datagram
//!
void lienaDatagramDecoder::convertLienaDatagramToChannelOpenedMessage(lienaDatagram *datagram){
    lienaChannelOpenedMessage * channelOpenedMsg = new lienaChannelOpenedMessage(datagram->getMessageID(), datagram->getTargetId(),  datagram->getOriginId(), datagram->getTimeStampes(), datagram->getDLC());
    emit channelOpenMsg(channelOpenedMsg);
}

//! ---------------------------------------------------------------------------------------------------------------------------------
//!
//! \brief igtDatagramAnalyser::decodeHandShakeMsg
//! \param datagram
//!
void lienaDatagramDecoder::convertLienaDatagramToHandShakeMessage(lienaDatagram *datagram){
    qDebug()<<"lienaDatagramDecoder::convertLienaDatagramToHandShakeMessage";

    char *body = datagram->getBody();
    QString addr;
    addr.append(QString::number(uint8_t(body[0])));
    addr.append(".");
    addr.append(QString::number(uint8_t(body[1])));
    addr.append(".");
    addr.append(QString::number(uint8_t(body[2])));
    addr.append(".");
    addr.append(QString::number(uint8_t(body[3])));

    unsigned short port = uint8_t(body[4])*256 + uint8_t(body[5]);

    lienaHandShakeMessage *handShakeMessage = new lienaHandShakeMessage(datagram->getMessageID(),
                                                                        datagram->getTargetId(),
                                                                        datagram->getOriginId(),
                                                                        datagram->getTimeStampes(),
                                                                        datagram->getDLC(),
                                                                        addr,
                                                                        port);
    //handShakeMessage->printHeader();
    emit handShakeMessageArrived(handShakeMessage);
    handshakeInstructionCount ++;
    return;
}

//! -------------------------------------------------------------------------------------------------------------------
//!
//! \brief lienaDatagramDecoder::printHeartBeat
//!
void lienaDatagramDecoder::convertLienaDatagramToHeartBeatMessage(lienaDatagram *datagram){
    lienaHeartBeatMessage *heartBeatMessage = new lienaHeartBeatMessage(datagram->getMessageID(),
                                                                        datagram->getTargetId(),
                                                                        datagram->getOriginId(),
                                                                        datagram->getTimeStampes(),
                                                                        datagram->getDLC());
    qDebug()<<"lienaDatagramDecoder::convertLienaDatagramToHeartBeatMessage";
    emit heartBeatMessageArrived(heartBeatMessage);
    return;
}

//! ---------------------------------------------------------------------------------------------------------------------------------
//!
//! \brief igtDatagramDecoder::decodeVelocityIsMsg
//! \param datagram
//!
void lienaDatagramDecoder::decodeVelocityIsMsg(lienaDatagram *datagram){
//    lienaAdvancementTargetVelocity *advancementTargetVelocity = new lienaAdvancementTargetVelocity();
//    advancementTargetVelocity->transformIgtdatagramToAdvancementTargetVelocity(datagram);
//    this->inputMessageCache->appendAdvancementTargetVelocity(advancementTargetVelocity);
}

//! ---------------------------------------------------------------------------------------------------------------------------------
//!
//! \brief igtDatagramAnalyser::decodeHelloMsg
//! \param datagram
//!
void lienaDatagramDecoder::decodeHelloMsg(lienaDatagram *datagram){
    return;
}

//! ---------------------------------------------------------------------------------------------------------------------------------
//!
//! \brief igtDatagramAnalyser::decodeHandShakeCommitMsg
//! \param datagram
//!
void lienaDatagramDecoder::decodeHandShakeCommitMsg(lienaDatagram *datagram){
    emit connectionConfirm();
    return;
}

//! ---------------------------------------------------------------------------------------------------------------------------------
//!
//! \brief igtDatagramAnalyser::decodeForceTorqueValues
//! \param datagram
//!
void lienaDatagramDecoder::decodeForceTorqueValues(lienaDatagram *datagram){
//    lienaForceTorqueValues *forceTorqueValues = new lienaForceTorqueValues();
//    forceTorqueValues->transformIgtdatagramToForceFeedback(datagram);
//    this->inputMessageCache->appendForceTorqueValues(forceTorqueValues);
    return;
}

//! ---------------------------------------------------------------------------------------------------------------------------------
//!
//! \brief igtDatagramAnalyser::decodeNDIPosition
//! \param datagram
//!
void lienaDatagramDecoder::decodeNDIPosition(lienaDatagram *datagram){
//    lienaNDIPosition *NDIPosition = new lienaNDIPosition();
//    NDIPosition->transformIgtdatagramToNDIPosition(datagram);
//    this->inputMessageCache->appendNDIPosition(NDIPosition);
}

//! ---------------------------------------------------------------------------------------------------------------------------------
//!
//! \brief igtDatagramAnalyser::decodeGuidewireMovingDistance
//! \param datagram
//!
void lienaDatagramDecoder::decodeGuidewireMovingDistance(lienaDatagram *datagram){
//    lienaGuidewireMovingDistance *dist = new lienaGuidewireMovingDistance();
//    dist->transformIgtdatagramToDistance(datagram);
//    this->inputMessageCache->appendGuidewireMovingDistanceSequence(dist->getValue());
}

//! -----------------------------------------------------------------------------------------------------------------------
//!
//! \brief lienaDatagramDecoder::setInputCache
//! \param inputMessageCache
//!
void lienaDatagramDecoder::setInputCache(lienaInputMessageCache* inputMessageCache){
    this->inputMessageCache = inputMessageCache;
}
