#ifndef LIENACHANNELCLOSEDMESSAGE_H
#define LIENACHANNELCLOSEDMESSAGE_H

#include "lienaMessage.h"

class lienaChannelClosedMessage : public lienaMessage
{
public:
    lienaChannelClosedMessage(unsigned long long messageId,
                              unsigned int targetId,
                              unsigned int originId,
                              unsigned long long timestampes,
                              unsigned int DLC);
};

#endif // LIENACHANNELCLOSEDMESSAGE_H
