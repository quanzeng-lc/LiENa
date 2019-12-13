#include "lienaDistributedSystem.h"


/**
 *
 * @brief lienaDistributedSystem::lienaDistributedSystem
 * @param globalParameter
 *
 */
lienaDistributedSystem::lienaDistributedSystem(lienaGlobal *globalParameter){
    this->globalParameter = globalParameter;
    this->init();
}

//! -----------------------------------------------------------------------------------------------------------------------------------------------------
//!
//! \brief lienaDistributedSystem::~lienaDistributedSystem
//!
lienaDistributedSystem::~lienaDistributedSystem(){
    delete this->globalParameter;
    delete &this->distributedModules;
}

//! ------------------------------------------------------------------------------------------------------------------------------------------------------
//!
//! \brief lienaDistributedSystem::checkModuleByAddr
//! \param addr
//! \return
//!
bool lienaDistributedSystem::checkModuleByAddr(QString addr){
    bool ret = false;
    for(int i = 0; i < distributedModules.size(); i++){
        if(distributedModules.at(i)->getAddress() == addr){
            ret = true;
        }
    }
    return ret;
}

//! ------------------------------------------------------------------------------------------------------------------------------------------------------
//!
//! \brief lienaDistributedSystem::configureMessageQueuePairBy
//! \param deviceID
//! \param outputMessageQueue
//! \param inputMessageQueue
//!
void lienaDistributedSystem::configureMessageQueuePairBy(uint32_t deviceID, lienaMessageQueue *outputMessageQueue,lienaMessageQueue *inputMessageQueue){
    qDebug()<<"lienaDistributedSystem::configureMessageQueuePairBy";
    for(int i = 0; i < distributedModules.size(); i++){
        if(distributedModules.at(i)->getDeviceId() == deviceID){
            distributedModules.at(i)->configureMessageQueuePair(outputMessageQueue,inputMessageQueue);
        }
    }
}

//! ------------------------------------------------------------------------------------------------------------------------------------------------------
//!
//! \brief lienaDistributedSystem::generateReceptionChannelByAddr
//! \param socketReception
//! \param addr
//!
void lienaDistributedSystem::generateReceptionChannelByAddr(SOCKET socketReception, QString addr){
    for(int i  = 0; i < distributedModules.size(); i++){
        if(distributedModules.at(i)->getAddress() == addr){
            if(distributedModules.at(i)->isInDiagnosis()){
                distributedModules.at(i)->repairReceptionChannel(socketReception);
            }
            else{
                distributedModules.at(i)->generateReceptionChannel(socketReception);
            }
        }
    }
}

//! ------------------------------------------------------------------------------------------------------------------------------------------------------
//!
//!
//! \brief lienaDistributedSystem::setRoboticSystemModule
//! \param deviceIDs
//!
void lienaDistributedSystem::setRoboticSystemModule(QVector<uint32_t> deviceIDs){
    this->globalDeviceIDs = deviceIDs;
}

//! ------------------------------------------------------------------------------------------------------------------------------------------------------
//!
//! \brief lienaDistributedSystem::creatIncomingModule
//! \return
//!
void lienaDistributedSystem::createDistributedModuleForReceptionChannel(SOCKET socketReception){
    lienaDistributedModule *module = new lienaDistributedModule(this->moduleIndex, this->globalParameter);
    this->connect(module, SIGNAL(lostConnection(uint32_t)), this, SLOT(launchRecoveryProcedure(uint32_t)));
    this->connect(module, SIGNAL(generateNewMessageSequence(uint32_t)), this, SLOT(notifyGenerateNewMessageSequence(uint32_t)));

    module->generateReceptionChannel(socketReception);
    this->distributedModules.append(module);
    this->moduleIndex ++;
}

//! ------------------------------------------------------------------------------------------------------------------------------------------------------
//!
//! \brief lienaDistributedSystem::launchRecoveryProcedure
//! \param deviceId
//!
void lienaDistributedSystem::launchRecoveryProcedure(uint32_t deviceId){
    qDebug()<<"lienaDistributedSystem::launchRecoveryProcedure"<<deviceId;
}

