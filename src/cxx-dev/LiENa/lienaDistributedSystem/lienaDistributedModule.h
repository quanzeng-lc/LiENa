#ifndef LIENADISTRIBUTEDMODULE_H
#define LIENADISTRIBUTEDMODULE_H

#include <QVector>
#include <QDebug>

#ifdef Q_OS_MACOS
    #include <stdio.h>
    #include <netinet/in.h>
    #include <sys/socket.h>
    #include <arpa/inet.h>
#else
    #include <WinSock2.h>
#endif

#include "lienaGlobal.h"
#include "lienaInputQueue.h"
#include "lienaOutputQueue.h"
#include "lienaReceptionTask.h"
#include "lienaTransmissionTask.h"
#include "lienaDecodingTask.h"
#include "lienaEncodingTask.h"
#include "lienaTcpClient.h"
#include "lienaHeartBeatTask.h"
#include "lienaDiagnosisTask.h"
#include "lienaRecoveryTask.h"
#include "lienaNTPSynchronizationTask.h"


class lienaDistributedModule : public QObject
{
    Q_OBJECT

public:
    void init();
    void freeze();
    void printSelf();

    int getIndex();
    uint32_t getDeviceId();
    int getConnexionStatus();
    uint16_t getPort();
    QString getAddress();
    SOCKET getSocketForTransmission();
    SOCKET getSocketForReception();

    void setIndex(int index);
    void setTargetDeviceId(uint32_t targetDeviceId);
    void setOriginDeviceId(uint32_t originDeviceId);
    void setAddress(QString address);
    void setPort(uint16_t port);
    void setSocketForTransmission(SOCKET socketForTransmission);
    void setSocketForReception(SOCKET socketForReception);
    void setConnexionStatus(int connexionStatus);

    void configureMessageQueuePair(lienaMessageQueue *outputMessageQueue,lienaMessageQueue *inputMessageQueue);

    void generateReceptionChannel(SOCKET socketReception);
    void repairReceptionChannel(SOCKET socketReception);

    int generateTransmissionChannel(bool motivate, QString addr, uint16_t port);
    int repairTransmissionChannel();

    void closeDistributedChannel();
    void terminateModule();
    bool isInDiagnosis();
    lienaDatagram* encodeNetworkQualityMessage(lienaNetworkQualityMessage* networkQualityMessage);

public slots:
    void launchHandshakeCommitProcedure(lienaHandShakeMessage *handShakeMessage);
    void launchDisengagementCommitProcedure(lienaDisengagementMessage *);
    void launchModuleCloseProcedure(lienaDisengagementCommitMessage* msg);
    void launchModuleClose();
    void notifChannelOpen(lienaChannelOpenedMessage* msg);
    void channelOpened();
    void launchHeartBeatMessage(lienaHeartBeatMessage* msg);
    void launchRecoveryProcedure();
    void launchSessionRepaired();
    void launchRehandshakeCommitProcedure(lienaReHandshakeMessage* msg);
    void channnelRepaired();
    void launchChannelReopened(lienaChannelReOpened* msg);
    void repairTransmissionChannelWith(SOCKET socket);
    void returnBackNetworkQUalityMeaasge(lienaNetworkQualityMessage* msg);
    void restoreNetworkQualityMessage(lienaNetworkQualityMessage* msg);

private:
    bool executed;
    bool motivate;
    bool diagnosis;

    int index;
    uint32_t targetDeviceId;
    uint32_t originDeviceId;
    QString address;
    uint16_t port;
    SOCKET socketForTransmission;
    SOCKET socketForReception;
    int connexionStatus;
    int openChannelProcedure;
    int repairChannelProcedure;

    lienaReceptionTask *receptionTask;
    lienaTransmissionTask* transmissionTask;
    lienaRecoveryTask* recoveryTask;

    lienaInputQueue *inputQueue;
    lienaOutputQueue * outputQueue;

    lienaGlobal *globalParameter;

    lienaDecodingTask * decodingTask;
    lienaEncodingTask * encodingTask;
    lienaHeartBeatTask* heartBeatTask;
    lienaDiagnosisTask* diagnosisConnectionTask;
    lienaNTPSynchronizationTask* networkQualityTask;

    lienaTcpClient* tcpClient;

signals:
    void generateNewMessageSequence(uint32_t targetDeviceId);
    void lostConnection(uint32_t targetDeviceId);

public:
    lienaDistributedModule(int index, lienaGlobal *globalParameter);
    ~lienaDistributedModule();
};

#endif // LIENADISTRIBUTEDMODULE_H
