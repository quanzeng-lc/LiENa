#include "lienaEncodingTask.h"


/**
 * @brief igtEncodingTask::igtEncodingTask
 * @param dataware
 * @param outputQueueManager
 * @param datagramAnalyser
 */
lienaEncodingTask::lienaEncodingTask(lienaOutputQueue *outputQueue, lienaGlobal *globalParameter, bool motivate, uint32_t targetDeviceId){
    this->flag = true;
    this->rtPeriod = 20;

    this->standby = true;

    this->motivate = motivate;
    this->targetDeviceId = targetDeviceId;

    this->msgQ = nullptr;
    this->outputQueue = outputQueue;
    this->globalParameter = globalParameter;
    this->encoder = new lienaDatagramEncoder(globalParameter);
}

//! ---------------------------------------------------------------------------------------------------------------
//!
//! \brief lienaEncodingTask::~lienaEncodingTask
//!
lienaEncodingTask::~lienaEncodingTask(){
    delete this->encoder;
}

//! ---------------------------------------------------------------------------------------------------------------
//!
//! \brief lienaEncodingTask::sendDisengagementCommitMessage
//!
void lienaEncodingTask::sendDisengagementCommitMessage(){
    if(DEBUG){
        qDebug()<<"lienaEncodingTask::sendDisengagementCommitMessage";
    }

    uint64_t message_id = LIENA_SESSION_MANAGEMENT_DISENGAGEMENTCOMMIT_MESSAGE;
    uint32_t targetDeviceId = this->targetDeviceId;
    uint64_t  timeStamps = this->globalParameter->getTimestamps();
    uint32_t originId = this->globalParameter->getLocalDeviceId();

    lienaDisengagementCommitMessage *disengagementCommitMessage = new lienaDisengagementCommitMessage(message_id, targetDeviceId,originId, timeStamps, 6);

    lienaDatagram *datagram = encoder->transformDisengagementCommitMessageToIgtDatagram(disengagementCommitMessage);
    this->outputQueue->append(datagram);
}

//! ---------------------------------------------------------------------------------------------------------------
//!
//! \brief lienaEncodingTask::sendDisengagementmessage
//!
void lienaEncodingTask::sendDisengagementMessage(){
    if(DEBUG){
        qDebug()<<"lienaEncodingTask::sendDisengagementmessage :";
    }

    uint64_t message_id = LIENA_SESSION_MANAGEMENT_DISENGAGEMENT_MESSAGE;
    uint32_t targetDeviceId = this->targetDeviceId;
    uint64_t timeStamps = this->globalParameter->getTimestamps();
    uint32_t originId = this->globalParameter->getLocalDeviceId();

    lienaDisengagementMessage *disengagementMessage = new lienaDisengagementMessage(message_id, targetDeviceId, originId, timeStamps, 6);

    lienaDatagram *datagram = encoder->transformDisengagementMessageToIgtDatagram(disengagementMessage);
    this->outputQueue->append(datagram);
}

//! ---------------------------------------------------------------------------------------------------------------
//!
//! \brief lienaEncodingTask::sendChannelClosedMessage
//!
void lienaEncodingTask::sendChannelClosedMessage(){
    if(DEBUG){
        qDebug()<<"lienaEncodingTask::sendChannelClosedMessage";
    }

    uint64_t message_id = LIENA_SESSION_MANAGEMENT_CHANNELCLOSED_MESSAGE;
    uint32_t targetDeviceId = this->targetDeviceId;
    uint64_t timeStamps = this->globalParameter->getTimestamps();
    uint32_t originId = this->globalParameter->getLocalDeviceId();

    lienaChannelClosedMessage *channelClosedMessage = new lienaChannelClosedMessage(message_id, targetDeviceId, originId, timeStamps, 6);

    lienaDatagram *datagram = encoder->transformchannelClosedMessageToIgtDatagram(channelClosedMessage);
    this->outputQueue->append(datagram);
}