//! ------------------------------------------------------------------------------------------------------------------------------------------------------
//!
//! \brief lienaDistributedSystem::notifyGenerateNewMessageSequence
//! \param deviceId
//!
void lienaDistributedSystem::notifyGenerateNewMessageSequence(uint32_t deviceId){
    emit generateNewMessageSequence(deviceId);
}

//! ------------------------------------------------------------------------------------------------------------------------------------------------------
//!
//! \brief createDistributedModuleWithTransmissionChannel
//! \param addr
//! \param port
//!
bool lienaDistributedSystem::createDistributedModuleWithTransmissionChannel(uint32_t targetDeviceId,bool motivated, QString addr, unsigned short port){

    lienaDistributedModule *module = new lienaDistributedModule(this->moduleIndex, this->globalParameter);

    this->connect(module, SIGNAL(lostConnection(uint32_t)), this, SLOT(launchRecoveryProcedure(uint32_t)));

    module->setTargetDeviceId(targetDeviceId);

    int ret = module->generateTransmissionChannel(motivated, addr, port);

    if(ret == 1){
        qDebug()<<"successful create a distributed module"<<targetDeviceId<<"with a transmission channel";
        this->distributedModules.append(module);
        this->moduleIndex ++;
        return true;
    }
    else{
        qDebug()<<"failed create a distributed module"<<targetDeviceId<<"with a transmission channel";
        qDebug()<<ret;
        return false;
    }
}

//! ------------------------------------------------------------------------------------------------------------------------------------------------------
//!
//! \brief lienaDistributedSystem::closeDistributedChannel
//! \param deviceId
//!
void lienaDistributedSystem::closeDistributedChannel(uint32_t deviceId){

    for(int i = 0;i<this->distributedModules.count(); i++){
        lienaDistributedModule* mdles = this->distributedModules.at(i);
        if(mdles->getDeviceId() == deviceId){
            qDebug()<<"lienaDistributedSystem |closeDistributedChannel deviceId:"<<deviceId;
            mdles->closeDistributedChannel();
        }
    }
}

//! ------------------------------------------------------------------------------------------------------------------------------------------------------
//!
//! \brief lienaDistributedSystem::init
//!
void lienaDistributedSystem::init(){
    this->clear();
}

//! ------------------------------------------------------------------------------------------------------------------------------------------------------
//!
//! \brief lienaDistributedSystem::clear
//!
void lienaDistributedSystem::clear(){
    this->moduleIndex = 0;
    this->distributedModules.clear();
}

//! ------------------------------------------------------------------------------------------------------------------------------------------------------
//!
//! \brief lienaDistributedSystem::append
//! \param module
//!
void lienaDistributedSystem::append(lienaDistributedModule*module){
    this->distributedModules.append(module);
}

//! ------------------------------------------------------------------------------------------------------------------------------------------------------
//!
//! \brief lienaDistributedSystem::getModuleCount
//! \return
//!
int lienaDistributedSystem::getModuleCount(){
    return this->distributedModules.size();
}

//! -----------------------------------------------------------------------------------------------------------------------------------------------------
//!
//! \brief lienaDistributedSystem::getModuleIndexByID
//! \return
//!
int lienaDistributedSystem::getModuleIndexByID(int deviceId){
    int ret = -1;
    int len = getModuleCount();
    for(int i =0; i< len; i++){
        if(this->distributedModules.at(i)->getDeviceId() == deviceId){
            ret = this->distributedModules.at(i)->getIndex();
        }
    }
    return ret;
}

//! -------------------------------------------------------------------------------------------------------------------------------------------------------
//!
//! \brief lienaDistributedSystem::setDeiveIDByIndex
//! \param index
//! \param deviceId
//!
void lienaDistributedSystem::setDeiveIDByIndex(int index, int deviceId){
    this->distributedModules.at(index)->setTargetDeviceId(deviceId);
}
