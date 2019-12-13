#include "lienaDistributedModule.h"


/**
 * @brief lienaDistributedModule::lienaDistributedModule
 */
lienaDistributedModule::lienaDistributedModule(int index, lienaGlobal *globalParameter){
    this->index = index;
    this->globalParameter = globalParameter;
    this->init();
    this->motivate = false;
    this->diagnosis = false;
    this->openChannelProcedure = 0;
    this->repairChannelProcedure = 0;

    this->diagnosisConnectionTask = new lienaDiagnosisTask();
    this->connect(this->diagnosisConnectionTask,  SIGNAL(lostConnection()),   this, SLOT(launchRecoveryProcedure()));

}

//! ----------------------------------------------------------------------------------------------------------------------------
//!
//! \brief lienaDistributedModule::configureMessageQueuePair
//! \param outputMessageQueue
//! \param inputMessageQueue
//!
void lienaDistributedModule::configureMessageQueuePair(lienaMessageQueue *outputMessageQueue,lienaMessageQueue *inputMessageQueue){
    qDebug()<<"lienaDistributedModule::configureMessageQueuePair";
    this->encodingTask->setOutputMessageQueue(outputMessageQueue);
    this->decodingTask->setInputMessageQueue(inputMessageQueue);
}

//! ----------------------------------------------------------------------------------------------------------------------------
//!
//! \brief lienaDistributedModule::isInDiagnosis
//! \return
//!
bool lienaDistributedModule::isInDiagnosis(){
    return this->diagnosis;
}

//! ----------------------------------------------------------------------------------------------------------------------------
//!
//! \brief lienaDistributedModule::launchRecoveryProcedure
//!
void lienaDistributedModule::launchRecoveryProcedure(){
    qDebug()<<"lienaDistributedModule | launchRecoveryProcedure";
    ::closesocket(this->socketForReception);
    ::closesocket(this->socketForTransmission);
    this->receptionTask->freeze();
    this->transmissionTask->freeze();
    this->encodingTask->freeze();
    this->decodingTask->freeze();
    this->heartBeatTask->freeze();
    this->inputQueue->clear();
    this->outputQueue->clear();

    this->diagnosis = true;

    if(this->motivate){
        this->recoveryTask = new lienaRecoveryTask(this->address, this->port);

        qRegisterMetaType<SOCKET>("SOCKET");
        this->connect(this->recoveryTask, SIGNAL(reconnectSuccess(SOCKET soc)), this, SLOT(repairTransmissionChannelWith(SOCKET socket)));

        this->recoveryTask->launch();
    }
    // emit lostConnection(this->deviceID);
}

//! ----------------------------------------------------------------------------------------------------------------------------
//!
//! \brief lienaDistributedModule::launchReConnectProcedure
//!
void lienaDistributedModule::repairTransmissionChannelWith(SOCKET socket){
    qDebug()<<"lienaDistributedModule | launchReConnectProcedure";
    this->transmissionTask->update_socket_descriptor(socket);
    transmissionTask->enable();
    encodingTask->enable();
    decodingTask->enable();
    encodingTask->sendReHandshakeMessage();
}

//! ----------------------------------------------------------------------------------------------------------------------------
//!
//! \brief lienaDistributedModule::launchSessionRepaired
//! \param socket
//!
void lienaDistributedModule::launchSessionRepaired(){
    qDebug()<<"lienaDistributedModule | launchSessionRepaired";
}

//! ----------------------------------------------------------------------------------------------------------------------------
//!
//! \brief lienaDistributedModule::~lienaDistributedModule
//!
lienaDistributedModule::~lienaDistributedModule(){
    delete &index;
    delete &targetDeviceId;
    delete &address;
    delete &port;
    delete &socketForTransmission;
    delete &socketForReception;
    delete &connexionStatus;
    delete this->receptionTask;
    delete this->transmissionTask;
    delete this->decodingTask;
    delete this->encodingTask;
    delete this->heartBeatTask;
    delete this->globalParameter;
}

