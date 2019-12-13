#include "lienaHandShakeMessage.h"


/**
 * @brief lienaHandShakeMessage::lienaHandShakeMessage
 */
lienaHandShakeMessage::lienaHandShakeMessage(uint64_t messageId,
                                             uint32_t targetId,
                                             uint32_t originId,
                                             uint64_t timestampes,
                                             uint32_t DLC,
                                             QString addr,
                                             uint16_t port) : lienaMessage(messageId, targetId, originId, timestampes, DLC){
    QStringList temp = addr.split(".");
    this->addr[0] = uchar(temp[0].toUShort());
    this->addr[1] = uchar(temp[1].toUShort());
    this->addr[2] = uchar(temp[2].toUShort());
    this->addr[3] = uchar(temp[3].toUShort());
    this->port = port;
}

//! -------------------------------------------------------------------------------------------------------------
//!
//! \brief lienaHandShakeMessage::getAddr
//! \return
//!
QString lienaHandShakeMessage::getIpAddresse(){
    QString addrs ;
    addrs.append(QString::number(int(addr[0])));
    addrs.append(".");
    addrs.append(QString::number(int(addr[1])));
    addrs.append(".");
    addrs.append(QString::number(int(addr[2])));
    addrs.append(".");
    addrs.append(QString::number(int(addr[3])));
    return addrs;
}

//! -------------------------------------------------------------------------------------------------------------
//!
//! \brief lienaHandShakeMessage::setAddr
//! \param address
//!
void lienaHandShakeMessage::setIpAddresse(QString address){
    QStringList addrs = address.split(".");
    addr[0] = uchar(addrs[0].toUShort());
    addr[1] = uchar(addrs[1].toUShort());
    addr[2] = uchar(addrs[2].toUShort());
    addr[3] = uchar(addrs[3].toUShort());
}

//! -------------------------------------------------------------------------------------------------------------
//!
//! \brief lienaHandShakeMessage::setAddr
//! \param addrZero
//! \param addrOne
//! \param addrTwo
//! \param addrThree
//!
void lienaHandShakeMessage::setIpAddresse(uchar addrZero, uchar addrOne, uchar addrTwo, uchar addrThree){
    addr[0] = addrZero;
    addr[1] = addrOne;
    addr[2] = addrTwo;
    addr[3] = addrThree;
}

//! -------------------------------------------------------------------------------------------------------------
//!
//! \brief lienaHandShakeMessage::getPort
//! \return
//!
uint16_t lienaHandShakeMessage::getPort(){
    return this->port;
}

//! -------------------------------------------------------------------------------------------------------------
//!
//! \brief lienaHandShakeMessage::setPort
//! \param port
//!
void lienaHandShakeMessage::setPort(uint16_t port){
    this->port = port;
}

//! -------------------------------------------------------------------------------------------------------------
//!
//! \brief lienaHandShakeMessage::setPort
//! \param portZero
//! \param portOne
//!
void lienaHandShakeMessage::setPort(uchar portZero, uchar portOne){
    this->port = portZero*256 + portOne;
}


