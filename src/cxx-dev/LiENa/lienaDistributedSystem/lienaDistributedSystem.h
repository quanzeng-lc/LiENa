#ifndef LIENADISTRIBUTEDSYSTEM_H
#define LIENADISTRIBUTEDSYSTEM_H

#include <QVector>
#include "QObject"
#include "lienaDistributedModule.h"
#include "lienaDistributedSystemInformation.h"
#include "lienaGlobal.h"


class lienaDistributedSystem : public QObject
{
    Q_OBJECT

public:
    void init();
    void clear();
    void configureMessageQueuePairBy(uint32_t deviceID, lienaMessageQueue *outputMessageQueue,lienaMessageQueue *inputMessageQueue);
    void setRoboticSystemModule(QVector<uint32_t> deviceIDs);
    void createDistributedModuleForReceptionChannel(SOCKET socketReception);
    bool createDistributedModuleWithTransmissionChannel(uint32_t deviceID, bool motivated, QString addr, unsigned short port);
    void generateReceptionChannelByAddr(SOCKET socketReception, QString addr);
    void append(lienaDistributedModule* module);
    void setDeiveIDByIndex(int index, int deviceId);

    int getModuleIndexByID(int deviceId);
    int getModuleCount();

    bool checkModuleByAddr(QString addr);
    void closeDistributedChannel(uint32_t deviceId);

private:
    int moduleIndex;
    lienaGlobal *globalParameter;
    QVector<lienaDistributedModule*> distributedModules;

    QVector<uint32_t> globalDeviceIDs;

signals:
    void generateNewMessageSequence(uint32_t deviceId);

public slots:
    void launchRecoveryProcedure(uint32_t deviceId);
    void notifyGenerateNewMessageSequence(uint32_t deviceId);

public:
    lienaDistributedSystem(lienaGlobal *globalParameter);
    ~lienaDistributedSystem();
};

#endif // LIENADISTRIBUTEDSYSTEM_H
