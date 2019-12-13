#include "liena.h"


/**
 *
 *  @brief liena::liena
 *
 */
liena::liena(){
    this->init();
    this->connexion();
}

//! ------------------------------------------------------------------------------------------------------------------------------------------------------------------
//!
//! \brief liena::~liena
//!
liena::~liena(){
    delete this->globalParameter;
    delete this->tcpServer;
    delete this->outputMessageCache;
    delete this->inputMessageCache;
    delete this->distributedSystem;
}

//! ------------------------------------------------------------------------------------------------------------------------------------------------------------------
//!
//! \brief igtCommunicationStack::init
//!
void liena::init(){

    this->globalParameter    = new lienaGlobal();                                    // pre-defined essential parameter
    this->tcpServer          = new lienaTcpServer(this->globalParameter);            // tcp server pour ecouter des autres composants du system robotic
    this->inputMessageCache  = new lienaInputMessageCache();                         // input message cache for developer
    this->outputMessageCache = new lienaOutputMessageCache();                        // output message cache for developer
    this->distributedSystem  = new lienaDistributedSystem(this->globalParameter);    // object to store all module in the distributed system
}

//! ------------------------------------------------------------------------------------------------------------------------------------------------------------------
//!
//! \brief liena::setConnections
//!
void liena::connexion(){
    qRegisterMetaType<SOCKET>("SOCKET");
    this->connect(this->tcpServer,         SIGNAL(clientArrived(SOCKET, QString)),       this, SLOT(createReceptionChannel(SOCKET, QString)));
    this->connect(this->tcpServer,         SIGNAL(localIPDetect(QString)),               this, SLOT(getSelfIp(QString)));
    this->connect(this->distributedSystem, SIGNAL(generateNewMessageSequence(uint32_t)), this, SLOT(generateNewMessageSequenceBy(uint32_t)));
}

//! ------------------------------------------------------------------------------------------------------------------------------------------------------------------
//!
//! \brief liena::generateNewMessageSequenceBy
//! \param deviceID
//!
void liena::generateNewMessageSequenceBy(uint32_t deviceID){
    qDebug()<<"liena::generateNewMessageSequenceBy"<<deviceID;
    this->distributedSystem->configureMessageQueuePairBy(deviceID, this->outputMessageCache->generateNewMessageSequence(deviceID), this->inputMessageCache->generateNewMessageSequence(deviceID));
}

//! -------------------------------------------------------------------------------------------------------------------------------------------------------------------
//!
//! \brief liena::registDevice
//!
void liena::registerDevices(){
   this->globalParameter->setLocalDeviceId(this->globalParameter->getDSI()->get_first_deviceId());
   this->distributedSystem->setRoboticSystemModule(this->globalParameter->getDSI()->get_deviceIds());
}

//! -------------------------------------------------------------------------------------------------------------------------------------------------------------------
//!
//! \brief liena::setLocalDeviceID
//! \param deviceID
//!
void liena::setLocalDeviceID(uint32_t deviceID){
    this->globalParameter->setLocalDeviceId(deviceID);
}

//! -------------------------------------------------------------------------------------------------------------------------------------------------------------------
//!
//! \brief liena::checkAvailability
//! \param deviceID
//! \return
//!
bool liena::checkAvailability(uint32_t deviceID){
    bool ret = false;
    return ret;
    //this->inputMessageCache->write
}

//! ------------------------------------------------------------------------------------------------------------------------------------------------------------------
//!
//! \brief igtCommunicationStack::lauchServer
//! \return
//!
bool liena::launch(){
    return this->tcpServer->launchServer();
}

//! ------------------------------------------------------------------------------------------------------------------------------------------------------------------
//!
//! \brief igtCommunicationStack::stopServer
//! \return
//!
bool liena::terminate(){
    bool ret = false;
    ret = this->tcpServer->stopServer();
    return ret;
}

//!--------------------------------------------------------------------------------------------------------------------------------------------------------------
//!
//! \brief liena::incomingConnection
//! \param handle : qintptr
//!
void liena::createReceptionChannel(SOCKET socketReception, QString addr){
    if(this->distributedSystem->checkModuleByAddr(addr)){
        this->distributedSystem->generateReceptionChannelByAddr(socketReception, addr);
    }
    else{
        this->distributedSystem->createDistributedModuleForReceptionChannel(socketReception);
    }
}

//!--------------------------------------------------------------------------------------------------------------------------------------------------------------
//!
//! \brief createDistributedModuleForTransmissionChannel
//! \param addr
//! \param port
//!
void liena::createMotivateModule(uint32_t targetDeviceId, QString addr,uint16_t port){
    this->distributedSystem->createDistributedModuleWithTransmissionChannel(targetDeviceId, true, addr, port);
}

//! -------------------------------------------------------------------------------------------------------------------------------------------------------------
//!
//! \brief liena::openChannel : programming interface for developer, aim to open a in&out channel which a target device/software
//! \param addr
//! \param port
//! \return
//!
bool liena::openSessionRequest(uint32_t targetDeviceId, QString addr,uint16_t port){
    if(!this->tcpServer->getStatus()){
        qDebug()<<"liena tcp server not launched ...";
        return false;
    }

    this->createMotivateModule(targetDeviceId, addr, port);

    return true;
}

//! -------------------------------------------------------------------------------------------------------------------------------------------------------------
//!
//! \brief liena::closeSessionRequest
//! \param deviceID
//!
void liena::closeSessionRequest(uint32_t deviceID){
    this->distributedSystem->closeDistributedChannel(deviceID);
}

//! -------------------------------------------------------------------------------------------------------------------------------------------------------------
//!
//! \brief liena::getInputMessageCache
//! \return
//!
lienaInputMessageCache* liena::getInputMessageCache(){
    return this->inputMessageCache;
}

//! -------------------------------------------------------------------------------------------------------------------------------------------------------------
//!
//! \brief liena::getOutputMessageCache
//! \return
//!
lienaOutputMessageCache* liena::getOutputMessageCache(){
    return this->outputMessageCache;
}

//! --------------------------------------------------------------------------------------------------------------------------------------------------------------
//!
//! \brief igtCommunicationStack::getSelfIp
//! \param addr
//!
void liena::getSelfIp(QString addr){
    this->globalParameter->setLocalIPAddr(addr);
}

//! --------------------------------------------------------------------------------------------------------------------------------------------------------------
//!
//! \brief igtCommunicationStack::connectionEstablish
//!
void liena::connectionEstablish(){
    //this->transmissionTaskManager->launchLatestTask();
    //this->addHelloMessageTask->start();
}
