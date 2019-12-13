#ifndef LIENAINPUTMESSAGESEQUENCE_H
#define LIENAINPUTMESSAGESEQUENCE_H

#include <QMutex>
#include <QVector>
#include "lienaMessageQueue.h"


class lienaInputMessageCache
{

public:
    //! -----------------------------------------------------------------------------------------------
    //!
    //!                 methodes for control command original messages
    //!
    //! -----------------------------------------------------------------------------------------------
    void clearAllBuffers();
    bool exist(uint32_t deviceID);
    lienaMessageQueue * generateNewMessageSequence(uint32_t deviceID);
    void writeMessageByIndex(int index, lienaCustomizedMessage* msg);

private:
    //! -----------------------------------------------------------------------------------------------
    //!
    //!                 control instructions sequences
    //!
    //! -----------------------------------------------------------------------------------------------
    QVector<lienaMessageQueue*>* messageSequence;

    bool echantionnage;

public:
    lienaInputMessageCache();
};

#endif // LIENAINPUTMESSAGESEQUENCE_H
