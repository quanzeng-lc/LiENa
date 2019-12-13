#include "lienaDevice.h"

lienaDevice::lienaDevice()
{
    this->device_index = 0;
    this->device_id = 0;
    this->device_priority = 0;
    this->connect_topology = "";
}

//!-------------------------------------------------------------------------------
//!
//! \brief lienaDevice::getAddr
//! \return
//!
QString lienaDevice::getAddr(){
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

//!-------------------------------------------------------------------------------
//!
//! \brief lienaDevice::setAddr
//! \param address
//!
void lienaDevice::setAddr(QString address){
    QStringList addrs = address.split(".");
    addr[0] = uchar(addrs[0].toInt());
    addr[1] = uchar(addrs[1].toInt());
    addr[2] = uchar(addrs[2].toInt());
    addr[3] = uchar(addrs[3].toInt());
}

//!-------------------------------------------------------------------------------
//!
//! \brief lienaDevice::setAddr
//! \param addrZero
//! \param addrOne
//! \param addrTwo
//! \param addrThree
//!
void lienaDevice::setAddr(uchar addrZero, uchar addrOne, uchar addrTwo, uchar addrThree){
    addr[0] = addrZero;
    addr[1] = addrOne;
    addr[2] = addrTwo;
    addr[3] = addrThree;
}

//!-------------------------------------------------------------------------------
//!
//! \brief lienaDevice::set_device_index
//! \param device_index
//!
void lienaDevice::set_device_index(int device_index){
    this->device_index = device_index;
}

//!-------------------------------------------------------------------------------
//!
//! \brief set_device_id
//! \param device_id
//!
void lienaDevice::set_device_id(unsigned int device_id){
    this->device_id = device_id;
}

//!-------------------------------------------------------------------------------
//!
//! \brief lienaDevice::set_device_priority
//! \param device_priority
//!
void lienaDevice::set_device_priority(unsigned int device_priority){
    this->device_priority = device_priority;
}

void lienaDevice::set_connection_topology(QString connectiob_topolgy){
    this->connect_topology = connectiob_topolgy;
}

//!-------------------------------------------------------------------------------
//!
//! \brief lienaDevice::set_port
//! \param port
//!
void lienaDevice::set_port(unsigned short port){
    this->port = port;
}

//!-------------------------------------------------------------------------------
//!
//! \brief lienaDevice::get_device_index
//! \return
//!
int lienaDevice::get_device_index(){
    return this->device_index;
}

//!-------------------------------------------------------------------------------
//!
//! \brief lienaDevice::get_device_id
//! \return
//!
unsigned int lienaDevice::get_device_id(){
    return this->device_id;
}

//!-------------------------------------------------------------------------------
//!
//! \brief lienaDevice::get_device_priority
//! \return
//!
unsigned int lienaDevice::get_device_priority(){
    return this->device_priority;
}

//!-------------------------------------------------------------------------------
//!
//! \brief lienaDevice::set_transmission_period
//! \param period
//!
void lienaDevice::set_transmission_period(unsigned short period){
    this->transmission_task_period = period;
}

//!-------------------------------------------------------------------------------
//!
//! \brief lienaDevice::set_encode_period
//! \param period
//!
void lienaDevice::set_encode_period(unsigned short period){
    this->encoding_task_period = period;
}

//!-------------------------------------------------------------------------------
//!
//! \brief lienaDevice::set_reception_period
//! \param period
//!
void lienaDevice::set_reception_period(unsigned short period){
    this->reception_task_period = period;
}

//!-------------------------------------------------------------------------------
//!
//! \brief lienaDevice::set_decode_period
//! \param period
//!
void lienaDevice::set_decode_period(unsigned short period){
    this->decoding_task_period = period;
}

//!-------------------------------------------------------------------------------
//!
//! \brief lienaDevice::get_transimission_task
//! \return
//!
unsigned short lienaDevice::get_transimission_task(){
    return this->transmission_task_period;
}

//!-------------------------------------------------------------------------------
//!
//! \brief lienaDevice::get_decode_task
//! \return
//!
unsigned short lienaDevice::get_decode_task(){
    return this->decoding_task_period;
}

//!-------------------------------------------------------------------------------
//!
//! \brief lienaDevice::get_encode_period
//! \return
//!
unsigned short lienaDevice::get_encode_period(){
    return this->encoding_task_period;
}

//!-------------------------------------------------------------------------------
//!
//! \brief lienaDevice::get_reception_period
//! \return
//!
unsigned short lienaDevice::get_reception_period(){
    return this->reception_task_period;
}






















