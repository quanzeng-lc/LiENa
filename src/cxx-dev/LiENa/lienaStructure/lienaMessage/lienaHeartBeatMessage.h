#ifndef LIENAHEARTBEATMESSAGE_H
#define LIENAHEARTBEATMESSAGE_H
#include "lienaMessage.h"


class lienaHeartBeatMessage : public lienaMessage
{


public:
    lienaHeartBeatMessage(unsigned long long messageId,
                          unsigned int targetId,
                          unsigned int originId,
                          unsigned int timestampes,
                          unsigned int DLC);
};

#endif // LIENAHEARTBEATMESSAGE_H
