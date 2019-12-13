#include "lienaMotorInSpeedModeMessage.h"

lienaMotorInSpeedModeMessage::lienaMotorInSpeedModeMessage(){
    this->round = 0;
}

int lienaMotorInSpeedModeMessage::getRPM(){
    return rpm;
}

int lienaMotorInSpeedModeMessage::getRound(){
    return round;
}

int lienaMotorInSpeedModeMessage::getSymbol(){
    return symbol;
}

int lienaMotorInSpeedModeMessage::getMotorType(){
    return motorType;
}

void lienaMotorInSpeedModeMessage::setRPM(int rpm){
    this->rpm = rpm;
}

void lienaMotorInSpeedModeMessage::setSymbol(int symbol){
    this->symbol = symbol;
}

void lienaMotorInSpeedModeMessage::setMotorType(int motorType){
    this->motorType = motorType;
}

void lienaMotorInSpeedModeMessage::setRound(int round){
    this->round = round;
}
