#include "lienaGlobal.h"

/*
 *
 *
    LARGE_INTEGER sendTime;
    QueryPerformanceCounter(&sendTime);
    double counter = (double)sendTime.QuadPart;
    QueryPerformanceFrequency(&sendTime);
    double fre = (doube)sendTime.QuadPart;
    uint32_t time = counter/fre;

    struct timeval sendTime;
    gettimeofday(&sendTime, 0);
    uint32_t time = sendTime.tv_sec *1000000 + sendTime.tv_usec;

*/

/**
 *
 * @brief lienaGlobal::lienaGlobal
 *
 */
lienaGlobal::lienaGlobal()
{
    this->init();
    this->globalClockOffset = 0;
}

//! ------------------------------------------------------------------------
//!
//! \brief lienaGlobal::getTimestamps
//! \return
//!
uint64_t lienaGlobal::getTimestamps(){
#ifdef WIN32
    LARGE_INTEGER counter;
    LARGE_INTEGER fre;
    QueryPerformanceCounter(&counter);
    QueryPerformanceFrequency(&fre);
    double frequency = fre.QuadPart/1000000000.0;
    double time = counter.QuadPart/frequency;

    return  ((uint64_t)time)%86400000000;

#elif linux
    struct timeval sendTime;
    gettimeofday(&sendTime, 0);
    uint64_t tus = (sendTime.tv_sec%86400)*1000000 + sendTime.tv_usec；

    return timeStamps

#endif
}

//! ------------------------------------------------------------------------------------
//!
//! \brief lienaGlobal::init
//!
void lienaGlobal::init(){
    this->globalDatagramSize = 1024;
    this->globalPort = 10704;
    this->localIpAddr = "192.168.1.172";

    this->initializeSystemPaths();
    this->dsi = new lienaDistributedSystemInformation();
    this->dsi->loadDSIFile("C://Users//cheng//Desktop//device.txt");
}

//! ------------------------------------------------------------------------------------
//!
//! \brief lienaGlobal::getDSI
//! \return
//!
lienaDistributedSystemInformation* lienaGlobal::getDSI(){
    return this->dsi;
}

//! ------------------------------------------------------------------------------------
//!
//! \brief lienaGlobal::setLocalIPAddr
//! \param localIpAddr
//!
void lienaGlobal::setLocalIPAddr(QString localIpAddr){
    this->localIpAddr = localIpAddr;
}

//! ------------------------------------------------------------------------------------
//!
//! \brief lienaGlobal::setLocalDeviceId
//! \param localDeviceId
//!
void lienaGlobal::setLocalDeviceId(uint32_t localDeviceId){
    this->localDeviceId = localDeviceId;
}

//! ------------------------------------------------------------------------------------
//!
//! \brief lienaGlobal::getLocalDeviceId
//! \return
//!
uint32_t lienaGlobal::getLocalDeviceId(){
    return this->localDeviceId;
}
//! ------------------------------------------------------------------------------------
//!
//! \brief lienaGlobal::getLocalIp
//! \return
//!
QString lienaGlobal::getLocalIp(){
    return this->localIpAddr;
}

//! ------------------------------------------------------------------------------------
//!
//! \brief lienaGlobal::setGlobalDatagramSize
//! \param globalDatagramSize
//!
void lienaGlobal::setGlobalDatagramSize(uint32_t globalDatagramSize){
    this->globalDatagramSize = globalDatagramSize;
}

//! ------------------------------------------------------------------------------------
//!
//! \brief lienaGlobal::setGlobalPort
//! \param globalPort
//!
void lienaGlobal::setGlobalPort(uint16_t globalPort){
    this->globalPort = globalPort;
}

//! ------------------------------------------------------------------------------------
//!
//!
//! \brief lienaGlobal::getGlobalDatagramSize
//! \return
//!
uint32_t lienaGlobal::getGlobalDatagramSize(){
    return  this->globalDatagramSize;
}

//! ------------------------------------------------------------------------------------
//!
//!
//! \brief lienaGlobal::getGlobalPort
//! \return
//!
uint16_t lienaGlobal::getGlobalPort(){
    return this->globalPort;
}

//!
//! \brief lienaGlobal::getCurrentTimeSeconds
//! \return
//!
uint32_t lienaGlobal::getCurrentTimeSeconds(){
    time_t rawtime;
    rawtime = time(NULL);
    uint32_t curentTimeInSeconds = (round(rawtime%86400)*1000000) - this->globalClockOffset;
    return curentTimeInSeconds;
}

//!---------------------------------------------------------------------------------------------------------
//!
//! \brief get_username
//! \return
//!
QString lienaGlobal::get_username(){
    QString name = qgetenv("USER");
        if (name.isEmpty())
            name = qgetenv("USERNAME");
    return name;
}

//!---------------------------------------------------------------------------------------------------------
//!
//! \brief SystemDispatcher::initializeSystemPaths
//!
void lienaGlobal::initializeSystemPaths(){

    //! ------create some folders by predefined paths as the workspaces of the application-------------------
    QString username = get_username();
    qDebug()<<"username:"<<username;
    if(username != "unknown") {
        //! define the software's deault path according to the os type

        #ifdef linux
        workspace_path = "/home/" + username + "/Documents/LiENa/";
        #elif _WIN64
        workspace_path = "C:\\Users\\" + username + "\\Documents\\LiENa\\";
        #elif __APPLE__
        workspace_path = "/Users/" + username + "/Documents/LiENa/";
        #endif
    }
    else{
        //qDebug()<<"exception";
        return;
    }

    QFileInfo folder(workspace_path);

    if(!folder.isDir()){
        QDir path;
        path.mkdir(workspace_path);
    }
}
