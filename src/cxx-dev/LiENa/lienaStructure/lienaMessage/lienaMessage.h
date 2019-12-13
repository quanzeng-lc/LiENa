#ifndef LIENAMESSAGE_H
#define LIENAMESSAGE_H

#include <QDebug>
#include "math.h"

class lienaMessage
{

public:

    void init();
    void printHeader();

    void setMessageId(uint64_t  messageId);
    void setTargetId(uint32_t targetId);
    void setTimestamps(uint64_t  timestampes);
    void setDLC(uint32_t dlc);

    void setOriginId(uint32_t originId);
    uint32_t getOriginId();

    uint64_t  getMessageId();
    uint32_t getTargetId();
    uint64_t  getTimestampes();
    uint32_t getDLC();
    uint32_t getMessageType();

    uint32_t getCustomerizedMessageId();
    uint8_t getDeviceClass();

    uint32_t getManufacture();
    uint8_t getDeviceType();
    uint8_t getDeviceVision();
    uint8_t getDeviceIndex();

private:
    uint64_t messageId;         //! 64 bits
    uint32_t targetId;          //! 32 bits
    uint32_t originId;          //! 32 bits
    uint64_t timestampes;       //! 32 bits
    uint32_t DLC;               //! 32 bits

public:
    lienaMessage(uint64_t messageId, uint32_t targetId, uint32_t originId, uint64_t timestampes, uint32_t DLC);
    ~lienaMessage();
};

#endif // LIENAMESSAGE_H
