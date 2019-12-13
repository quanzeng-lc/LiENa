#ifndef HANDSHAKECOMMITMESSAGE_H
#define HANDSHAKECOMMITMESSAGE_H
#include <QString>
#include <QStringList>

#include "lienaMessage.h"


class lienaHandShakeCommitMessage: public lienaMessage
{
public:
    QString getAddr();
    void  setAddr(QString address);
    void  setAddr(uchar addrZero, uchar addrOne, uchar addrTwo, uchar addrThree);
    unsigned short   getPort();
    void  setPort(uint16_t port);
    void  setPort(uchar portZero, uchar portOne);

private:
    uint8_t addr[4];//!4字节
    uint16_t port;  //!2字节

public:
    lienaHandShakeCommitMessage(uint64_t messageId,
                                uint32_t targetId,
                                uint32_t originId,
                                uint64_t timestampes,
                                uint32_t DLC);
};

#endif // HANDSHAKECOMMITMESSAGE_H
