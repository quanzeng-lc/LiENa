#ifndef LIENAREHANDSHAKECOMMITMESSAGE_H
#define LIENAREHANDSHAKECOMMITMESSAGE_H

#include "lienaMessage.h"


class lienaReHandshakeCommitMessage : public lienaMessage
{


public:
    lienaReHandshakeCommitMessage(unsigned long long messageId,
                             unsigned int targetId,
                                  unsigned int originId,
                             unsigned long long timestampes,
                             unsigned int DLC);
};

#endif // LIENAREHANDSHAKECOMMITMESSAGE_H
