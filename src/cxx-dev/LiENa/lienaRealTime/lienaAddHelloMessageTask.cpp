#include "lienaAddHelloMessageTask.h"

lienaAddHelloMessageTask::lienaAddHelloMessageTask(int index)
{
    this->flag = true;
    this->index = index;
}

//! -----------------------------------------------------------------------------------------------------
//!
//! \brief igtAddHelloMessageTask::run
//!
void lienaAddHelloMessageTask::run(){
    while (this->flag) {
//        lienaHelloMessage *helloMessage = new lienaHelloMessage();
//        helloMessage->setCount(this->index+1);
//        helloMessage->setConnectionState(24);
//        lienaDatagram *datagram = helloMessage->transformHelloMessageToIgtDatagram();
//        outputQueueManager->setMessageToSendByTargetId(this->index, datagram);

        sleep(4);
    }
    return;
}
