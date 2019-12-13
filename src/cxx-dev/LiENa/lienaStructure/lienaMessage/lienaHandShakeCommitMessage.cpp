#include "lienaHandShakeCommitMessage.h"


lienaHandShakeCommitMessage::lienaHandShakeCommitMessage(uint64_t messageId,
                                                         uint32_t targetId,
                                                         uint32_t originId,
                                                         uint64_t timestampes,
                                                         uint32_t DLC) : lienaMessage(messageId, targetId, originId, timestampes, DLC)
{

}

//! ----------------------------------------------------------------------------------------------------
//!
//! \brief lienaHandShakeCommitMessage::getAddr
//! \return
//!
QString lienaHandShakeCommitMessage::getAddr(){
    QString addrs ;
    addrs.append(QString::number((int)addr[0]));
    addrs.append(".");
    addrs.append(QString::number((int)addr[1]));
    addrs.append(".");
    addrs.append(QString::number((int)addr[2]));
    addrs.append(".");
    addrs.append(QString::number((int)addr[3]));
    return addrs;
}

//! ------------------------------------------------------------------------------------------------------
//!
//! \brief lienaHandShakeCommitMessage::setAddr
//! \param address
//!
void lienaHandShakeCommitMessage::setAddr(QString address){
    QStringList addrs = address.split(".");
    addr[0] = (uchar)addrs[0].toInt();
    addr[1] = (uchar)addrs[1].toInt();
    addr[2] = (uchar)addrs[2].toInt();
    addr[3] = (uchar)addrs[3].toInt();
}

void lienaHandShakeCommitMessage::setAddr(uchar addrZero, uchar addrOne, uchar addrTwo, uchar addrThree){
    addr[0] = addrZero;
    addr[1] = addrOne;
    addr[2] = addrTwo;
    addr[3] = addrThree;
}

unsigned short lienaHandShakeCommitMessage::getPort(){
    return this->port;
}

void lienaHandShakeCommitMessage::setPort(unsigned short port){
    this->port = port;
}

void lienaHandShakeCommitMessage::setPort(uchar portZero, uchar portOne){
    this->port = (int)(portZero + portOne*256);
}
