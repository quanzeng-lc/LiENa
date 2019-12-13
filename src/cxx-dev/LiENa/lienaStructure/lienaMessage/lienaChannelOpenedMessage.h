#ifndef CHANNELOPENEDMESSAGE_H
#define CHANNELOPENEDMESSAGE_H
#include "QString"
#include "QStringList"
#include "lienaMessage.h"


class lienaChannelOpenedMessage: public lienaMessage
{
public:
    QString getAddr();
    void  setAddr(QString address);
    void  setAddr(uchar addrZero, uchar addrOne, uchar addrTwo, uchar addrThree);

    void  setPort(unsigned short port);
    void  setPort(uchar portZero, uchar portOne);
    unsigned short getPort();

private:
    unsigned char addr[4];//!4字节
    unsigned short port;  //!2字节

public:
    lienaChannelOpenedMessage(unsigned long long messageId,
                                unsigned int targetId,
                                unsigned int originId,
                                unsigned long long timestampes,
                                unsigned int DLC);
};

#endif // CHANNELOPENEDMESSAGE_H
