#include "lienaNTPSynchronizationTask.h"


/**
 * @brief lienaNTPSynchronizationTask::lienaNTPSynchronizationTask
 * @param outputQueue
 * @param globalParameter
 * @param targetDeviceId
 */
lienaNTPSynchronizationTask::lienaNTPSynchronizationTask(lienaOutputQueue* outputQueue,
                                                         lienaGlobal *globalParameter,
                                                         unsigned int targetDeviceId)
{
    this->outputQueue = outputQueue;
    this->globalParameter = globalParameter;
    this->targetDeviceId = targetDeviceId;
    this->flag = true;
    this->standby = true;

    this->rtPeriod = 50;
    this->loopNumber = 10;
}

//! -------------------------------------------------------------------------------------------------------------
//!
//! \brief lienaNTPSynchronizationTask::setLoopNumber
//! \param loopNumber
//!
void lienaNTPSynchronizationTask::setLoopNumber(int loopNumber){
    this->loopNumber = loopNumber;
}

//! -------------------------------------------------------------------------------------------------------------
//!
//! \brief lienaNTPSynchronizationTask::receive
//! \param msg
//!
void lienaNTPSynchronizationTask::receive(lienaNetworkQualityMessage* msg){
    this->msgReturned.append(msg);
    if(this->msgReturned.length() == this->loopNumber){
        qDebug()<<"all message returned";
    }
}

//! -------------------------------------------------------------------------------------------------------------
//!
//! \brief lienaNTPSynchronizationTask::launch
//!
void lienaNTPSynchronizationTask::launch(){
    this->standby = false;
    this->start();
}

//! -------------------------------------------------------------------------------------------------------------
//!
//! \brief lienaNTPSynchronizationTask::stop
//!
void lienaNTPSynchronizationTask::stop(){
    this->flag = false;
}

//! -------------------------------------------------------------------------------------------------------------
//!
//! \brief lienaNTPSynchronizationTask::run
//!
void lienaNTPSynchronizationTask::run(){

    for(int i =0; i <this->loopNumber; i++){
        this->sendNetworkQualityMessage(i);
        msleep(rtPeriod);
    }
}

//! -------------------------------------------------------------------------------------------------------------
//!
//! \brief lienaNTPSynchronizationTask::sendBackNetworkQualityMessage
//! \param networkQualityMessage
//!
void lienaNTPSynchronizationTask::sendBackNetworkQualityMessage(lienaNetworkQualityMessage* networkQualityMessage){
    qDebug()<<"lienaNTPSynchronizationTask::sendBackNetworkQualityMessage";
    lienaDatagram* datagram = this->encodeNetworkQualityMessage(networkQualityMessage);
    if(datagram != nullptr){
        this->outputQueue->append(datagram);
    }
}

//! -------------------------------------------------------------------------------------------------------------
//!
//! \brief lienaNTPSynchronizationTask::sendNetworkQualityMessage
//! \param count
//!
void lienaNTPSynchronizationTask::sendNetworkQualityMessage(int count){
    if(DEBUG){
        qDebug()<<"lienaNTPSynchronizationTask::sendNetworkQualityMessage";
    }
    uint64_t message_id = LIENA_SESSION_MANAGMENT_NTP_SYNCHRONIZATION_MESSAGE;
    uint64_t timeStamps = this->globalParameter->getTimestamps();
    uint32_t targetDeviceId = this->targetDeviceId;
    uint32_t originId = this->globalParameter->getLocalDeviceId();

    networkQualityMessage = new lienaNetworkQualityMessage(message_id, targetDeviceId, originId,timeStamps, 33);
    networkQualityMessage->set_index(count);
    networkQualityMessage->set_t1(0);

    lienaDatagram* datagram = this->encodeNetworkQualityMessage(networkQualityMessage);
    this->outputQueue->append(datagram);
}

