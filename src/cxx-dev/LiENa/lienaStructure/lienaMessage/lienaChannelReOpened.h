#ifndef LIENACHANNELREOPENED_H
#define LIENACHANNELREOPENED_H

#include "lienaMessage.h"

class lienaChannelReOpened  : public lienaMessage
{
public:
    lienaChannelReOpened(unsigned long long messageId,
                         unsigned int targetId,
                         unsigned int originId,
                         unsigned long long timestampes,
                         unsigned int DLC);
};

#endif // LIENACHANNELREOPENED_H
