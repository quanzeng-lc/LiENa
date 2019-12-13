#ifndef IGTADDHELLOMESSAGETASK_H
#define IGTADDHELLOMESSAGETASK_H

#include <QThread>
#include "lienaHelloMessage.h"

class lienaAddHelloMessageTask : public QThread
{
    Q_OBJECT
private:
    bool flag;
    int index;
public:
    void run();
public:
    lienaAddHelloMessageTask(int index);
};

#endif // IGTADDHELLOMESSAGETASK_H
