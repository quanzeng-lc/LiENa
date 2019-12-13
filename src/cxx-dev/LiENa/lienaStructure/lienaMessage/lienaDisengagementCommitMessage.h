#ifndef LIENADISENGAGEMENTCOMMITMESSAGE_H
#define LIENADISENGAGEMENTCOMMITMESSAGE_H

#include "lienaMessage.h"

class lienaDisengagementCommitMessage : public lienaMessage
{
public:
    lienaDisengagementCommitMessage(uint64_t messageId,
                                    uint32_t targetId,
                                    uint32_t originId,
                                    uint64_t  timestampes,
                                    uint32_t DLC);
    ~lienaDisengagementCommitMessage();
};

#endif // LIENADISENGAGEMENTCOMMITMESSAGE_H