//! -------------------------------------------------------------------------------------------------------------------------------------------------
//!
//! \brief igtClient::sendHandShakeCommitMessage
//!
void lienaEncodingTask::sendHandShakeCommitMessage(){
    if(DEBUG){
        qDebug()<<"lienaEncodingTask::sendHandShakeCommitMessage";
    }

    uint64_t message_id = LIENA_SESSION_MANAGEMENT_HANDSHAKECOMMIT_MESSAGE;
    uint32_t targetDeviceId = this->targetDeviceId;
    uint64_t timeStamps = this->globalParameter->getTimestamps();
    uint32_t originId = this->globalParameter->getLocalDeviceId();

    lienaHandShakeCommitMessage *handShakeCommitMessage = new lienaHandShakeCommitMessage(message_id, targetDeviceId, originId, timeStamps, 6);

    lienaDatagram *datagram = encoder->transformHandShakeCommitMessageToIgtDatagram(handShakeCommitMessage);
    this->outputQueue->append(datagram);
}

//! ---------------------------------------------------------------------------------------------------------------
//!
//! \brief lienaEncodingTask::sendReHandshakeMessage
//!
void lienaEncodingTask::sendReHandshakeMessage(){
    if(DEBUG){
        qDebug()<<"lienaEncodingTask | sendReHandshakeMessage";
    }
    uint64_t message_id = LIENA_SESSION_MANAGEMENT_REHANDSHAKE_MESSAGE;
    uint32_t targetDeviceId = this->targetDeviceId;
    uint64_t timeStamps = this->globalParameter->getTimestamps();
    uint32_t originId = this->globalParameter->getLocalDeviceId();

    lienaReHandshakeMessage *reHandShakeMessage = new lienaReHandshakeMessage(message_id, targetDeviceId, originId, timeStamps, 6);
    lienaDatagram *datagram = encoder->transformReHandShakeMessageToIgtDatagram(reHandShakeMessage);
    this->outputQueue->append(datagram);
}

//! ---------------------------------------------------------------------------------------------------------------
//!
//! \brief lienaEncodingTask::sendReHandshakeCommitMessage
//!
void lienaEncodingTask::sendReHandshakeCommitMessage(){
    if(DEBUG){
        qDebug()<<"lienaEncodingTask | sendReHandshakeCommitMessage";
    }
    uint64_t message_id = LIENA_SESSION_MANAGEMENT_REHANDSHAKECOMMIT_MESSAGE;
    uint32_t targetDeviceId = this->targetDeviceId;
    uint64_t timeStamps = this->globalParameter->getTimestamps();
    uint32_t originId = this->globalParameter->getLocalDeviceId();

    lienaReHandshakeCommitMessage *reHandShakeCommitMessage = new lienaReHandshakeCommitMessage(message_id, targetDeviceId , originId, timeStamps, 6);
    lienaDatagram *datagram = encoder->transformReHandShakeCommitMessageToIgtDatagram(reHandShakeCommitMessage);
    this->outputQueue->append(datagram);
}

//! ---------------------------------------------------------------------------------------------------------------
//!
//! \brief lienaEncodingTask::sendChannelReOpened
//!
void lienaEncodingTask::sendChannelRepairedMessage(){
    if(DEBUG){
        qDebug()<<"lienaEncodingTask | sendChannelReOpened";
    }
    uint64_t message_id = LIENA_SESSION_MANAGEMENT_REOPENED_MESSAGE;
    uint32_t targetDeviceId = this->targetDeviceId;
    uint64_t timeStamps = this->globalParameter->getTimestamps();
    uint32_t originId = this->globalParameter->getLocalDeviceId();

    lienaChannelReOpened *channelReOpenedMessage = new lienaChannelReOpened(message_id, targetDeviceId,originId,  timeStamps, 6);
    lienaDatagram *datagram = encoder->transformChannelReOpenedMessageToIgtDatagram(channelReOpenedMessage);
    this->outputQueue->append(datagram);
}

