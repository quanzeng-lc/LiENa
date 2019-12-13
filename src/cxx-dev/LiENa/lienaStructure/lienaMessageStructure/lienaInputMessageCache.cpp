#include "lienaInputMessageCache.h"


/**
 * @brief lienaOutputMessageCache::lienaOutputMessageCache
 */
lienaInputMessageCache::lienaInputMessageCache()
{
    this->echantionnage = false;

    this->messageSequence = new QVector<lienaMessageQueue*>();
}

//! ----------------------------------------------------------------------------------------------------
//!
//! \brief lienaInputMessageCache::exist
//! \param deviceID
//! \return
//!
bool lienaInputMessageCache::exist(uint32_t deviceID){
    bool ret = false;
    for(int i = 0 ; i < this->messageSequence->size(); i++){
        if(this->messageSequence->at(i)->getDeviceId() == deviceID){
            ret = true;
            break;
        }
    }
    return ret;
}

//! ----------------------------------------------------------------------------------------------------
//!
//! \brief lienaOutputMessageCache::writeMessageByIndex
//! \param index
//!
void lienaInputMessageCache::writeMessageByIndex(int index, lienaCustomizedMessage*msg){
   this->messageSequence->at(index)->append(msg);
}

//! ----------------------------------------------------------------------------------------------------
//!
//! \brief lienaOutputMessageCache::generateNewMessageSequence
//! \return
//!
lienaMessageQueue * lienaInputMessageCache::generateNewMessageSequence(uint32_t deviceID){
    lienaMessageQueue *msgQ = new lienaMessageQueue();
    msgQ->setDeviceId(deviceID);
    this->messageSequence->append(msgQ);
    return  msgQ;
}

//! ----------------------------------------------------------------------------------------------------
//!
//! \brief lienaOutputMessageCache::clearAllBuffers
//!
void lienaInputMessageCache::clearAllBuffers(){
    messageSequence->clear();
}
