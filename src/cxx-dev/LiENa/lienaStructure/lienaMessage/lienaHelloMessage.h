#ifndef HELLOMESSAGE_H
#define HELLOMESSAGE_H

#include "lienaMessage.h"

class lienaHelloMessage: public lienaMessage{

private:
    long count;
    int connectionState;

public:
    void setCount(long count);
    long getCount();

    void setConnectionState(int connectionState);
    int getConnectionState();

public:
    lienaHelloMessage(unsigned int messageId,
                      unsigned int targetId,
                      unsigned int originId,
                      unsigned long long  timestampes,
                      unsigned int DLC);
    ~lienaHelloMessage();

};

#endif // HELLOMESSAGE_H
