#include "lienaHeartBeatTask.h"


/**
 * @brief lienaHeartBeatTask::lienaHeartBeatTask
 * @param outputQueue
 * @param globalParameter
 */
lienaHeartBeatTask::lienaHeartBeatTask(lienaOutputQueue* outputQueue, lienaGlobal *globalParameter)
{
    this->outputQueue = outputQueue;
    this->flag = true;
    this->standby = true;
    this->globalParameter = globalParameter;
}

//! ------------------------------------------------------------------------------------------------------------------------
//!
//! \brief lienaHeartBeatTask::stop
//!
void lienaHeartBeatTask::stop(){
    this->flag = false;
}

//! ------------------------------------------------------------------------------------------------------------------------
//!
//! \brief lienaHeartBeatTask::launch
//!
void lienaHeartBeatTask::launch(){
    this->standby = false;
    this->start();
}

//! ------------------------------------------------------------------------------------------------------------------------
//!
//! \brief lienaHeartBeatTask::enable
//!
void lienaHeartBeatTask::enable(){
    this->standby = false;
}

//! ------------------------------------------------------------------------------------------------------------------------
//!
//! \brief lienaHeartBeatTask::freeze
//!
void lienaHeartBeatTask::freeze(){
    this->standby = true;
}

//! ------------------------------------------------------------------------------------------------------------------------
//!
//! \brief lienaHeartBeatTask::sendHeartBeatMessage
//!
void lienaHeartBeatTask::sendHeartBeatMessage(){
    uint64_t message_id = LIENA_SESSION_MANAGEMENT_HEARTBEAT_MESSAGE;

    if(DEBUG){
        qDebug()<<"heartbeatMessage | message_id: "<<message_id;
    }
    lienaHeartBeatMessage* heartBeatMessage = new lienaHeartBeatMessage(message_id, 0, 0, 123456, 10);
    lienaDatagram* datagram =this->transformHeartbeatMessageToIgtDatagram(heartBeatMessage);

    this->outputQueue->append(datagram);
}

//! ------------------------------------------------------------------------------------------------------------------------
//!
//! \brief lienaHeartBeatTask::run
//!
void lienaHeartBeatTask::run(){

    while (this->flag) {
        if(this->standby){
            sleep(1);
            continue;
        }
        this->sendHeartBeatMessage();
        sleep(1);
    }
    return;
}

//! ------------------------------------------------------------------------------------------------------------------------
//!
//! \brief lienaHeartBeatTask::transformHandShakeCommitMessageToIgtDatagram
//! \param heartBeatMessage
//! \return
//!
lienaDatagram* lienaHeartBeatTask::transformHeartbeatMessageToIgtDatagram(lienaHeartBeatMessage* heartBeatMessage){
    lienaDatagram *datagram = nullptr;

    if(heartBeatMessage != nullptr){

        uint32_t globalDatagramSize = globalParameter->getGlobalDatagramSize();
        char* byteArray = new char[globalDatagramSize];

        uint64_t messageId   = heartBeatMessage->getMessageId();
        uint32_t targetId    = heartBeatMessage->getTargetId();
        uint32_t originId    = heartBeatMessage->getOriginId();
        uint64_t timestampes = heartBeatMessage->getTimestampes();
        uint32_t dlc         = heartBeatMessage->getDLC();

        //-------------------------------message id-----------------------
        byteArray[0] = (uchar)((0xff00000000000000&messageId)>>56);
        byteArray[1] = (uchar)((0x00ff000000000000&messageId)>>48);
        byteArray[2] = (uchar)((0x0000ff0000000000&messageId)>>40);
        byteArray[3] = (uchar)((0x000000ff00000000&messageId)>>32);
        byteArray[4] = (uchar)((0x00000000ff000000&messageId)>>24);
        byteArray[5] = (uchar)((0x0000000000ff0000&messageId)>>16);
        byteArray[6] = (uchar)((0x000000000000ff00&messageId)>>8);
        byteArray[7] = (uchar)((0x00000000000000ff&messageId));

        //-------------------------------target id-----------------------
        byteArray[8] =  (uchar)((0xff000000&targetId)>>24);
        byteArray[9] =  (uchar)((0x00ff0000&targetId)>>16);
        byteArray[10] = (uchar)((0x0000ff00&targetId)>>8);
        byteArray[11] = (uchar)((0x000000ff&targetId));

        //-------------------------------origin id-----------------------
        byteArray[12] =  (uchar)((0xff000000&originId)>>24);
        byteArray[13] =  (uchar)((0x00ff0000&originId)>>16);
        byteArray[14] = (uchar)((0x0000ff00&originId)>>8);
        byteArray[15] = (uchar)((0x000000ff&originId));

        //-------------------------------time stamps -----------------------
        byteArray[16] = (uchar)((0xff00000000000000&timestampes)>>56);
        byteArray[17] = (uchar)((0x00ff000000000000&timestampes)>>48);
        byteArray[18] = (uchar)((0x0000ff0000000000&timestampes)>>40);
        byteArray[19] = (uchar)((0x000000ff00000000&timestampes)>>32);
        byteArray[20] = (uchar)((0x00000000ff000000&timestampes)>>24);
        byteArray[21] = (uchar)((0x0000000000ff0000&timestampes)>>16);
        byteArray[22] = (uchar)((0x000000000000ff00&timestampes)>>8);
        byteArray[23] = (uchar)((0x00000000000000ff&timestampes));

        //-------------------------------dlc -----------------------------

        byteArray[24] =  (uchar)((0xff000000&dlc)>>24);
        byteArray[25] =  (uchar)((0x00ff0000&dlc)>>16);
        byteArray[26] = (uchar)((0x0000ff00&dlc)>>8);
        byteArray[27] = (uchar)((0x000000ff&dlc));

        //-------------------------------body -----------------------

        for(uint32_t i =28; i< globalDatagramSize; i++){
            byteArray[i] = '\0';
        }

        datagram = new lienaDatagram(globalDatagramSize, &byteArray);
    }

    return datagram;
}

