#include "lienaMessage.h"


/**
 * @brief lienaMessage::lienaMessage
 * @param messageId
 * @param targetId
 * @param timestampes
 * @param DLC
 */
lienaMessage::lienaMessage(uint64_t messageId, uint32_t targetId, uint32_t originId, uint64_t timestampes, uint32_t DLC)
{
    this->messageId = messageId;
    this->targetId = targetId;
    this->originId = originId;
    this->timestampes = timestampes;
    this->DLC = DLC;
}

//! ------------------------------------------------------------------------------------------------------------
//!
//! \brief lienaMessage::~lienaMessage
//!
lienaMessage::~lienaMessage(){
    delete &messageId;
    delete &targetId;
    delete &originId;
    delete &timestampes;
    delete &DLC;
}

//! ----------------------------------------------------------------------
//!
//! \brief lienaMessage::getOrigineId
//! \return
//!
uint32_t lienaMessage::getOriginId(){
    return this->originId;
}

//!
//! \brief lienaMessage::setOriginId
//! \param originId
//!
void lienaMessage::setOriginId(uint32_t originId){
    this->originId  = originId;
}

//! ---------------------------------------------------------------------
//!
//! \brief lienaMessage::printHeader
//!
void lienaMessage::printHeader(){
    qDebug()<<"header"<<this->messageId<<this->targetId<<this->timestampes<<this->DLC;
}

//! ---------------------------------------------------------------------
//!
//! \brief lienaMessage::init
//!
void lienaMessage::init(){
    this->messageId = 0;
    this->targetId = 0;
    this->timestampes = 0;
    this->DLC = 0;
}

//! ----------------------------------------------------------------------
//!
//! \brief lienaMessage::set_message_id
//! \param message_id
//!
void lienaMessage::setMessageId(uint64_t  message_id){
    this->messageId = message_id;
}

//! ----------------------------------------------------------------------
//!
//! \brief lienaMessage::getMessageType
//! \return
//!
//uint32_t lienaMessage::getMessageType(){
//    return uint32_t(this->messageId % (2**32));
//}

//! ----------------------------------------------------------------------
//!
//! \brief lienaMessage::set_target_id
//! \param target_id
//!
void lienaMessage::setTargetId(uint32_t target_id){
    this->targetId = target_id;
}

//! ----------------------------------------------------------------------
//!
//! \brief lienaMessage::getCustomerizedMessageId
//! \return
//!
uint32_t lienaMessage:: getCustomerizedMessageId(){
    return this->messageId % (uint64_t)pow(256, 4);
}

//! ----------------------------------------------------------------------
//!
//! \brief lienaMessage::getDeviceClass
//! \return 3bits from 29-31
//!
uint8_t lienaMessage:: getDeviceClass(){
    return uint8_t(this->getOriginId()/(256*256*256*32));
}

//! ----------------------------------------------------------------------
//!
//! \brief lienaMessage::getManufacture
//! \return 14bits for 15-28
//!
uint32_t lienaMessage:: getManufacture(){
    return uint32_t((this->getOriginId() % (256*256*256*32)) / (256*128));
}

//! ----------------------------------------------------------------------
//!
//! \brief lienaMessage::getDeviceType
//! \return 6bits from 9-14
//!
uint8_t lienaMessage:: getDeviceType(){
    return uint8_t((this->getOriginId() % (256*128)) / (256*2));

}

//! ----------------------------------------------------------------------
//!
//! \brief lienaMessage::getDeviceVision
//! \return 6bits from 3-8
//!
uint8_t lienaMessage:: getDeviceVision(){
    return uint8_t((this->getOriginId() % (256*2)) /8);

}

//! ----------------------------------------------------------------------
//!
//! \brief lienaMessage::getDeviceIndex
//! \return 3bits for 0-2
//!
uint8_t lienaMessage:: getDeviceIndex(){
    return uint32_t(this->getOriginId()%8);

}

//! ----------------------------------------------------------------------
//!
//! \brief lienaMessage::get_message_id
//! \return
//!
uint64_t lienaMessage::getMessageId(){
    return this->messageId;
}

//! ----------------------------------------------------------------------
//!
//! \brief lienaMessage::get_target_id
//! \return
//!
uint32_t lienaMessage::getTargetId(){
    return this->targetId;
}

//! ----------------------------------------------------------------------
//!
//! \brief lienaMessage::getTimestampes
//! \return
//!
uint64_t lienaMessage::getTimestampes(){
    return this->timestampes;
}

//! ----------------------------------------------------------------------
//!
//! \brief lienaMessage::getDLC
//! \return
//!
uint32_t lienaMessage::getDLC(){
    return this->DLC;
}
