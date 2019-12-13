#include "lienaDatagram.h"


/**
 * @brief lienaDatagram::lienaDatagram
 */
lienaDatagram::lienaDatagram(uint32_t size, char** chunk){
    this->size = size;
    this->byteArray = *chunk;
}

//! -----------------------------------------------------------------------------------------------------------------------
//!
//! \brief lienaDatagram::set_size
//! \param size
//!
void lienaDatagram::setSize(int size){
    this->size = size;
}

//! -----------------------------------------------------------------------------------------------------------------------
//!
//! \brief lienaDatagram::getDataType
//! \return
//!
uint64_t lienaDatagram::getMessageID(){
    return (uint64_t(byteArray[0]))*256*256*256*256*256*256*256  +
           (uint64_t(byteArray[1]))*256*256*256*256*256*256  +
           (uint64_t(byteArray[2]))*256*256*256*256*256  +
           (uint64_t(byteArray[3]))*256*256*256*256 +
           (uint64_t(byteArray[4]))*256*256*256 +
           (uint64_t(byteArray[5]))*256*256 +
           (uint64_t(byteArray[6]))*256 +
           (uint64_t(byteArray[7]));
}

//! -----------------------------------------------------------------------------------------------------------------------
//!
//! \brief lienaDatagram::getOrigineId
//! \return
//!
uint32_t lienaDatagram::getOriginId(){
    return  (uint32_t(byteArray[12]))*256*256*256  +
            (uint8_t(byteArray[13]))*256*256  +
            (uint8_t(byteArray[14]))*256 +
            (uint8_t(byteArray[15]));
}

//! -----------------------------------------------------------------------------------------------------------------------
//!
//! \brief lienaDatagram::writeValueInFiveBytes
//! \param start
//! \param value
//!
void lienaDatagram::writeValueInEightBytes(int start, uint64_t value){
    byteArray[start]     = (value&0xff00000000000000)>>56;
    byteArray[start + 1] = (value&0x00ff000000000000)>>48;
    byteArray[start + 2] = (value&0x0000ff0000000000)>>40;
    byteArray[start + 3] = (value&0x000000ff00000000)>>32;
    byteArray[start + 4] = (value&0x00000000ff000000)>>24;
    byteArray[start + 5] = (value&0x0000000000ff0000)>>16;
    byteArray[start + 6] = (value&0x000000000000ff00)>>8;
    byteArray[start + 7] = (value&0x00000000000000ff);
}

//! -----------------------------------------------------------------------------------------------------------------------
//!
//! \brief lienaDatagram::getDeviceClass
//! \return
//!
uint8_t lienaDatagram::getDeviceClass(){
    return uint8_t(this->getOriginId() / (256*256*256*32));
}

//! -----------------------------------------------------------------------------------------------------------------------
//!
//! \brief lienaDatagram::getManufacture
//! \return
//!
uint32_t lienaDatagram::getManufacture(){
    return uint32_t((this->getOriginId() % (256*256*256*32)) / (256*128));
}

//! -----------------------------------------------------------------------------------------------------------------------
//!
//! \brief lienaDatagram::getDeviceType
//! \return
//!
uint8_t lienaDatagram::getDeviceType(){
    return uint8_t((this->getOriginId() % (256*128)) / (256*2));
}

//! -----------------------------------------------------------------------------------------------------------------------
//!
//! \brief lienaDatagram::getDeviceVision
//! \return
//!
uint8_t lienaDatagram::getDeviceVision(){
    return uint8_t((this->getOriginId() %  (256*2)) /8);
}

//! -----------------------------------------------------------------------------------------------------------------------
//!
//! \brief lienaDatagram::getDeviceIndex
//! \return
//!
uint8_t lienaDatagram::getDeviceIndex(){
    return uint8_t(this->getOriginId() % 8);

}

//! -----------------------------------------------------------------------------------------------------------------------
//!
//! \brief lienaDatagram::getTargetId
//! \return
//!
uint32_t lienaDatagram::getTargetId(){
    return (uint32_t(byteArray[8]))*256*256*256 +
           (uint32_t(byteArray[9]))*256*256 +
           (uint32_t(byteArray[10]))*256 +
           (uint32_t(byteArray[11]));
}

//! -----------------------------------------------------------------------------------------------------------------------
//!
//! \brief lienaDatagram::getTimeStampes
//! \return
//!
uint32_t lienaDatagram::getTimeStampes(){
    return uint8_t(byteArray[16])*256*256*256*256*256*256*256  +
           uint8_t(byteArray[17])*256*256*256*256*256*256 +
           uint8_t(byteArray[18])*256*256*256*256*256 +
           uint8_t(byteArray[19])*256*256*256*256 +
           uint8_t(byteArray[20])*256*256*256 +
           uint8_t(byteArray[21])*256*256 +
           uint8_t(byteArray[22])*256 +
           uint8_t(byteArray[23]);
}

//! -----------------------------------------------------------------------------------------------------------------------
//!
//! \brief lienaDatagram::getDLC
//! \return
//!
uint32_t lienaDatagram::getDLC(){
    return uint8_t(byteArray[24])*256*256*256 +
           uint8_t(byteArray[25])*256*256 +
           uint8_t(byteArray[26])*256 +
           uint8_t(byteArray[27]);
}

//! -----------------------------------------------------------------------------------------------------------------------
//!
//! \brief lienaDatagram::getIgtDatagramBody
//! \return
//!
char* lienaDatagram::getBody(){
    char *body = new char[size-HEADER_SIZE];
    strncpy(body, this->byteArray + HEADER_SIZE, this->size - HEADER_SIZE - 1);
    return body;
}

//! -----------------------------------------------------------------------------------------------------------------------
//!
//! \brief lienaDatagram::getByteArray
//! \return
//!
char** lienaDatagram::getByteArray(){
    return &this->byteArray;
}

//! -----------------------------------------------------------------------------------------------------------------------
//!
//! \brief igtDatagram::print
//!
QString lienaDatagram::print(){
    QString print_str;
    for(int i = 0; i < 26; i++){
        print_str+= " " + QString::number((uint8_t)this->byteArray[i]);
    }
    return print_str;
}
