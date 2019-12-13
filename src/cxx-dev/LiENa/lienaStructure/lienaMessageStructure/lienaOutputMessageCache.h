#ifndef LIENAOUTPUTMESSAGESEQUENCE_H
#define LIENAOUTPUTMESSAGESEQUENCE_H

#include "OmegaPosition.h"
#include "lienaMessageQueue.h"
#include <QMutex>
#include <QVector>
#include <QDebug>
#include <QDateTime>


class lienaOutputMessageCache
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
    void writeMessageByIndex(int index, lienaCustomizedMessage*msg);

private:
    //! -----------------------------------------------------------------------------------------------
    //!
    //!                 control instructions sequences
    //!
    //! -----------------------------------------------------------------------------------------------
    QVector<lienaMessageQueue*>* messageSequence;

    bool echantionnage;

public:
    lienaOutputMessageCache();
};

#endif // LIENAOUTPUTMESSAGESEQUENCE_H
