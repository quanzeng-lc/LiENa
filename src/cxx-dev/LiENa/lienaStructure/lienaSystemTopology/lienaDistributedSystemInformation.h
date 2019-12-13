#ifndef LIENADISTRIBUTEDSYSTEMINFORMATION_H
#define LIENADISTRIBUTEDSYSTEMINFORMATION_H


#include <QVector>
#include <QFile>
#include <lienaDevice.h>
#include <lienaSystemTopology.h>

class lienaDistributedSystemInformation
{

public:
    void loadDSIFile(QString path);
    unsigned int get_first_deviceId();
    QVector<unsigned int> get_deviceIds();
    //void addDeviceToDSI(lienaDevice* device)

private:
    QVector<lienaDevice *>devices;
    QVector<unsigned int> deviceIds;
    lienaSystemTopology *topology;

public:
    lienaDistributedSystemInformation();
};

#endif // LIENADISTRIBUTEDSYSTEMINFORMATION_H