//! ---------------------------------------------------------------------------------------------------------------------
//!
//! \brief lienaDistributedModule::channelOpened
//!
void lienaDistributedModule::channelOpened(){
    qDebug()<<"lienaDistributedModule | channelOpened";

    this->encodingTask->sendChannelOpenedMessage();
    this->heartBeatTask = new lienaHeartBeatTask(this->outputQueue, this->globalParameter);
    this->heartBeatTask->launch();

    emit generateNewMessageSequence(targetDeviceId);
}

//! ----------------------------------------------------------------------------------------------------------------------
//!
//! \brief lienaDistributedModule::closeDistributedChannel
//! \param deviceId
//!
void lienaDistributedModule::closeDistributedChannel(){
    this->heartBeatTask->stop();
    this->encodingTask->sendDisengagementMessage();
}

//! -----------------------------------------------------------------------------------------------------------------------
//!
//! \brief lienaDistributedModule::launchDisengagementCommitProcedure
//!
void lienaDistributedModule::launchDisengagementCommitProcedure(lienaDisengagementMessage *){
    //! put disengagement commit message into the output queue
    encodingTask->sendDisengagementCommitMessage();
}

//! -----------------------------------------------------------------------------------------------------------------------
//!
//! \brief igtCommunicationStack::receptConnection
//! \param remoteIP
//!
void lienaDistributedModule::launchHandshakeCommitProcedure(lienaHandShakeMessage *handShakeMessage){
//    handShakeMessage->printHeader();

    this->address = handShakeMessage->getIpAddresse();
    this->port = handShakeMessage->getPort();

    qDebug()<<"launchHandshakeCommitProcedure address"<<this->address<<this->port;

    this->generateTransmissionChannel(false, handShakeMessage->getIpAddresse(), handShakeMessage->getPort());
}

//! ------------------------------------------------------------------------------------------------------------------------
//!
//! \brief lienaDistributedModule::notiftHeartBeatMessage
//! \param heartBeatMessage
//!
void lienaDistributedModule::notifChannelOpen(lienaChannelOpenedMessage* msg){
    heartBeatTask = new lienaHeartBeatTask(this->outputQueue, this->globalParameter);
    heartBeatTask->launch();

    emit generateNewMessageSequence(targetDeviceId);
}

//! ------------------------------------------------------------------------------------------------------------------------
//!
//! \brief lienaDistributedModule::notifChannelClose
//! \param msg
//!
void lienaDistributedModule::launchModuleCloseProcedure(lienaDisengagementCommitMessage* msg){
    this->encodingTask->sendChannelClosedMessage();
    Sleep(1000);
    this->terminateModule();
}

//! ------------------------------------------------------------------------------------------------------------------------
//!
//! \brief lienaDistributedModule::terminateModule
//!
void lienaDistributedModule::terminateModule(){
    qDebug()<<"lienaDistributedModule | terminateModule";
    decodingTask->stop();
    encodingTask->stop();
    receptionTask->stop();
    transmissionTask->stop();
    heartBeatTask->stop();
}

//!------------------------------------------------------------------------------------------------------------------------------
//!
//! \brief lienaDistributedModule::repairReceptionChannel
//! \param socketReception
//!
void lienaDistributedModule::repairReceptionChannel(SOCKET socketReception){
    if(this->motivate){
        if(this->repairChannelProcedure == 1){
            qDebug()<< "repair reception channel while motivate module";

            this->socketForReception = socketReception;
            this->decodingTask->enable();
            this->receptionTask->updateSocketDescripter(socketReception);
            this->receptionTask->enable();
        }
    }
    else{
        if(this->repairChannelProcedure == 1){
            return;
        }
        this->repairChannelProcedure = 1;

        qDebug()<< "repair reception channel while passive module";
        this->socketForReception = socketReception;
        this->receptionTask->updateSocketDescripter(socketReception);
        this->receptionTask->enable();
        this->decodingTask->enable();
    }
}

