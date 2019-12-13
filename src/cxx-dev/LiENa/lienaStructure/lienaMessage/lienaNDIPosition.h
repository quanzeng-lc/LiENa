#ifndef LIENANDIPOSITION_H
#define LIENANDIPOSITION_H
#include "lienaDatagram.h"

class lienaNDIPosition
{
private:
    double positionX;
    double positionY;
    double positionZ;

    long timestamps;

public:
    void setPositionX(double positionX);
    double getPositionX();

    void setPositionY(double positionY);
    double getPositionY();

    void setPositionZ(double positionZ);
    double getPositionZ();

    void setTimestamps(long timestamps);
    long getTimestamps();

    void transformIgtdatagramToNDIPosition(lienaDatagram* datagram);
public:
    lienaNDIPosition();
};

#endif // IGTNDIPOSITION_H
