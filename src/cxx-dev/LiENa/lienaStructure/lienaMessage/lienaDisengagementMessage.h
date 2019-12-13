#ifndef LIENADISENGAGEMENTMESSAGE_H
#define LIENADISENGAGEMENTMESSAGE_H

#include "lienaMessage.h"


class lienaDisengagementMessage : public lienaMessage
{
public:
    lienaDisengagementMessage(unsigned long long messageId,
                              unsigned int targetId,
                              unsigned int originId,
                              unsigned long long timestampes,
                              unsigned int DLC);
    ~lienaDisengagementMessage();
};

#endif // LIENADISENGAGEMENTMESSAGE_H
