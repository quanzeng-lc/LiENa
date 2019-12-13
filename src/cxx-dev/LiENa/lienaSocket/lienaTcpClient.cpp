#include "lienaTcpClient.h"

/**
 * @brief lienaTcpClient::lienaTcpClient
 */
lienaTcpClient::lienaTcpClient(QString addr, unsigned short port){
    this->addr = addr;
    this->port = port;
}

//! ---------------------------------------------------------------------------------------------------------------------------------------------
//!
//! \brief lienaTcpClient::~lienaTcpClient
//!
lienaTcpClient::~lienaTcpClient(){
    delete &this->addr;
    delete &this->port;
}

//! ---------------------------------------------------------------------------------------------------------------------------------------------
//!
//! \brief lienaTcpClient::getSocketCom
//! \return
//!
SOCKET lienaTcpClient::getSocketCom(){
    return this->connectSocket;
}

//!
//! \brief lienaTcpClient::getLocalIp
//! \return
//!
QString lienaTcpClient::getLocalIp()
{
    QString localIp;
    char szText[256];
    //获取本机主机名称
    int iRet;
    iRet = gethostname(szText,256);
    int a = WSAGetLastError();
    if (iRet!=0)
    {
        printf("gethostname()  Failed!");
        return FALSE;
    }
    //通过主机名获取到地址信息
    HOSTENT *host = gethostbyname(szText);
    if (nullptr==host)
    {
        printf("gethostbyname() Failed!");
        return nullptr;
    }
    in_addr PcAddr;
    for (int i=0;;i++)
    {
        char *p = host->h_addr_list[i];
        if (nullptr==p)
        {
            break;
        }
        memcpy(&(PcAddr.S_un.S_addr),p,host->h_length);
        char*szIP = ::inet_ntoa(PcAddr);
        localIp = QString(QLatin1String(szIP));
    }
    return localIp;
}

//! ---------------------------------------------------------------------------------------------------------------------------------------------
//!
//! \brief lienaTcpClient::connectera
//!
int lienaTcpClient::connectera(int timeout){

    qDebug()<<"lienaTcpClient::connectera:"<<this->addr<<this->port;

    int ret = -1;

    //! transform ip and port to char*
    const char* addr_str = this->addr.toLatin1().data();

    WORD    wVersionRequested;
    WSADATA wsaData;
    int     err;
    wVersionRequested = MAKEWORD(2,2);//create 16bit data

    //(1)Load WinSock
    err =   WSAStartup(wVersionRequested,&wsaData); //load win socket
    if(err!=0){
       qDebug()<<"Load WinSock Failed!";
       return ret;
    }

    //(2)create socket
    connectSocket = socket(AF_INET,SOCK_STREAM,0);
    if(connectSocket == INVALID_SOCKET){
        qDebug()<<"socket() fail:"<<WSAGetLastError()<<endl;
        return ret;
    }

    // 设置为非阻塞的socket
    int iMode = 1;
    ioctlsocket(connectSocket, FIONBIO, (u_long FAR*)&iMode);

    //(3)IP
    SOCKADDR_IN addrSrv;
    addrSrv.sin_family = AF_INET;
    addrSrv.sin_addr.s_addr = inet_addr(addr_str);
    //this->port = 10704
    addrSrv.sin_port = htons(this->port);


    //! set timeout
    struct timeval tm;
    tm.tv_sec  = timeout;
    tm.tv_usec = 0;

    //(5)connect
    int ret_err;
    if (-1 != (ret_err = connect(connectSocket,(SOCKADDR*)&addrSrv,sizeof(SOCKADDR)))){
        qDebug()<<"ret error"<<ret_err;
        ret = 1;
    }
    else{
        fd_set set;
        FD_ZERO(&set);
        FD_SET(connectSocket, &set);

        if (select(-1, nullptr, &set, nullptr, &tm) <= 0){
            ret = -1;
        }
        else{
            int error = -1;
            int optLen = sizeof(int);
            getsockopt(connectSocket, SOL_SOCKET, SO_ERROR, (char*)&error, &optLen);

            if (0 != error){
                //qDebug()<<"error != 0"<<error;
                ret = -1;
            }
            else{
                //qDebug()<<"error = 0"<<error;
                ret = 1;
            }
        }
    }

    // 设置为非阻塞的socket
    iMode = 0;
    ioctlsocket(connectSocket, FIONBIO, (u_long FAR*)&iMode);

    return ret;

    /*//! disable nagle
    char value = 1;
    setsockopt(connectSocket, IPPROTO_TCP, TCP_NODELAY, &value, sizeof(value));*/
}



