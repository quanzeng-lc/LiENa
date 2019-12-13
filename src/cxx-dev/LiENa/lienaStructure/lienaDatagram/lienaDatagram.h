#ifndef IGTDATAGRAM_H
#define IGTDATAGRAM_H

#include "lienaDefinition.h"
#include "lienaGlobal.h"
#include <QDebug>


class lienaDatagram{

private:
    uint32_t size;
    char *byteArray;

public:
    uint64_t getMessageID();
    uint32_t getTargetId();
    uint32_t getTimeStampes();
    uint32_t getDLC();

    //! sub information in message id
    uint32_t getOriginId();
    uint8_t getDeviceClass();
    uint32_t getManufacture();
    uint8_t getDeviceType();
    uint8_t getDeviceVision();
    uint8_t getDeviceIndex();

    void writeValueInEightBytes(int start, uint64_t value);

    char* getBody();
    char** getByteArray();

    void setSize(int size);
    QString print();

public:
    lienaDatagram(uint32_t size, char** byte_array);
};

#endif // IGTDATAGRAM_H
