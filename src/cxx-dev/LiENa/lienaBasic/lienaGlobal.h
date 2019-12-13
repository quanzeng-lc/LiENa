#ifndef LIENAGLOBAL_H
#define LIENAGLOBAL_H

#include <QString>
#include <QFileInfo>
#include <QDir>
#include "time.h"
#include "math.h"

#ifdef WIN32
    #include "Windows.h"
#elif linux
    #include <time.h>
#endif

#include "lienaDistributedSystemInformation.h"


class lienaGlobal
{
public:
    void init();
    void initializeSystemPaths();
    void setGlobalDatagramSize(uint32_t globalDatagramSize);
    void setGlobalPort(uint16_t globalPort);
    void setLocalIPAddr(QString localIpAddr);
    void setLocalDeviceId(uint32_t localDeviceId);

    uint16_t getGlobalPort();
    uint32_t getLocalDeviceId();
    uint32_t getGlobalDatagramSize();
    uint32_t getCurrentTimeSeconds();
    uint64_t getTimestamps();

    QString get_username();
    QString getLocalIp();

    lienaDistributedSystemInformation* getDSI();

private:
    QString workspace_path;
    lienaDistributedSystemInformation* dsi;

    uint32_t   globalDatagramSize;
    uint16_t globalPort;
    QString localIpAddr;
    uint32_t localDeviceId;
    uint32_t globalClockOffset;

public:
    lienaGlobal();
};

#endif // LIENAGLOBAL_H
