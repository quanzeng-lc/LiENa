#ifndef FORCETORQUEVALUES_H
#define FORCETORQUEVALUES_H

#include "lienaDatagram.h"
#include "lienaMessage.h"

class lienaForceTorqueValues: public lienaMessage
{

private:
    double fx;
    double fy;
    double fz;
    double tx;
    double ty;
    double tz;
public:
    void setForceFeedbackX(double fx);
    void setForceFeedbackY(double fy);
    void setForceFeedbackZ(double fz);
    void setTorqueFeedbackX(double tx);
    void setTorqueFeedbackY(double ty);
    void setTorqueFeedbackZ(double tz);

    double getForceFeedbackX();
    double getForceFeedbackY();
    double getForceFeedbackZ();
    double getTorqueFeedbackX();
    double getTorqueFeedbackY();
    double getTorqueFeedbackZ();

    void transformIgtdatagramToForceFeedback(lienaDatagram* datagram);

public:
    lienaForceTorqueValues(unsigned int messageId,
                          unsigned int targetId,
                          unsigned int timestampes,
                          unsigned int DLC);
};

#endif // FORCETORQUEVALUES_H


