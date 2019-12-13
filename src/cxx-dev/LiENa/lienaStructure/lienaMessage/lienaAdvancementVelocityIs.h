#ifndef ADVANCMENTTARGETVELOCITY_H
#define ADVANCMENTTARGETVELOCITY_H

#include "lienaDatagram.h"
#include "lienaMessage.h"

class lienaAdvancementTargetVelocity : public lienaMessage
{
private:
    int targetVelocity; // motor's rpm value
    bool symbol; // +/-
    int motorType; // 0: progress, 1: rotate
public:
    int getTargetVelocity();
    bool getSymbol();
    int getMotorType();

    void setTargetVelocity(int targetVelocity);
    void setSymbol(bool symbol);
    void setMotorType(int motorType);

    void transformIgtdatagramToAdvancementTargetVelocity(lienaDatagram* datagram);

public:
    lienaAdvancementTargetVelocity(unsigned int messageId,
                                   unsigned int targetId,
                                   unsigned int timestampes,
                                   unsigned int DLC);
};

#endif // ADVANCMENTTARGETVELOCITY_H
