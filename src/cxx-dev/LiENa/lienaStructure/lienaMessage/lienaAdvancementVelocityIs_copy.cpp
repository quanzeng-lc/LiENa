#include "lienaAdvancementVelocityIs.h"


lienaAdvancementTargetVelocity::lienaAdvancementTargetVelocity(unsigned int messageId,
                                                               unsigned int targetId,
                                                               unsigned int timestampes,
                                                               unsigned int DLC) : lienaMessage(messageId, targetId, timestampes, DLC)
{

}

int lienaAdvancementTargetVelocity::getTargetVelocity(){
    return targetVelocity;
}

bool lienaAdvancementTargetVelocity::getSymbol(){
    return symbol;
}

int lienaAdvancementTargetVelocity::getMotorType(){
    return motorType;
}

void lienaAdvancementTargetVelocity::setTargetVelocity(int targetVelocity){
    this->targetVelocity = targetVelocity;
}

void lienaAdvancementTargetVelocity::setSymbol(bool symbol){
    this->symbol = symbol;
}

void lienaAdvancementTargetVelocity::setMotorType(int motorType){
    this->motorType = motorType;
}

void lienaAdvancementTargetVelocity::transformIgtdatagramToAdvancementTargetVelocity(lienaDatagram* datagram){
    char* body = datagram->getBody();
    this->setMotorType((int)body[0]);
    this->setSymbol((int)body[1]);
    this->setTargetVelocity(((int)body[2]) + ((int)body[3]*256));
    return;
}
