#include "lienaOutputQueue.h"


/**
 * @brief lienaOutputQueue::lienaOutputQueue
 */
lienaOutputQueue::lienaOutputQueue(){
    outputQueue.clear();
}

//! ---------------------------------------------------------------
//!
//! \brief lienaOutputQueue::~lienaOutputQueue
//!
lienaOutputQueue::~lienaOutputQueue(){
    delete &outputQueue;
}

//! ---------------------------------------------------------------
//!
//! \brief lienaOutputQueue::clear
//!
void lienaOutputQueue::clear(){
    mutex.lock();
    this->outputQueue.clear();
    mutex.unlock();
}

//! ----------------------------------------------------------------
//!
//! \brief lienaOutputQueue::append
//! \param sendMessage
//!
void lienaOutputQueue::append(lienaDatagram *sendMessage){
    mutex.lock();
    this->outputQueue.append(sendMessage);
    mutex.unlock();
}

//! -----------------------------------------------------------------
//!
//! \brief lienaOutputQueue::getFrontDatagram
//! \return
//!
lienaDatagram *lienaOutputQueue::getFrontDatagram(){
    lienaDatagram *ret = nullptr;
    mutex.lock();
    ret = this->outputQueue[0];
    mutex.unlock();
    return ret;
}

//! ------------------------------------------------------------------
//!
//! \brief lienaOutputQueue::deleteLatestFrame
//!
void lienaOutputQueue::deleteFrontDatagram(){
    mutex.lock();
    if(outputQueue.length() > 0){
        this->outputQueue.removeAt(0);
    }
    mutex.unlock();
}

//! ------------------------------------------------------------------
//!
//! \brief lienaOutputQueue::getOutputQueueLength
//! \return
//!
int lienaOutputQueue::getOutputQueueLength(){
    int ret;
    mutex.lock();
    ret = this->outputQueue.size();
    mutex.unlock();
    return ret;
}