//!------------------------------------------------------------------------------------------------------------------------------
//!
//! \brief lienaDistributedModule::incomingConnection
//! \param handle : qintptr
//!
void lienaDistributedModule::generateReceptionChannel(SOCKET socketReception){

    if(this->motivate){
        if(this->openChannelProcedure == 1){
            qDebug()<< "generate reception channel while motivate module";
            this->inputQueue = new lienaInputQueue();

            this->decodingTask = new lienaDecodingTask(this->inputQueue, this->motivate, this->globalParameter->getLocalDeviceId(), this->targetDeviceId);

            //this->connect(decodingTask, SIGNAL(connexionConfirm()),                                                         this,   SLOT(connectionEstablish()));
            this->connect(this->decodingTask, SIGNAL(handShakeMessageArrived(lienaHandShakeMessage *)),                       this,  SLOT(launchHandshakeCommitProcedure(lienaHandShakeMessage *)));
            this->connect(this->decodingTask, SIGNAL(handShakeCommitMessageArrived()),                                        this,  SLOT(channelOpened()));
            this->connect(this->decodingTask, SIGNAL(channelOpenMsg(lienaChannelOpenedMessage*)),                             this,  SLOT(notifChannelOpen(lienaChannelOpenedMessage*)));
            this->connect(this->decodingTask, SIGNAL(disengagementMessageArrived(lienaDisengagementMessage*)),                this,  SLOT(launchDisengagementCommitProcedure(lienaDisengagementMessage *)));
            this->connect(this->decodingTask, SIGNAL(disengagementCommitMessageArrived(lienaDisengagementCommitMessage*)),    this,  SLOT(launchModuleCloseProcedure(lienaDisengagementCommitMessage*)));
            this->connect(this->decodingTask, SIGNAL(channelClosedMessageArrived()),                                          this,  SLOT(launchModuleClose()));
            this->connect(this->decodingTask, SIGNAL(heartBeatMessageArrived(lienaHeartBeatMessage*)),                        this,  SLOT(launchHeartBeatMessage(lienaHeartBeatMessage*)));
            this->connect(this->decodingTask, SIGNAL(rehandshakeMsgArrived(lienaReHandshakeMessage*)),                        this,  SLOT(launchRehandshakeCommitProcedure(lienaReHandshakeMessage*)));
            this->connect(this->decodingTask, SIGNAL(rehandshakeCommitMsgArrived(lienaReHandshakeCommitMessage*)),            this,  SLOT(channnelRepaired()));
            this->connect(this->decodingTask, SIGNAL(reOpenedMsgArrived(lienaChannelReOpened*)),                              this,  SLOT(launchChannelReopened(lienaChannelReOpened* )));
            this->connect(this->decodingTask, SIGNAL(networkPassiveQualityMessageArrived(lienaNetworkQualityMessage*)),       this,  SLOT(returnBackNetworkQUalityMeaasge(lienaNetworkQualityMessage*)));
            this->connect(this->decodingTask, SIGNAL(networkMotivateQualityMessageArrived(lienaNetworkQualityMessage*)),      this,  SLOT(restoreNetworkQualityMessage(lienaNetworkQualityMessage*)));

            this->decodingTask->launch();

            receptionTask = new lienaReceptionTask(this->index, this->globalParameter, socketReception, this->inputQueue);
            receptionTask->launch();
        }
    }
    else{
        if(this->openChannelProcedure == 1){
            return;
        }
        this->openChannelProcedure = 1;

        qDebug()<< "generate reception channel while passive module";
        this->inputQueue = new lienaInputQueue();

        this->decodingTask = new lienaDecodingTask(this->inputQueue, this->motivate, this->globalParameter->getLocalDeviceId(), this->targetDeviceId);

        this->connect(this->decodingTask, SIGNAL(handShakeMessageArrived(lienaHandShakeMessage *)),                       this,  SLOT(launchHandshakeCommitProcedure(lienaHandShakeMessage *)));
        this->connect(this->decodingTask, SIGNAL(handShakeCommitMessageArrived()),                                        this,  SLOT(channelOpened()));
        this->connect(this->decodingTask, SIGNAL(channelOpenMsg(lienaChannelOpenedMessage*)),                             this,  SLOT(notifChannelOpen(lienaChannelOpenedMessage*)));
        this->connect(this->decodingTask, SIGNAL(disengagementMessageArrived(lienaDisengagementMessage*)),                this,  SLOT(launchDisengagementCommitProcedure(lienaDisengagementMessage *)));
        this->connect(this->decodingTask, SIGNAL(disengagementCommitMessageArrived(lienaDisengagementCommitMessage*)),    this,  SLOT(launchModuleCloseProcedure(lienaDisengagementCommitMessage*)));
        this->connect(this->decodingTask, SIGNAL(channelClosedMessageArrived()),                                          this,  SLOT(launchModuleClose()));
        this->connect(this->decodingTask, SIGNAL(heartBeatMessageArrived(lienaHeartBeatMessage*)),                        this,  SLOT(launchHeartBeatMessage(lienaHeartBeatMessage*)));
        this->connect(this->decodingTask, SIGNAL(rehandshakeMsgArrived(lienaReHandshakeMessage*)),                        this,  SLOT(launchRehandshakeCommitProcedure(lienaReHandshakeMessage*)));
        this->connect(this->decodingTask, SIGNAL(rehandshakeCommitMsgArrived(lienaReHandshakeCommitMessage*)),            this,  SLOT(channnelRepaired()));
        this->connect(this->decodingTask, SIGNAL(reOpenedMsgArrived(lienaChannelReOpened*)),                              this,  SLOT(launchChannelReopened(lienaChannelReOpened* )));
        this->connect(this->decodingTask, SIGNAL(networkPassiveQualityMessageArrived(lienaNetworkQualityMessage*)),       this,  SLOT(returnBackNetworkQUalityMeaasge(lienaNetworkQualityMessage*)));
        this->connect(this->decodingTask, SIGNAL(networkMotivateQualityMessageArrived(lienaNetworkQualityMessage*)),      this,  SLOT(restoreNetworkQualityMessage(lienaNetworkQualityMessage*)));

        this->decodingTask->launch();

        receptionTask = new lienaReceptionTask(this->index, this->globalParameter, socketReception, this->inputQueue);
        receptionTask->launch();
    }
}

