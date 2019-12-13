#include "lienaNetworkQualityMessage.h"

lienaNetworkQualityMessage::lienaNetworkQualityMessage(uint64_t messageId,
                                                       uint32_t targetId,
                                                       uint32_t originId,
                                                       uint64_t timestampes,
                                                       uint32_t DLC) :lienaMessage(messageId, targetId, originId, timestampes, DLC)
{
    this->index = 0;
    this->t1 = 0;
    this->t2 = 0;
    this->t3 = 0;
    this->t4 = 0;
}

//! ---------------------------------------------------------------------------------------------
//!
//! \brief lienaNetworkQualityMessage::get_inde
//!
int lienaNetworkQualityMessage::get_index(){
    return this->index;
}

//! ---------------------------------------------------------------------------------------------
//!
//! \brief lienaNetworkQualityMessage::get_t1
//!
uint64_t lienaNetworkQualityMessage::get_t1(){
    return this->t1;
}

//! ---------------------------------------------------------------------------------------------
//!
//! \brief lienaNetworkQualityMessage::get_t2
//!
uint64_t lienaNetworkQualityMessage::get_t2(){
    return this->t2;
}

//! ---------------------------------------------------------------------------------------------
//!
//! \brief lienaNetworkQualityMessage::get_t3
//!
uint64_t lienaNetworkQualityMessage::get_t3(){
    return this->t3;
}

//! ---------------------------------------------------------------------------------------------
//!
//! \brief lienaNetworkQualityMessage::set_index
//! \param index
//!
void lienaNetworkQualityMessage::set_index(int index){
    this->index = index;
}

//! ---------------------------------------------------------------------------------------------
//!
//! \brief lienaNetworkQualityMessage::get_t4
//!
uint64_t lienaNetworkQualityMessage::get_t4(){
    return this->t4;
}

//! ---------------------------------------------------------------------------------------------
//!
//! \brief lienaNetworkQualityMessage::set_t1
//! \param t
//! \return
//!
void lienaNetworkQualityMessage::set_t1(uint64_t t){
    this->t1 = t;
}

//! ---------------------------------------------------------------------------------------------
//!
//! \brief lienaNetworkQualityMessage::set_t2
//! \param t2
//! \return
//!
void lienaNetworkQualityMessage::set_t2(uint64_t t2){
    this->t2 = t2;
}

//! ---------------------------------------------------------------------------------------------
//!
//! \brief lienaNetworkQualityMessage::set_t3
//! \param t3
//! \return
//!
void lienaNetworkQualityMessage::set_t3(uint64_t t3){
    this->t3 = t3;
}

//! ---------------------------------------------------------------------------------------------
//!
//! \brief lienaNetworkQualityMessage::set_t4
//! \param t4
//! \return
//!
void lienaNetworkQualityMessage::set_t4(uint64_t t4){
    this->t4 = t4;

}
