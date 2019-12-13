#ifndef IGTOUTPUTQUEUE_H
#define IGTOUTPUTQUEUE_H
#include <QList>
#include <QByteArray>
#include "lienaDatagram.h"
#include <QMutex>
#include <QDebug>

class lienaOutputQueue
{
private:
    QList<lienaDatagram *> outputQueue;
    QMutex mutex;

public:
    lienaDatagram *getFrontDatagram();
    int getOutputQueueLength();
    void append(lienaDatagram *sendMessage);
    void deleteFrontDatagram();
    void clear();

 public:
    lienaOutputQueue();
    ~lienaOutputQueue();
};

#endif // IGTOUTPUTQUEUE_H
