#include "lienaHelloMessage.h"

lienaHelloMessage::lienaHelloMessage(unsigned int messageId,
                                     unsigned int targetId,
                                     unsigned int originId,
                                     unsigned long long timestampes,
                                     unsigned int DLC) : lienaMessage(messageId, targetId, originId, timestampes, DLC)
{

}

void lienaHelloMessage::setCount(long count){
    this->count = count;
    return;
}

long lienaHelloMessage::getCount(){
    long ret = this->count;
    return ret;
}

void lienaHelloMessage::setConnectionState(int connectionState){
    this->connectionState = connectionState;
    return;
}

int lienaHelloMessage::getConnectionState(){
    int ret = this->connectionState;
    return ret;
}
