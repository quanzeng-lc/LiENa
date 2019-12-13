#include "lienaMessageQueue.h"


/**
 * @brief lienaMessageQueue::lienaMessageQueue
 */
lienaMessageQueue::lienaMessageQueue()
{
    this->deviceID = 0;
    this->messageSequence = new QVector<lienaCustomizedMessage*>();
}

//! -----------------------------------------------------------------------------------------
//!
//! \brief lienaMessageQueue::setDeviceId
//! \param deviceID
//!
void lienaMessageQueue::setDeviceId(uint32_t deviceID){
    this->deviceID = deviceID;
}

//! -----------------------------------------------------------------------------------------
//!
//! \brief lienaMessageQueue::getDeviceId
//! \return
//!
uint32_t lienaMessageQueue::getDeviceId(){
    return this->deviceID;
}

//! ------------------------------------------------------------------------------------------
//!
//! \brief lienaMessageQueue::clear
//!
void lienaMessageQueue::clear(){
    mutex.lock();
    messageSequence->clear();
    mutex.unlock();
}

//! ------------------------------------------------------------------------------------------
//!
//! \brief lienaMessageQueue::isEmpty
//! \return
//!
bool lienaMessageQueue::isEmpty(){
    bool ret = false;
    mutex.lock();
    if(messageSequence->size()==0){
        ret = true;
    }
    mutex.unlock();
    return ret;
}

//! ------------------------------------------------------------------------------------------
//!
//! \brief lienaMessageQueue::append
//! \param msg
//!
void lienaMessageQueue::append(lienaCustomizedMessage*msg){
    mutex.lock();
    messageSequence->append(msg);
    mutex.unlock();
}

//! ------------------------------------------------------------------------------------------
//!
//! \brief lienaMessageQueue::getSize
//! \return
//!
int lienaMessageQueue::getSize(){
    int ret = -1;
    mutex.lock();
    ret = messageSequence->size();
    mutex.unlock();
    return ret;
}

//! ------------------------------------------------------------------------------------------
//!
//! \brief lienaMessageQueue::pop_front
//! \return
//!
lienaCustomizedMessage* lienaMessageQueue::pop_front(){
    lienaCustomizedMessage* ret = nullptr;
    mutex.lock();
    ret = messageSequence->at(0);
    messageSequence->removeFirst();
    mutex.unlock();
    return ret;
}

//! ------------------------------------------------------------------------------------------
//!
//! \brief lienaMessageQueue::pop_back
//! \return
//!
lienaCustomizedMessage* lienaMessageQueue::pop_back(){
    lienaCustomizedMessage* ret = nullptr;
    mutex.lock();
    ret = messageSequence->at(getSize()-1);
    messageSequence->removeLast();
    mutex.unlock();
    return ret;
}
