#ifndef HANDSHAKEMESSAGE_H
#define HANDSHAKEMESSAGE_H

#include "lienaMessage.h"
#include "lienaDatagram.h"
#include <QByteArray>
#include <QString>
#include <QStringList>
#include <QDebug>


class lienaHandShakeMessage : public lienaMessage
{
public:
    QString getIpAddresse();
    void  setIpAddresse(QString address);
    void  setIpAddresse(uchar addrZero, uchar addrOne, uchar addrTwo, uchar addrThree);

    uint16_t getPort();
    void  setPort(uint16_t port);
    void  setPort(uchar portZero, uchar portOne);

private:
    uint8_t addr[4];//! 4byte
    uint16_t port;  //! 2byte

public:
    lienaHandShakeMessage(uint64_t messageId,
                          uint32_t targetId,
                          uint32_t originId,
                          uint64_t timestampes,
                          uint32_t DLC,
                          QString addr,
                          uint16_t port);
};

#endif // HANDSHAKEMESSAGE_H
