#ifndef LIENADIAGNOSISTASK_H
#define LIENADIAGNOSISTASK_H
#include "lienaHeartBeatMessage.h"

#include <QThread>
#include "QMutex"
#include "QDebug"


class lienaDiagnosisTask : public QThread{
    Q_OBJECT

public:
    void launch();
    void run();
    int get_sequence_length();
    void connexionFailedRecovered();
    void append(lienaHeartBeatMessage* msg);
    void deleteFrontDatagram();

    lienaHeartBeatMessage *get_latest_message();

private:
    bool rtFlag;
    bool connexionFailed;
    unsigned int rtPeriod;
    int connectionStatus;
    int heartBeatMessageCount;
    QList<lienaHeartBeatMessage*> heartBeatMessage;
    QMutex mutex;

signals:
    void lostConnection();

public:
    explicit lienaDiagnosisTask();
    ~lienaDiagnosisTask();

};

#endif // LIENADIAGNOSISTASK_H
