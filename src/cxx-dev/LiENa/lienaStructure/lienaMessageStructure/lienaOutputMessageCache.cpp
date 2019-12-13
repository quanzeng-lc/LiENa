#include "lienaOutputMessageCache.h"


/**
 * @brief lienaOutputMessageCache::lienaOutputMessageCache
 */
lienaOutputMessageCache::lienaOutputMessageCache()
{
    this->echantionnage = false;

    this->messageSequence = new QVector<lienaMessageQueue*>();
}

//! ----------------------------------------------------------------------------------------------------
//!
//! \brief lienaOutputMessageCache::exist
//! \param deviceID
//! \return
//!
bool lienaOutputMessageCache::exist(uint32_t deviceID){
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
void lienaOutputMessageCache::writeMessageByIndex(int index, lienaCustomizedMessage*msg){
    if(this->messageSequence->size() > 0){
        this->messageSequence->at(index)->append(msg);
    }
}

//! ----------------------------------------------------------------------------------------------------
//!
//! \brief lienaOutputMessageCache::generateNewMessageSequence
//! \return
//!
lienaMessageQueue *lienaOutputMessageCache::generateNewMessageSequence(uint32_t deviceID){
    lienaMessageQueue *msgQ = new lienaMessageQueue();
    msgQ->setDeviceId(deviceID);
    this->messageSequence->append(msgQ);
    return msgQ;
}

//! ----------------------------------------------------------------------------------------------------
//!
//! \brief lienaOutputMessageCache::clearAllBuffers
//!
void lienaOutputMessageCache::clearAllBuffers(){
    messageSequence->clear();
}