//! ------------------------------------------------------------------------------------------------------------------------
//!
//! \brief lienaDistributedModule::restoreNetworkQualityMessage
//! \param msg
//!
void lienaDistributedModule::restoreNetworkQualityMessage(lienaNetworkQualityMessage* msg){
    this->networkQualityTask->receive(msg);
}

//! ------------------------------------------------------------------------------------------------------------------------
//!
//! \brief lienaDistributedModule::returnBackNetworkQUalityMeaasge
//! \param msg
//!
void lienaDistributedModule::returnBackNetworkQUalityMeaasge(lienaNetworkQualityMessage *msg){
    qDebug()<<"lienaNTPSynchronizationTask::sendBackNetworkQualityMessage";
    lienaDatagram* datagram = this->encodeNetworkQualityMessage(msg);
    if(datagram != nullptr){
        this->outputQueue->append(datagram);
    }
}

//! ------------------------------------------------------------------------------------------------------------------------
//!
//! \brief lienaDistributedModule::launchRehandshakeCommitProcedure
//! \param msg
//!
void lienaDistributedModule::launchRehandshakeCommitProcedure(lienaReHandshakeMessage *msg){
    qDebug()<<"lienaDistributedModule | launchRehandshakeCommitProcedure";
    this->repairTransmissionChannel();
}

//! ------------------------------------------------------------------------------------------------------------------------
//!
//! \brief lienaDistributedModule::channnelReOpened
//!
void lienaDistributedModule::channnelRepaired(){
    qDebug()<<"lienaDistributedModule | channnelReOpened";
    encodingTask->sendChannelRepairedMessage();
    Sleep(1);
    this->heartBeatTask->enable();
    this->diagnosisConnectionTask->connexionFailedRecovered();
}

