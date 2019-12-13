#include "lienaCustomizedMessage.h"


/**
 * @brief lienaCustomizedMessage::lienaCustomizedMessage
 * @param messageId
 * @param targetId
 * @param originId
 * @param timestampes
 * @param DLC
 */
lienaCustomizedMessage::lienaCustomizedMessage(uint64_t messageId,
                                               uint32_t targetId,
                                               uint32_t originId,
                                               uint64_t  timestampes,
                                               uint32_t DLC) : lienaMessage(messageId, targetId, originId, timestampes, DLC)
{
    this->index = 0;
    this->message_body = nullptr;
}

//! -------------------------------------------------------------------------------------------------------------------------
//!
//! \brief lienaCustomizedMessage::get_message_body
//! \return
//!
uint8_t* lienaCustomizedMessage::get_message_body(){
    return message_body;
}

//! -------------------------------------------------------------------------------------------------------------------------
//!
//! \brief lienaCustomizedMessage::define_body_length
//! \param length
//!
void lienaCustomizedMessage::define_body_length(uint32_t length){
    this->message_body = (uint8_t *)malloc(length * sizeof(uint8_t));

    for(uint32_t i =0; i< length; i++){
        this->message_body[i] = 0;
    }
}

//! -------------------------------------------------------------------------------------------------------------------------
//!
//! \brief lienaCustomizedMessage::append_uint8
//! \param uint8
//!
void lienaCustomizedMessage::append_uint8(uint8_t v){
    //qDebug()<<this->index<<v;
    this->message_body[this->index] = v;
    this->index += 1;
}

//! --------------------------------------------------------------------------------------------------------------------------
//!
//! \brief append_uint16
//! \param v
//!
void lienaCustomizedMessage::append_uint16(uint16_t v){
    this->message_body[this->index]   = uint8_t(0xff00&v)>>8;
    this->message_body[this->index+1] = uint8_t(0x00ff&v);
    this->index += 2;
}

//! --------------------------------------------------------------------------------------------------------------------------
//!
//! \brief lienaCustomizedMessage::append_uint32
//! \param v
//!
void lienaCustomizedMessage::append_uint32(uint32_t v){
    this->message_body[this->index]   = uint8_t(0xff000000&v)>>24;
    this->message_body[this->index+1] = uint8_t(0x00ff0000&v)>>16;
    this->message_body[this->index+2] = uint8_t(0x0000ff00&v)>>8;
    this->message_body[this->index+3] = uint8_t(0x000000ff&v);
    this->index += 4;
}

//! --------------------------------------------------------------------------------------------------------------------------
//!
//! \brief lienaCustomizedMessage::append_uint64
//! \param v
//!
void lienaCustomizedMessage::append_uint64(uint64_t v){
    this->message_body[this->index]   = uint8_t((0xff000000&v)>>56);
    this->message_body[this->index+1] = uint8_t((0x00ff0000&v)>>48);
    this->message_body[this->index+2] = uint8_t((0x0000ff00&v)>>40);
    this->message_body[this->index+3] = uint8_t((0x000000ff&v)>>32);
    this->message_body[this->index+4] = uint8_t((0xff000000&v)>>24);
    this->message_body[this->index+5] = uint8_t((0x00ff0000&v)>>16);
    this->message_body[this->index+6] = uint8_t((0x0000ff00&v)>>8);
    this->message_body[this->index+7] = uint8_t(0x000000ff&v);
    this->index += 8;
}