//! -------------------------------------------------------------------------------------------------------------
//!
//! \brief lienaNTPSynchronizationTask::encodeNetworkQualityMessage
//! \param networkQualityMessage
//! \return
//!
lienaDatagram* lienaNTPSynchronizationTask::encodeNetworkQualityMessage(lienaNetworkQualityMessage* networkQualityMessage){

    lienaDatagram *datagram = nullptr;
    if(networkQualityMessage != nullptr){

        uint32_t globalDatagramSize = globalParameter->getGlobalDatagramSize();
        qDebug()<<"globalDatagramSize:"<<globalDatagramSize;
        char* byteArray = new char[globalDatagramSize];

        uint64_t  messageId = networkQualityMessage->getMessageId();
        uint32_t targetId = networkQualityMessage->getTargetId();
        uint32_t originId = networkQualityMessage->getOriginId();
        uint64_t timestampes = networkQualityMessage->getTimestampes();
        uint32_t dlc = networkQualityMessage->getDLC();
        byteArray[0]  = uchar((0xff00000000000000&messageId)>>56);
        byteArray[1]  = uchar((0x00ff000000000000&messageId)>>48);
        byteArray[2]  = uchar((0x0000ff0000000000&messageId)>>40);
        byteArray[3]  = uchar((0x000000ff00000000&messageId)>>32);
        byteArray[4]  = uchar((0x00000000ff000000&messageId)>>24);
        byteArray[5]  = uchar((0x0000000000ff0000&messageId)>>16);
        byteArray[6]  = uchar((0x000000000000ff00&messageId)>>8);
        byteArray[7]  = uchar((0x00000000000000ff&messageId));

        byteArray[8]  = uchar((0xff000000&targetId)>>24);
        byteArray[9]  = uchar((0x00ff0000&targetId)>>16);
        byteArray[10] = uchar((0x0000ff00&targetId)>>8);
        byteArray[11] = uchar((0x000000ff&targetId));

        byteArray[12] = uchar((0xff000000&originId)>>24);
        byteArray[13] = uchar((0x00ff0000&originId)>>16);
        byteArray[14] = uchar((0x0000ff00&originId)>>8);
        byteArray[15] = uchar((0x000000ff&originId));

        byteArray[16] = uchar((0xff00000000000000&timestampes)>>56);
        byteArray[17] = uchar((0x00ff000000000000&timestampes)>>48);
        byteArray[18] = uchar((0x0000ff0000000000&timestampes)>>40);
        byteArray[19] = uchar((0x000000ff00000000&timestampes)>>32);
        byteArray[20] = uchar((0x00000000ff000000&timestampes)>>24);
        byteArray[21] = uchar((0x0000000000ff0000&timestampes)>>16);
        byteArray[22] = uchar((0x000000000000ff00&timestampes)>>8);
        byteArray[23] = uchar((0x00000000000000ff&timestampes));

        byteArray[24] = uchar((0xff000000&dlc)>>24);
        byteArray[25] = uchar((0x00ff0000&dlc)>>16);
        byteArray[26] = uchar((0x0000ff00&dlc)>>8);
        byteArray[27] = uchar((0x000000ff&dlc));

        byteArray[28] = uchar((0x000000ff&networkQualityMessage->get_index()));

        byteArray[29] = uchar((0xff00000000000000&networkQualityMessage->get_t1())>>56);
        byteArray[30] = uchar((0x00ff000000000000&networkQualityMessage->get_t1())>>48);
        byteArray[31] = uchar((0x0000ff0000000000&networkQualityMessage->get_t1())>>40);
        byteArray[32] = uchar((0x000000ff00000000&networkQualityMessage->get_t1())>>32);
        byteArray[33] = uchar((0x00000000ff000000&networkQualityMessage->get_t1())>>24);
        byteArray[34] = uchar((0x0000000000ff0000&networkQualityMessage->get_t1())>>16);
        byteArray[35] = uchar((0x000000000000ff00&networkQualityMessage->get_t1())>>8);
        byteArray[36] = uchar((0x00000000000000ff&networkQualityMessage->get_t1()));

        byteArray[37] = uchar((0xff00000000000000&networkQualityMessage->get_t2())>>56);
        byteArray[38] = uchar((0x00ff000000000000&networkQualityMessage->get_t2())>>48);
        byteArray[39] = uchar((0x0000ff0000000000&networkQualityMessage->get_t2())>>40);
        byteArray[40] = uchar((0x000000ff00000000&networkQualityMessage->get_t2())>>32);
        byteArray[41] = uchar((0x00000000ff000000&networkQualityMessage->get_t2())>>24);
        byteArray[42] = uchar((0x0000000000ff0000&networkQualityMessage->get_t2())>>16);
        byteArray[43] = uchar((0x000000000000ff00&networkQualityMessage->get_t2())>>8);
        byteArray[44] = uchar((0x00000000000000ff&networkQualityMessage->get_t2()));

        byteArray[45] = uchar((0xff00000000000000&networkQualityMessage->get_t3())>>56);
        byteArray[46] = uchar((0x00ff000000000000&networkQualityMessage->get_t3())>>48);
        byteArray[47] = uchar((0x0000ff0000000000&networkQualityMessage->get_t3())>>40);
        byteArray[48] = uchar((0x000000ff00000000&networkQualityMessage->get_t3())>>32);
        byteArray[49] = uchar((0x00000000ff000000&networkQualityMessage->get_t3())>>24);
        byteArray[50] = uchar((0x0000000000ff0000&networkQualityMessage->get_t3())>>16);
        byteArray[51] = uchar((0x000000000000ff00&networkQualityMessage->get_t3())>>8);
        byteArray[52] = uchar((0x00000000000000ff&networkQualityMessage->get_t3()));

        byteArray[53] = uchar((0xff00000000000000&networkQualityMessage->get_t4())>>56);
        byteArray[54] = uchar((0x00ff000000000000&networkQualityMessage->get_t4())>>48);
        byteArray[55] = uchar((0x0000ff0000000000&networkQualityMessage->get_t4())>>40);
        byteArray[56] = uchar((0x000000ff00000000&networkQualityMessage->get_t4())>>32);
        byteArray[57] = uchar((0x00000000ff000000&networkQualityMessage->get_t4())>>24);
        byteArray[58] = uchar((0x0000000000ff0000&networkQualityMessage->get_t4())>>16);
        byteArray[59] = uchar((0x000000000000ff00&networkQualityMessage->get_t4())>>8);
        byteArray[60] = uchar((0x00000000000000ff&networkQualityMessage->get_t4()));
        for(unsigned int i =61; i< globalDatagramSize; i++){
            byteArray[i] = '\0';
        }
        datagram = new lienaDatagram(globalDatagramSize, &byteArray);
    }
    return datagram;
}

//! -------------------------------------------------------------------------------------------------------------
//!
//! \brief lienaNTPSynchronizationTask::enable
//!
void lienaNTPSynchronizationTask::enable(){
    this->standby = false;
}

//! -------------------------------------------------------------------------------------------------------------
//!
//! \brief lienaNTPSynchronizationTask::freeze
//!
void lienaNTPSynchronizationTask::freeze(){
    this->standby = true;
}

//! -------------------------------------------------------------------------------------------------------------
//!
//! \brief lienaNTPSynchronizationTask::terminate
//!
void lienaNTPSynchronizationTask::terminate(){
    this->outputQueue->clear();
    this->flag = false;
}
