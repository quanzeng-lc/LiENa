#include "lienaDecodingTask.h"


/**
 *
 * @brief igtDecodingTask::igtDecodingTask
 * @param dataware
 * @param inputQueueManager
 * @param datagramAnalyser
 */
lienaDecodingTask::lienaDecodingTask(lienaInputQueue *inputQueue,
                                     bool motivate,
                                     uint32_t localDeviceid,
                                     uint32_t targetDeviceId){

    this->motivate = motivate;
    this->localDeviceid = localDeviceid;
    this->targetDeviceId = targetDeviceId;
    this->inputQueue = inputQueue;

    this->decoder = new lienaDatagramDecoder(this->motivate, this->localDeviceid, this->targetDeviceId);
    this->rtFlag = true;
    this->rtPeriod = 20;
    this->standby = true;

    this->connect(this->decoder, SIGNAL(handShakeMessageArrived(lienaHandShakeMessage *)),                     this, SLOT(notiftHandShakeMessage(lienaHandShakeMessage *)));
    this->connect(this->decoder, SIGNAL(handShakeCommitMessageArrived()),                                      this, SLOT(channelOpen()));
    this->connect(this->decoder, SIGNAL(disengagementMessageArrived(lienaDisengagementMessage *)),             this, SLOT(notiftDisengagementMessage(lienaDisengagementMessage *)));
    this->connect(this->decoder, SIGNAL(disengagementCommitMessageArrived(lienaDisengagementCommitMessage *)), this, SLOT(notiftDisengagementCommitMessage(lienaDisengagementCommitMessage *)));
    this->connect(this->decoder, SIGNAL(connectionConfirm()),                                                  this, SLOT(connectionEstablish()));
    this->connect(this->decoder, SIGNAL(channelOpenMsg(lienaChannelOpenedMessage *)),                          this, SLOT(notiftHeartBeatMessage(lienaChannelOpenedMessage *)));
    this->connect(this->decoder, SIGNAL(channelClosedMessageArrived(lienaChannelClosedMessage * )),            this, SLOT(notifyChannelClosedMessage()));
    this->connect(this->decoder, SIGNAL(heartBeatMessageArrived(lienaHeartBeatMessage*)),                      this, SLOT(notifyHeartBeatMessage(lienaHeartBeatMessage*)));
    this->connect(this->decoder, SIGNAL(reHandshakeMessageArrived(lienaReHandshakeMessage*)),                  this, SLOT(notifyReHandshakeMsg(lienaReHandshakeMessage*)));
    this->connect(this->decoder, SIGNAL(reHandshakeCommitMessageArrived(lienaReHandshakeCommitMessage*)),      this, SLOT(notifyReHandshakeCommit(lienaReHandshakeCommitMessage*)));
    this->connect(this->decoder, SIGNAL(channelReOpenedMessageArrived(lienaChannelReOpened*)),                 this, SLOT(notifyChannelReOpened(lienaChannelReOpened*)));
    this->connect(this->decoder, SIGNAL(passiveNTPClockSynchronizationMessageArrived(lienaNetworkQualityMessage*)), this, SLOT(notifyPassiveNetworkQualityMessage(lienaNetworkQualityMessage*)));
    this->connect(this->decoder, SIGNAL(motivateNTPClockSynchronizationMessageArrived(lienaNetworkQualityMessage*)),this,SLOT(notifyMotivateNetworkQualityMessage(lienaNetworkQualityMessage*)));
}

//! ---------------------------------------------------------------------------------------------------------------------------------------------
//!
//! \brief lienaDecodingTask::notifyMotivateNetworkQualityMessage
//! \param msg
//!
void lienaDecodingTask::notifyMotivateNetworkQualityMessage(lienaNetworkQualityMessage *msg){
    if(DEBUG){
        qDebug()<<"lienaDecodingTask::notifyMotivateNetworkQualityMessage";
    }
    emit networkMotivateQualityMessageArrived(msg);
}

//! ---------------------------------------------------------------------------------------------------------------------------------------------
//!
//! \brief lienaDecodingTask::notifyPassiveNetworkQualityMessage
//! \param msg
//!
void lienaDecodingTask::notifyPassiveNetworkQualityMessage(lienaNetworkQualityMessage *msg){
    if(DEBUG){
        qDebug()<<"lienaDecodingTask::notifyPassiveNetworkQualityMessage";
    }
    emit networkPassiveQualityMessageArrived(msg);
}

//! ---------------------------------------------------------------------------------------------------------------------------------------------
//!
//! \brief lienaDecodingTask::notifyReHandshakeMsg
//! \param msg
//!
void lienaDecodingTask::notifyReHandshakeMsg(lienaReHandshakeMessage* msg){
    if(DEBUG){
        qDebug()<<"lienaDecodingTask::notifyReHandshakeMsg";
    }
    emit rehandshakeMsgArrived(msg);
}

//! ---------------------------------------------------------------------------------------------------------------------------------------------
//!
//! \brief lienaDecodingTask::notifyReHandshakeCommit
//! \param msg
//!
void lienaDecodingTask::notifyReHandshakeCommit(lienaReHandshakeCommitMessage* msg){
    emit rehandshakeCommitMsgArrived(msg);
}