//! ------------------------------------------------------------------------------------------------------------------------
//!
//! \brief lienaDistributedModule::notifChannelReOpened
//!
void lienaDistributedModule::launchChannelReopened(lienaChannelReOpened* msg){
    qDebug()<<"lienaDistributedModule | notifChannelReOpened";
    this->diagnosisConnectionTask->connexionFailedRecovered();
    this->heartBeatTask->enable();
}

//! ------------------------------------------------------------------------------------------------------------------------
//!
//! \brief lienaDistributedModule::launchHeartBeatMessage
//! \param msg
//!
void lienaDistributedModule::launchHeartBeatMessage(lienaHeartBeatMessage *msg){
    if(DEBUG){
        qDebug()<<"lienaDistributedModule | launchHeartBeatMessage";
    }
    this->diagnosisConnectionTask->append(msg);
    if(this->diagnosisConnectionTask->get_sequence_length() == 1){
        if(this->motivate){
            if(!this->executed){
                qDebug()<<" instantiate lienaNTPSynchronizationTask";
                this->networkQualityTask = new lienaNTPSynchronizationTask(this->outputQueue, this->globalParameter, this->targetDeviceId);
                this->networkQualityTask->setLoopNumber(10);
                this->networkQualityTask->launch();
                this->executed = true;
            }
        }
    }
}

//! ------------------------------------------------------------------------------------------------------------------------
//!
//! \brief lienaDistributedModule::repairTransmissionChannel
//!
int lienaDistributedModule::repairTransmissionChannel(){
    int ret = -1;
    if(motivate){
        if(this->repairChannelProcedure == 1){
            return ret;
        }
        this->repairChannelProcedure = 1;
        //...
    }
    else{
        if(this->repairChannelProcedure == 1){

            tcpClient = new lienaTcpClient(this->address, this->port);
            ret = tcpClient->connectera(2);
            if (ret == 1){
                this->transmissionTask->update_socket_descriptor(tcpClient->getSocketCom());
                this->transmissionTask->enable();
                this->encodingTask->enable();
                //Sleep(2000);
                this->encodingTask->sendReHandshakeCommitMessage();
            }
        }
    }
    return ret;
}

//! ------------------------------------------------------------------------------------------------------------------------
//!
//! \brief lienaDistributedModule::generateTransmissionChannel
//! \param addr
//! \param port
//!
int lienaDistributedModule::generateTransmissionChannel(bool motivate, QString addr, uint16_t port){
    int ret = -1;
    this->motivate = motivate;

    if(DEBUG){
        qDebug()<<"lienaDistributedModule | generateTransmissionChannel";
    }

    if(motivate){
        if(this->openChannelProcedure == 1){
            return ret;
        }
        this->openChannelProcedure = 1;

        this->address = addr;
        this->port = port;

        tcpClient = new lienaTcpClient(addr, port);
        ret = tcpClient->connectera(5);

        if(ret == 1){
            this->outputQueue = new lienaOutputQueue();

            encodingTask = new lienaEncodingTask(this->outputQueue, this->globalParameter, this->motivate, this->targetDeviceId);
            encodingTask->launch();

            transmissionTask = new lienaTransmissionTask(tcpClient->getSocketCom(), this->index, this->globalParameter, this->outputQueue);
            transmissionTask->launch();

            //! deprecated
            encodingTask->sendHandShakeMessage(tcpClient->getLocalIp());
        }
    }
    else{

        //! connect back
        tcpClient = new lienaTcpClient(addr, port);
        ret = tcpClient->connectera(5);
        if(ret == 1){
            //! generate an output queue
            outputQueue = new lienaOutputQueue();

            //! decoding task generate
            encodingTask = new lienaEncodingTask(this->outputQueue, this->globalParameter, this->motivate, this->targetDeviceId);
            encodingTask->launch();

            //! transmission task
            transmissionTask = new lienaTransmissionTask(tcpClient->getSocketCom(), this->index, this->globalParameter, this->outputQueue);
            transmissionTask->launch();

            //! put handshake commit message into the output queue
            encodingTask->sendHandShakeCommitMessage();
        }
    }
    return ret;
}

