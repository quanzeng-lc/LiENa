#include "lienaTransmissionTask.h"

/**
 * @brief igtClient::igtClient
 * @param index
 * @param outputQueueManager
 * @param targetIpLineEdit
 * @param targetPortLineEdit
 * @param MB
 */
lienaTransmissionTask::lienaTransmissionTask(SOCKET connectSocket, int index, lienaGlobal *globalParameter, lienaOutputQueue *outputQueue)
{
    this->connectSocket = connectSocket;

    this->index = index;
    this->globalParameter = globalParameter;
    this->outputQueue = outputQueue;

    this->flag = true;
    this->connectionStatus = false;
    this->rtPeriod = 20;
    this->compteur = 0;

    this->standby = true;

    this->localIP = this->ipDetect();

    this->encoder = new lienaDatagramEncoder(globalParameter);

    //this->connectera();
}

//! ----------------------------------------------------------------------------------------------------------------------------
//!
//! \brief lienaTransmissionTask::update_socket_descriptor
//! \param socket
//!
void lienaTransmissionTask::update_socket_descriptor(SOCKET socket){
    this->connectSocket = socket;
}

//! --------------------------------------------------------------------------------------------------------------------------------------------
//!
//! \brief igtClient::~igtClient
//!
lienaTransmissionTask::~lienaTransmissionTask(){

}

//! --------------------------------------------------------------------------------------------------------------------------------------------
//!
//! \brief lienaTransmissionTask::stop
//!
void lienaTransmissionTask::stop(){
    this->flag = false;
}

//! --------------------------------------------------------------------------------------------------------------------------------------------
//!
//! \brief lienaTransmissionTask::run
//!
void lienaTransmissionTask::run(){
    while(this->flag){

        if(standby){
            this->msleep(1000);
            continue;
        }

        this->transmit();
        this->msleep(this->rtPeriod);
    }
}

//! ----------------------------------------------------------------------------------------------------------------------------------------------
//!
//! \brief lienaTransmissionTask::enable
//!
void lienaTransmissionTask::enable(){
    this->standby = false;
}

//! ----------------------------------------------------------------------------------------------------------------------------------------------
//!
//! \brief lienaTransmissionTask::freeze
//!
void lienaTransmissionTask::freeze(){
    this->standby = true;

}

//! ----------------------------------------------------------------------------------------------------------------------------------------------
//!
//! \brief igtClient::wellConnected
//!
void lienaTransmissionTask::launch(){
    this->standby = false;
    this->start();
}

//! ----------------------------------------------------------------------------------------------------------------------------------------------
//!
//! \brief igtClient::write
//!
void lienaTransmissionTask::transmit(){
    if(this->outputQueue->getOutputQueueLength() > 0){
        lienaDatagram *ret = this->outputQueue->getFrontDatagram();
        qDebug()<<ret->getMessageID()<<"----------------------------------------------"<<ret->getOriginId()<<this->globalParameter->getLocalDeviceId();
        if(ret->getMessageID() == LIENA_SESSION_MANAGMENT_NTP_SYNCHRONIZATION_MESSAGE){

            if(ret->getOriginId() == this->globalParameter->getLocalDeviceId()){
                qDebug()<<"ntp write t1: "<<int(ret->getBody()[0])<<this->globalParameter->getTimestamps();
                ret->writeValueInEightBytes(29, this->globalParameter->getTimestamps());
            }
            else{
                qDebug()<<"ntp write t3: "<<int(ret->getBody()[0])<<this->globalParameter->getTimestamps();
                ret->writeValueInEightBytes(45, this->globalParameter->getTimestamps());
            }
        }

        int iResult =::send(this->connectSocket, *ret->getByteArray(), int(this->globalParameter->getGlobalDatagramSize()), 0);
        ::fflush(stdout);

        this->lastMessageId = ret->getMessageID();
        this->outputQueue->deleteFrontDatagram();
    }
}

//! -----------------------------------------------------------------------------------------------------------------------------------------------
//!
//! \brief lienaTransmissionTask::isChannelClosedMessageSended
//!
uint64_t lienaTransmissionTask::getLatestMessageID(){
    return this->lastMessageId;
}

//! -------------------------------------------------------------------------------------------------------------
//!
//! \brief igtClient::ipDetect
//! \return
//!
QString lienaTransmissionTask::ipDetect(){
    QString ret;

//    QList<QHostAddress> list = QNetworkInterface::allAddresses();

//    for(int nIter = 0; nIter < list.count(); nIter++){
//        if(!list[nIter].isLoopback()){
//            if(list[nIter].protocol() == QAbstractSocket::IPv4Protocol && list[nIter] != QHostAddress(QHostAddress::LocalHost)){

//                ret = list[nIter].toString();
//                if(ret.contains("192")){
//                    break;
//                }
//            }
//        }
//    }

    return ret;
}

//!----------------------------------------------------------------------------------------------------------------------------------------
//!
//! \brief igtClient::getConnectState
//! \return
//!
bool lienaTransmissionTask::getConnectionState(){
    return this->connectionStatus;
}
