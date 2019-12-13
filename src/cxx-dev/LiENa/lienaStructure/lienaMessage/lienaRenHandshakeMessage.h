#ifndef LIENARENHANDSHAKEMESSAGE_H
#define LIENARENHANDSHAKEMESSAGE_H

#include "lienaMessage.h"


class lienaReHandshakeMessage : public lienaMessage
{


public:
    lienaReHandshakeMessage(unsigned long long messageId,
                             unsigned int targetId,
                            unsigned int originId,
                             unsigned long long timestampes,
                             unsigned int DLC);
};

#endif // LIENARENHANDSHAKEMESSAGE_H
