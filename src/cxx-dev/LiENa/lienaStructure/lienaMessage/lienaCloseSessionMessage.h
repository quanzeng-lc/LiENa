#ifndef IGTCLOSESESSIONMESSAGE_H
#define IGTCLOSESESSIONMESSAGE_H

#include "lienaMessage.h"

class lienaCloseSessionMessage: public lienaMessage
{
private:
    int close_mode;
    int close_time;

public:
    lienaCloseSessionMessage(unsigned long long messageId,
                             unsigned int targetId,
                             unsigned int originId,
                             unsigned int timestampes,
                             unsigned int DLC);
};

#endif // IGTCLOSESESSIONMESSAGE_H
