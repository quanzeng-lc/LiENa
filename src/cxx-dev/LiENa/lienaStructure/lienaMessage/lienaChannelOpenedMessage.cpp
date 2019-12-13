#include "lienaChannelOpenedMessage.h"

/**
 * @brief channelOpenedMessage::channelOpenedMessage
 */
lienaChannelOpenedMessage::lienaChannelOpenedMessage(unsigned long long messageId,
                                                     unsigned int targetId,
                                                     unsigned int originId,
                                                     unsigned long long timestampes,
                                                     unsigned int DLC) : lienaMessage(messageId, targetId,originId, timestampes, DLC)
{

}

//! ----------------------------------------------------------------------------------------------------
//!
//! \brief lienaChannelOpenedMessage::getAddr
//! \return
//!
QString lienaChannelOpenedMessage::getAddr(){
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
void lienaChannelOpenedMessage::setAddr(QString address){
    QStringList addrs = address.split(".");
    addr[0] = (uchar)addrs[0].toInt();
    addr[1] = (uchar)addrs[1].toInt();
    addr[2] = (uchar)addrs[2].toInt();
    addr[3] = (uchar)addrs[3].toInt();
}

//!
//! \brief lienaChannelOpenedMessage::setAddr
//! \param addrZero
//! \param addrOne
//! \param addrTwo
//! \param addrThree
//!
void lienaChannelOpenedMessage::setAddr(uchar addrZero, uchar addrOne, uchar addrTwo, uchar addrThree){
    addr[0] = addrZero;
    addr[1] = addrOne;
    addr[2] = addrTwo;
    addr[3] = addrThree;
}

//!
//! \brief lienaChannelOpenedMessage::getPort
//! \return
//!
unsigned short lienaChannelOpenedMessage::getPort(){
    return this->port;
}

//!
//! \brief lienaChannelOpenedMessage::setPort
//! \param port
//!
void lienaChannelOpenedMessage::setPort(unsigned short port){
    this->port = port;
}

//!
//! \brief lienaChannelOpenedMessage::setPort
//! \param portZero
//! \param portOne
//!
void lienaChannelOpenedMessage::setPort(uchar portZero, uchar portOne){
    this->port = (int)(portZero + portOne*256);
}