//! ---------------------------------------------------------------------------------------------------------------------------------------------
//!
//! \brief lienaDecodingTask::notifyChannelReOpened
//! \param msg
//!
void lienaDecodingTask::notifyChannelReOpened(lienaChannelReOpened* msg){
    emit reOpenedMsgArrived(msg);
}

//! ---------------------------------------------------------------------------------------------------------------------------------------------
//!
//! \brief lienaDecodingTask::notifyHeartBeatMessage
//! \param msg
//!
void lienaDecodingTask::notifyHeartBeatMessage(lienaHeartBeatMessage* msg){
    emit heartBeatMessageArrived(msg);
}

//! ---------------------------------------------------------------------------------------------------------------------------------------------
//!
//! \brief lienaDecodingTask::channelClosed
//!
void lienaDecodingTask::notifyChannelClosedMessage(){
    qDebug()<<"channelClosed";
    emit channelClosedMessageArrived();
}

//! ---------------------------------------------------------------------------------------------------------------------------------------------
//!
//! \brief lienaDecodingTask::channelOpen
//!
void lienaDecodingTask::channelOpen(){
    qDebug()<<"lienaDecodingTask::channelOpen()";
    emit handShakeCommitMessageArrived();
}

//! ---------------------------------------------------------------------------------------------------------------------------------------------
//!
//! \brief lienaDecodingTask::notiftHeartBeatMessage
//! \param channelOpenedMsg
//!
void lienaDecodingTask::notiftHeartBeatMessage(lienaChannelOpenedMessage* channelOpenedMsg){
    emit channelOpenMsg(channelOpenedMsg);
}

//! ---------------------------------------------------------------------------------------------------------------------------------------------
//!
//! \brief lienaDecodingTask::~lienaDecodingTask
//!
lienaDecodingTask::~lienaDecodingTask(){
    delete this->decoder;
}

//! ---------------------------------------------------------------------------------------------------------------------------------------------
//!
//! \brief lienaDecodingTask::enable
//!
void lienaDecodingTask::enable(){
    this->standby = false;

}

//! ---------------------------------------------------------------------------------------------------------------------------------------------
//!
//! \brief lienaDecodingTask::freeze
//!
void lienaDecodingTask::freeze(){
    this->standby = true;

}

//! ---------------------------------------------------------------------------------------------------------------------------------------------
//!
//! \brief lienaDecodingTask::launch
//!
void lienaDecodingTask::launch(){
    this->standby = false;
    this->start();
}

//! ---------------------------------------------------------------------------------------------------------------------------------------------
//!
//! \brief igtDecodingTask::run
//!
void lienaDecodingTask::run(){
    while(this->rtFlag){

        if(standby){
            this->sleep(1);
            continue;
        }

        //! to check if the input queue is empty
        if(this->inputQueue->getLength() > 0){

            //! fetch
            lienaDatagram *datagram = inputQueue->getFrontDatagram();

            //! analysis
            this->decoder->analyse(datagram);

            //! delete
            this->inputQueue->deleteFrontDatagram();

        }

        //! latency
        msleep(this->rtPeriod);
    }
}

//! -----------------------------------------------------------------------------------------------------------------------------------------------
//!
//! \brief igtDecodingTask::receptConnection
//! \param remoteIP
//!
void lienaDecodingTask::notiftHandShakeMessage(lienaHandShakeMessage *handShakeMessage){
    emit handShakeMessageArrived(handShakeMessage);
}

//! -----------------------------------------------------------------------------------------------------------------------------------------------
//!
//! \brief lienaDecodingTask::notiftDisengagementMessage
//! \param disengagementMessage
//!
void lienaDecodingTask::notiftDisengagementMessage(lienaDisengagementMessage *disengagementMessage){
    emit disengagementMessageArrived(disengagementMessage);
}

void lienaDecodingTask::notiftDisengagementCommitMessage(lienaDisengagementCommitMessage *disengagementCommitMessage){
    qDebug()<<"lienaDecodingTask | notiftDisengagementCommitMessage emit disengagementCommitMessageArrived";
    emit disengagementCommitMessageArrived(disengagementCommitMessage);
}

//! ----------------------------------------------------------------------------------------------------------------------------------------------
//!
//! \brief lienaDecodingTask::setPeriod
//! \param rtPeriod
//!
void lienaDecodingTask::setPeriod(uint32_t rtPeriod){
    this->rtPeriod = rtPeriod;
}

//! ----------------------------------------------------------------------------------------------------------------------------------------------
//!
//! \brief lienaDecodingTask::setInputCache
//! \param inputMessageCache
//!
void lienaDecodingTask::setInputMessageQueue(lienaMessageQueue* inputMessageQueue){
    //this->decoder->setInputCache(inputMessageCache);
}

//! ----------------------------------------------------------------------------------------------------------------------------------------------
//!
//! \brief igtDecodingTask::connectionEstablish
//!
void lienaDecodingTask::connectionEstablish(){
    emit connexionConfirm();
}

//! ----------------------------------------------------------------------------------------------------------------------------------------------
//!
//! \brief igtDecodingTask::stop
//!
void lienaDecodingTask::stop(){
    rtFlag = false;
}
