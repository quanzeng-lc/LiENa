#include "lienaDiagnosisTask.h"


/**
 *
 * @brief lienaDiagnosisTask::lienaDiagnosisTask
 *
 */
lienaDiagnosisTask::lienaDiagnosisTask(){
    this->rtFlag = false;
    this->connexionFailed = false;
    this->rtPeriod = 2;
    this->heartBeatMessageCount = 0;
}

//!------------------------------------------------------------------------------
//!
//!
//! \brief lienaDiagnosisTask::~lienaDiagnosisTask
//!
lienaDiagnosisTask::~lienaDiagnosisTask(){

}

//!------------------------------------------------------------------------------
//!
//! \brief lienaDiagnosisTask::append
//! \param msg
//!
void lienaDiagnosisTask::append(lienaHeartBeatMessage* msg){
    this->heartBeatMessageCount += 1;
    mutex.lock();
    this->heartBeatMessage.append(msg);
    mutex.unlock();

    if(this->heartBeatMessageCount == 1){
        qDebug()<<"diagnosis task launch";
        this->launch();
    }
}

//!------------------------------------------------------------------------------
//!
//! \brief lienaDiagnosisTask::launch
//!
void lienaDiagnosisTask::launch(){
    this->rtFlag = true;
    this->connectionStatus = 1;
    this->start();
}

//!//!------------------------------------------------------------------------------
//! //! \brief lienaDiagnosisTask::get_sequence_length
//! \return
//!
int lienaDiagnosisTask::get_sequence_length(){
    int ret;
    mutex.lock();
    ret = this->heartBeatMessage.size();
    mutex.unlock();
    return ret;
}

//!------------------------------------------------------------------------------
//!
//! \brief lienaDiagnosisTask::get_latest_message
//! \return
//!
lienaHeartBeatMessage *lienaDiagnosisTask::get_latest_message(){
    lienaHeartBeatMessage *ret = nullptr;
    mutex.lock();
    ret = this->heartBeatMessage.at(0);
    mutex.unlock();
    return ret;
}

//!------------------------------------------------------------------------------
//!
//! \brief lienaDiagnosisTask::deleteFrontDatagram
//!
void lienaDiagnosisTask::deleteFrontDatagram(){
    mutex.lock();
    this->heartBeatMessage.pop_front();
    mutex.unlock();
}

//!------------------------------------------------------------------------------
//!
//! \brief lienaDiagnosisTask::connexionFailedRecovered
//!
void lienaDiagnosisTask::connexionFailedRecovered(){
    this->connexionFailed = false;
}

//!------------------------------------------------------------------------------
//!
//! \brief lienaDiagnosisTask::run
//!
void lienaDiagnosisTask::run(){
    while(this->rtFlag){

        if(this->connexionFailed){
            qDebug()<<"lienaDiagnosisTask | wait for network failure handling";
            sleep(1);
            continue;
        }

        int len = this->get_sequence_length();
        if(len > 0){
            this->connectionStatus = 1;
            //lienaHeartBeatMessage* msg = this->get_latest_message();
            this->deleteFrontDatagram();
        }
        else if(len == 0){
            this->connectionStatus *= 2;
            if(this->connectionStatus > 8){
                qDebug()<<"lenaDiagnosisTask | error";
                emit lostConnection();
                this->connectionStatus = 1;
                connexionFailed = true;
            }
        }
        sleep(this->rtPeriod);
    }
}
