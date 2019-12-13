#include "lienaInputQueue.h"

lienaInputQueue::lienaInputQueue()
{

}

//! ------------------------------------------------------------------------
//!
//! \brief lienaInputQueue::getLength
//! \return
//!
int lienaInputQueue::getLength(){
    int ret = 0;
    mutex.lock();
    ret = inputQueue.size();
    mutex.unlock();
    return ret;
}

//! ---------------------------------------------------------------
//!
//! \brief lienaOutputQueue::clear
//!
void lienaInputQueue::clear(){
    mutex.lock();
    this->inputQueue.clear();
    mutex.unlock();
}

//! ------------------------------------------------------------------------
//!
//! \brief lienaInputQueue::append
//! \param byteArray
//!
void lienaInputQueue::append(lienaDatagram *byteArray){
    mutex.lock();
    inputQueue.append(byteArray);
    mutex.unlock();
}

//! ------------------------------------------------------------------------
//!
//! \brief lienaInputQueue::getFrontDatagram
//! \return
//!
lienaDatagram *lienaInputQueue::getFrontDatagram(){
    lienaDatagram *ret = NULL;
    mutex.lock();
    ret = inputQueue.at(0);
    mutex.unlock();
    return ret;
}

//! ------------------------------------------------------------------------
//!
//! \brief lienaInputQueue::deleteLatestArray
//!
void lienaInputQueue::deleteFrontDatagram(){
    mutex.lock();
    inputQueue.pop_front();
    mutex.unlock();
}

//! ------------------------------------------------------------------------
//!
//! \brief lienaInputQueue::print
//!
void lienaInputQueue::print(){
//    for(int cpt = 0; cpt < inputQueue.size(); cpt++){
//        int array_size = inputQueue.at(cpt).size();

//        qDebug()<<"size:"<<array_size<<", value:"
//                <<(unsigned char)inputQueue.at(cpt).at(0)
//                <<(unsigned char)inputQueue.at(cpt).at(1)
//                <<(unsigned char)inputQueue.at(cpt).at(2)
//                <<(unsigned char)inputQueue.at(cpt).at(3)
//                <<(unsigned char)inputQueue.at(cpt).at(4)
//                <<(unsigned char)inputQueue.at(cpt).at(5)
//                <<(unsigned char)inputQueue.at(cpt).at(6)
//                <<(unsigned char)inputQueue.at(cpt).at(7)
//                <<(unsigned char)inputQueue.at(cpt).at(8)
//                <<(unsigned char)inputQueue.at(cpt).at(9);

//    }
}
