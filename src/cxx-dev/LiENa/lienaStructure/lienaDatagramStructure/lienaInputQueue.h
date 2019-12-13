#ifndef IGTINPUTQUEUE_H
#define IGTINPUTQUEUE_H

#include <QList>
#include <QByteArray>
#include <QDebug>
#include <QMutex>
#include <lienaDatagram.h>


class lienaInputQueue
{
public:
    void append(lienaDatagram *datagram);
    void print();
    lienaDatagram* getFrontDatagram();
    void deleteFrontDatagram();
    int getLength();
    void clear();

private:
    QMutex mutex;
    QList<lienaDatagram *> inputQueue;

public:
    lienaInputQueue();
};

#endif // IGTINPUTQUEUE_H
