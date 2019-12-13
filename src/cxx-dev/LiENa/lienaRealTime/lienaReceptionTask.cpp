#include "lienaReceptionTask.h"


/**
 * @brief igtReceptionTask::igtReceptionTask
 * @param ID
 * @param inputQueue
 */
lienaReceptionTask::lienaReceptionTask(int index, lienaGlobal *globalParameter, SOCKET recSoc, lienaInputQueue *inputQueue){
    qDebug()<<"lienaReceptionTask"<<index;
    this->index = index;
    this->globalParameter = globalParameter;
    this->recSoc = recSoc;
    this->inputQueue = inputQueue;
    this->compteur = 0;
    this->init();
}

//! ------------------------------------------------------------------------------------------------------------------------------------------
//!
//! \brief igtReceptionTask::run
//!
void lienaReceptionTask::run(){
    while(this->rtFlag){

        if(standby){
            this->msleep(1000);
            continue;
        }

        this->globalDatagramSize = globalParameter->getGlobalDatagramSize();

        char* byteArray = new char[globalDatagramSize+1]; //(char *)malloc(globalDatagramSize+1);

        int nRecv = ::recv(this->recSoc, byteArray, int(globalDatagramSize+1), 0);
        ::fflush(stdin);
        if(nRecv > 0){
            byteArray[globalDatagramSize] = '\0';
            lienaDatagram *datagram = new lienaDatagram(globalDatagramSize, &byteArray);
            if(datagram->getMessageID() == LIENA_SESSION_MANAGMENT_NTP_SYNCHRONIZATION_MESSAGE){

                if(datagram->getOriginId() == this->globalParameter->getLocalDeviceId()){
                    qDebug()<<"ntp write t4: "<<int(datagram->getBody()[0])<<this->globalParameter->getTimestamps();
                    datagram->writeValueInEightBytes(37, this->globalParameter->getTimestamps());
                }
                else{
                    qDebug()<<"ntp write t2: "<<int(datagram->getBody()[0])<<this->globalParameter->getTimestamps();
                    datagram->writeValueInEightBytes(53, this->globalParameter->getTimestamps());
                }
            }
            this->inputQueue->append(datagram);
        }
        else{
            //qDebug()<<"no msg yet..";
        }
        this->msleep(this->rtPeriod );
    }
}

//! ------------------------------------------------------------------------------------------------------------------------------------------
//!
//! \brief lienaReceptionTask::getIndex
//! \return
//!
int lienaReceptionTask::getIndex(){
    return this->index;
}

//! ------------------------------------------------------------------------------------------------------------------------------------------
//!
//! \brief lienaReceptionTask::getTaskStatus
//! \return
//!
bool lienaReceptionTask::getTaskStatus(){
    return this->rtFlag;
}

//! ------------------------------------------------------------------------------------------------------------------------------------------
//!
//! \brief lienaReceptionTask::changePeriod
//! \param period
//!
void lienaReceptionTask::changePeriod(unsigned int period){
    this->rtPeriod = period;
}

//! ------------------------------------------------------------------------------------------------------------------------------------------
//!
//! \brief lienaReceptionTask::init
//!
void lienaReceptionTask::init(){
    this->rtFlag = true;
    this->rtPeriod = 20;
    this->dlc  = 1024;
    this->standby = true;
}

//! ------------------------------------------------------------------------------------------------------------------------------------------
//!
//! \brief lienaReceptionTask::updateSocketDescripter
//! \param recSoc
//!
void lienaReceptionTask::updateSocketDescripter(SOCKET recSoc){
    this->recSoc = recSoc;
}

//! ------------------------------------------------------------------------------------------------------------------------------------------
//!
//! \brief lienaReceptionTask::enable
//!
void lienaReceptionTask::enable(){
    this->standby = false;
}

//! ------------------------------------------------------------------------------------------------------------------------------------------
//!
//! \brief lienaReceptionTask::freeze
//!
void lienaReceptionTask::freeze(){
    this->standby = true;
}

//! ------------------------------------------------------------------------------------------------------------------------------------------
//!
//! \brief lienaReceptionTask::launch
//!
void lienaReceptionTask::launch(){
    this->rtFlag = true;
    this->standby = false;
    this->start();
}

//! ------------------------------------------------------------------------------------------------------------------------------------------
//!
//! \brief igtReceptionTask::stop
//!
void lienaReceptionTask::stop(){
    this->rtFlag = false;
}

//! ------------------------------------------------------------------------------------------------------------------------------------------
//!
//! \brief lienaReceptionTask::setGlobalDLC
//! \param dlc
//!
void lienaReceptionTask::setGlobalDLC(unsigned int dlc){
    this->dlc = dlc;
}
