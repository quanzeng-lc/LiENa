#ifndef IGTCOMMUNICATIONSTACK_H
#define IGTCOMMUNICATIONSTACK_H

#include <QObject>
#include <QVector>

#include "lienaGlobal.h"
#include "lienaDistributedSystem.h"
#include "lienaTcpServer.h"
#include "lienaTransmissionTask.h"
#include "lienaDecodingTask.h"
#include "lienaOutputQueue.h"
#include "lienaEncodingTask.h"
#include "lienaDatagramDecoder.h"
#include "lienaAddHelloMessageTask.h"
#include "lienaInputMessageCache.h"
#include "lienaOutputMessageCache.h"
#include "lienaDistributedSystemInformation.h"


/**
 * @brief The liena class aim to build an asynchronouse communication mechanism.
 *
 *        The software architecture include:
 *
 *           a global TCP server
 *
 */
class liena : public QObject{

    Q_OBJECT

public:
    //! ----------------------------------------------------------------------------------------
    //! core interface for developer to manipulate the liena middleware
    //! lauch global server module
    bool launch();

    //! terminate global server module
    bool terminate();

    //! register networked devices of a given distributed medical robot system
    void registerDevices();

    //! open/close real time data exchange session
    bool openSessionRequest(uint32_t deviceID, QString addr, uint16_t port=10704);
    void closeSessionRequest(uint32_t deviceID);

    //! get message cache reference to put/get message into message cache layer
    lienaInputMessageCache* getInputMessageCache();
    lienaOutputMessageCache* getOutputMessageCache();

    bool checkAvailability(uint32_t deviceID);
    void setLocalDeviceID(uint32_t deviceID);

    //! TODO
    //! lienaCustomizedMessage* readLatestInputMessageByID(uint32_t deviceID);
    //! void writeMessageByID(uint32_t deviceID, lienaCustomizedMessage* msg);
    //!
    //! void clearMessageCache();

public:
    //! local methode
    void init();// initialize principal modules
    void connexion();// qt signaux/slots
    void createMotivateModule(uint32_t deviceID, QString addr, uint16_t port);

private:
    lienaGlobal *globalParameter;                                                   //! parameters to be regulated while real time scheduling
    lienaInputMessageCache* inputMessageCache;
    lienaOutputMessageCache* outputMessageCache;
    lienaDistributedSystem *distributedSystem;                                      //! where to store each distributed module to communicate with an incomming device
    lienaTcpServer *tcpServer;                                                      //! local socket server wait for new incoming connection

public slots:
    void getSelfIp(QString addr);
    void createReceptionChannel(SOCKET socketReception, QString addr);
    void connectionEstablish();
    void generateNewMessageSequenceBy(uint32_t deviceID);

signals:
    void localIPDetect(QString addr);
    void newConnection(QString remoteIP);

public:
    liena();
    ~liena();
};

#endif // IGTCOMMUNICATIONSTACK_H
