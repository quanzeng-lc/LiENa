#ifndef LIENAMESSAGEQUEUE_H
#define LIENAMESSAGEQUEUE_H

#include "lienaCustomizedMessage.h"
#include <QMutex>


class lienaMessageQueue
{
public:
    void setDeviceId(uint32_t deviceID);
    uint32_t getDeviceId();
    int getSize();
    bool isEmpty();
    void clear();
    void append(lienaCustomizedMessage*msg);
    lienaCustomizedMessage* pop_front();
    lienaCustomizedMessage* pop_back();

private:
    uint32_t deviceID;
    QVector<lienaCustomizedMessage*>* messageSequence;
    QMutex mutex;

public:
    lienaMessageQueue();
};

#endif // LIENAMESSAGEQUEUE_H