//!------------------------------------------------------------------------------------------------------------------------------
//!
//! \brief lienaDistributedModule::freeze
//!
void lienaDistributedModule::freeze(){

}

//! ----------------------------------------------------------------------------------------------------------------------------
//!
//! \brief lienaDistributedModule::init
//!
void lienaDistributedModule::init(){
    this->index = 0;
    this->targetDeviceId = 0;
    this->address = "";
    this->port = this->globalParameter->getGlobalPort();
    this->openChannelProcedure = 0;
    this->executed = false;
}

//! ----------------------------------------------------------------------------------------------------------------------------
//!
//! \brief lienaDistributedModule::setIndex
//! \param index
//!
void lienaDistributedModule::setIndex(int index){
    this->index = index;
}

//! ----------------------------------------------------------------------------------------------------------------------------
//!
//! \brief lienaDistributedModule::setDeviceId
//! \param deviceID
//!
void lienaDistributedModule::setTargetDeviceId(uint32_t targetDeviceId){
    this->targetDeviceId = targetDeviceId;
}

//! ----------------------------------------------------------------------------------------------------------------------------
//!
//! \brief lienaDistributedModule::setOriginDeviceId
//! \param originDeviceId
//!
void lienaDistributedModule::setOriginDeviceId(uint32_t originDeviceId){
    this->originDeviceId = originDeviceId;
}

//! ----------------------------------------------------------------------------------------------------------------------------
//!
//! \brief lienaDistributedModule::setAddress
//! \param address
//!
void lienaDistributedModule::setAddress(QString address){
    this->address = address;
}

//! ----------------------------------------------------------------------------------------------------------------------------
//!
//! \brief lienaDistributedModule::setPort
//! \param port
//!
void lienaDistributedModule::setPort(uint16_t port){
    this->port = port;
}

//! ----------------------------------------------------------------------------------------------------------------------------
//!
//! \brief lienaDistributedModule::setSocketForTransmission
//! \param socketForTransmission
//!
void lienaDistributedModule::setSocketForTransmission(SOCKET socketForTransmission){
    this->socketForTransmission = socketForTransmission;
}

//! ----------------------------------------------------------------------------------------------------------------------------
//!
//! \brief lienaDistributedModule::setSocketForReception
//! \param socketForReception
//!
void lienaDistributedModule::setSocketForReception(SOCKET socketForReception){
    this->socketForReception = socketForReception;
}

//! ----------------------------------------------------------------------------------------------------------------------------
//!
//! \brief lienaDistributedModule::setConnexionStatus
//! \param connexionStatus
//!
void lienaDistributedModule::setConnexionStatus(int connexionStatus){
    this->connexionStatus = connexionStatus;
}

//! ----------------------------------------------------------------------------------------------------------------------------
//!
//! \brief lienaDistributedModule::getIndex
//! \return
//!
int lienaDistributedModule::getIndex(){
    return this->index;
}

//! ----------------------------------------------------------------------------------------------------------------------------
//!
//! \brief lienaDistributedModule::getDeviceId
//! \return
//!
uint32_t lienaDistributedModule::getDeviceId(){
    return this->targetDeviceId;
}

//! ----------------------------------------------------------------------------------------------------------------------------
//!
//! \brief lienaDistributedModule::getAddress
//! \return
//!
QString lienaDistributedModule::getAddress(){
    return this->address;
}

//! ----------------------------------------------------------------------------------------------------------------------------
//!
//! \brief lienaDistributedModule::getPort
//! \return
//!
uint16_t lienaDistributedModule::getPort(){
    return this->port;
}

//! ----------------------------------------------------------------------------------------------------------------------------
//!
//! \brief lienaDistributedModule::getSocketForTransmission
//! \return
//!
SOCKET lienaDistributedModule::getSocketForTransmission(){
    return this->socketForTransmission;

}