//! ---------------------------------------------------------------------------------------------------------------
//!
//! \brief lienaEncodingTask::sendChannelOpenedMessage
//!
void lienaEncodingTask::sendChannelOpenedMessage(){
    if(DEBUG){
        qDebug()<<"lienaEncodingTask::sendChannelOpenedMessage";
    }
    uint64_t message_id = LIENA_SESSION_MANAGEMENT_CHANNELOPENNED_MESSAGE;
    uint32_t targetDeviceId = this->targetDeviceId;
    uint64_t timeStamps = this->globalParameter->getTimestamps();
    uint32_t originId = this->globalParameter->getLocalDeviceId();

    lienaChannelOpenedMessage *channelOpenedMsg = new lienaChannelOpenedMessage(message_id, targetDeviceId, originId, timeStamps, 6);
    lienaDatagram *datagram = encoder->transformChannelOpenedMessageToIgtDatagram(channelOpenedMsg);
    this->outputQueue->append(datagram);
}

//! ---------------------------------------------------------------------------------------------------------------
//!
//! \brief lienaTransmissionTask::sendHandShakeMessage
//!
void lienaEncodingTask::sendHandShakeMessage(QString addr){
    if(DEBUG){
        qDebug()<<"lienaTransmissionTask::sendHandShakeMessage";
    }

    uint64_t message_id = LIENA_SESSION_MANAGEMENT_HANDSHAKE_MESSAGE;
    uint32_t targetDeviceId = this->targetDeviceId;
    uint64_t timeStamps = this->globalParameter->getTimestamps();
    uint32_t originId = this->globalParameter->getLocalDeviceId();

    lienaHandShakeMessage *handShakeMessage = new lienaHandShakeMessage(message_id, targetDeviceId, originId, timeStamps, 6, addr, 10704);
    lienaDatagram *datagram = this->encoder->encode_handshake_message(handShakeMessage);
    outputQueue->append(datagram);
}

//! ---------------------------------------------------------------------------------------------------------------
//!
//! \brief lienaEncodingTask::launch
//!
void lienaEncodingTask::launch(){
    this->standby = false;
    this->start();
}

//! ---------------------------------------------------------------------------------------------------------------
//!
//! \brief lienaEncodingTask::enable
//!
void lienaEncodingTask::enable(){
    this->standby = false;
}

//! ---------------------------------------------------------------------------------------------------------------
//!
//! \brief lienaEncodingTask::freeze
//!
void lienaEncodingTask::freeze(){
    this->standby = true;
}

//! ---------------------------------------------------------------------------------------------------------------
//!
//! \brief igtEncodingTask::setPatientHandling
//! \param patientHandling
//!
void lienaEncodingTask::setOutputMessageQueue(lienaMessageQueue* msgQ){
    this->msgQ = msgQ;
}

//! ---------------------------------------------------------------------------------------------------------------
//!
//! \brief igtEncodingTask::run
//!
void lienaEncodingTask::run(){
    while (flag) {

        if(standby){
            this->msleep(1000);
            continue;
        }
        if (this->msgQ != nullptr){
            if(!this->msgQ->isEmpty()){
                lienaCustomizedMessage *msg = this->msgQ->pop_front();



                //transformation(msg);


                outputQueue->append(this->encoder->encode_customized_message(msg));
            }
        }

        msleep(this->rtPeriod);
    }
}

//! ---------------------------------------------------------------------------------------------------------------
//!
//! \brief lienaEncodingTask::setPeriod
//! \param rtPeriod
//!
void lienaEncodingTask::setPeriod(uint32_t rtPeriod){
    this->rtPeriod = rtPeriod;
}

//! ---------------------------------------------------------------------------------------------------------------
//!
//! \brief lienaEncodingTask::stop
//!
void lienaEncodingTask::stop(){
    this->flag = false;
}