//! ----------------------------------------------------------------------------------------------------------------------------
//!
//! \brief lienaDistributedModule::getSocketForReception
//! \return
//!
SOCKET lienaDistributedModule::getSocketForReception(){
    return this->socketForReception;
}

//! ----------------------------------------------------------------------------------------------------------------------------
//!
//! \brief lienaDistributedModule::getConnexionStatus
//! \return
//!
int lienaDistributedModule::getConnexionStatus(){
    return this->connexionStatus;
}

//! ---------------------------------------------------------------------------------------------------------------
//!
//! \brief lienaDistributedModule::channelClosed
//!
void lienaDistributedModule::launchModuleClose(){
    qDebug()<<"lienaDistributedModule | closeChannel";
    decodingTask->stop();
    encodingTask->stop();
    receptionTask->stop();
    transmissionTask->stop();
    heartBeatTask->stop();
}

//! ----------------------------------------------------------------------------------------------------------------------------
//!
//! \brief lienaDistributedModule::printSelf
//!
void lienaDistributedModule::printSelf(){
    qDebug()<<" -------------------------------------------- ";
    qDebug()<<"index:"<<this->index;
    qDebug()<<"deviceId:"<<this->targetDeviceId;
    qDebug()<<"address:"<<this->address<<"port:"<<this->port;
    qDebug()<<"socket for transmission"<<this->socketForTransmission<<"socketForReception:"<<this->socketForReception<<"connexionStatus:"<<this->connexionStatus;
    qDebug()<<" -------------------------------------------- ";
}

lienaDatagram* lienaDistributedModule::encodeNetworkQualityMessage(lienaNetworkQualityMessage* networkQualityMessage){

    lienaDatagram *datagram = nullptr;
    if(networkQualityMessage != nullptr){

        uint32_t globalDatagramSize = globalParameter->getGlobalDatagramSize();
        qDebug()<<"globalDatagramSize:"<<globalDatagramSize;
        char* byteArray = new char[globalDatagramSize];

        uint64_t  messageId = networkQualityMessage->getMessageId();
        uint32_t targetId = networkQualityMessage->getTargetId();
        uint32_t originId = networkQualityMessage->getOriginId();
        uint64_t timestampes = networkQualityMessage->getTimestampes();
        uint32_t dlc = networkQualityMessage->getDLC();
        byteArray[0]  = uchar((0xff00000000000000&messageId)>>56);
        byteArray[1]  = uchar((0x00ff000000000000&messageId)>>48);
        byteArray[2]  = uchar((0x0000ff0000000000&messageId)>>40);
        byteArray[3]  = uchar((0x000000ff00000000&messageId)>>32);
        byteArray[4]  = uchar((0x00000000ff000000&messageId)>>24);
        byteArray[5]  = uchar((0x0000000000ff0000&messageId)>>16);
        byteArray[6]  = uchar((0x000000000000ff00&messageId)>>8);
        byteArray[7]  = uchar((0x00000000000000ff&messageId));

        byteArray[8]  = uchar((0xff000000&targetId)>>24);
        byteArray[9]  = uchar((0x00ff0000&targetId)>>16);
        byteArray[10] = uchar((0x0000ff00&targetId)>>8);
        byteArray[11] = uchar((0x000000ff&targetId));

        byteArray[12] = uchar((0xff000000&originId)>>24);
        byteArray[13] = uchar((0x00ff0000&originId)>>16);
        byteArray[14] = uchar((0x0000ff00&originId)>>8);
        byteArray[15] = uchar((0x000000ff&originId));

        byteArray[16] = uchar((0xff00000000000000&timestampes)>>56);
        byteArray[17] = uchar((0x00ff000000000000&timestampes)>>48);
        byteArray[18] = uchar((0x0000ff0000000000&timestampes)>>40);
        byteArray[19] = uchar((0x000000ff00000000&timestampes)>>32);
        byteArray[20] = uchar((0x00000000ff000000&timestampes)>>24);
        byteArray[21] = uchar((0x0000000000ff0000&timestampes)>>16);
        byteArray[22] = uchar((0x000000000000ff00&timestampes)>>8);
        byteArray[23] = uchar((0x00000000000000ff&timestampes));

        byteArray[24] = uchar((0xff000000&dlc)>>24);
        byteArray[25] = uchar((0x00ff0000&dlc)>>16);
        byteArray[26] = uchar((0x0000ff00&dlc)>>8);
        byteArray[27] = uchar((0x000000ff&dlc));

        byteArray[28] = uchar((0x000000ff&networkQualityMessage->get_index()));

        byteArray[29] = uchar((0xff00000000000000&networkQualityMessage->get_t1())>>56);
        byteArray[30] = uchar((0x00ff000000000000&networkQualityMessage->get_t1())>>48);
        byteArray[31] = uchar((0x0000ff0000000000&networkQualityMessage->get_t1())>>40);
        byteArray[32] = uchar((0x000000ff00000000&networkQualityMessage->get_t1())>>32);
        byteArray[33] = uchar((0x00000000ff000000&networkQualityMessage->get_t1())>>24);
        byteArray[34] = uchar((0x0000000000ff0000&networkQualityMessage->get_t1())>>16);
        byteArray[35] = uchar((0x000000000000ff00&networkQualityMessage->get_t1())>>8);
        byteArray[36] = uchar((0x00000000000000ff&networkQualityMessage->get_t1()));

        byteArray[37] = uchar((0xff00000000000000&networkQualityMessage->get_t2())>>56);
        byteArray[38] = uchar((0x00ff000000000000&networkQualityMessage->get_t2())>>48);
        byteArray[39] = uchar((0x0000ff0000000000&networkQualityMessage->get_t2())>>40);
        byteArray[40] = uchar((0x000000ff00000000&networkQualityMessage->get_t2())>>32);
        byteArray[41] = uchar((0x00000000ff000000&networkQualityMessage->get_t2())>>24);
        byteArray[42] = uchar((0x0000000000ff0000&networkQualityMessage->get_t2())>>16);
        byteArray[43] = uchar((0x000000000000ff00&networkQualityMessage->get_t2())>>8);
        byteArray[44] = uchar((0x00000000000000ff&networkQualityMessage->get_t2()));

        byteArray[45] = uchar((0xff00000000000000&networkQualityMessage->get_t3())>>56);
        byteArray[46] = uchar((0x00ff000000000000&networkQualityMessage->get_t3())>>48);
        byteArray[47] = uchar((0x0000ff0000000000&networkQualityMessage->get_t3())>>40);
        byteArray[48] = uchar((0x000000ff00000000&networkQualityMessage->get_t3())>>32);
        byteArray[49] = uchar((0x00000000ff000000&networkQualityMessage->get_t3())>>24);
        byteArray[50] = uchar((0x0000000000ff0000&networkQualityMessage->get_t3())>>16);
        byteArray[51] = uchar((0x000000000000ff00&networkQualityMessage->get_t3())>>8);
        byteArray[52] = uchar((0x00000000000000ff&networkQualityMessage->get_t3()));

        byteArray[53] = uchar((0xff00000000000000&networkQualityMessage->get_t4())>>56);
        byteArray[54] = uchar((0x00ff000000000000&networkQualityMessage->get_t4())>>48);
        byteArray[55] = uchar((0x0000ff0000000000&networkQualityMessage->get_t4())>>40);
        byteArray[56] = uchar((0x000000ff00000000&networkQualityMessage->get_t4())>>32);
        byteArray[57] = uchar((0x00000000ff000000&networkQualityMessage->get_t4())>>24);
        byteArray[58] = uchar((0x0000000000ff0000&networkQualityMessage->get_t4())>>16);
        byteArray[59] = uchar((0x000000000000ff00&networkQualityMessage->get_t4())>>8);
        byteArray[60] = uchar((0x00000000000000ff&networkQualityMessage->get_t4()));
        for(unsigned int i =61; i< globalDatagramSize; i++){
            byteArray[i] = '\0';
        }
        datagram = new lienaDatagram(globalDatagramSize, &byteArray);
    }
    return datagram;
}
