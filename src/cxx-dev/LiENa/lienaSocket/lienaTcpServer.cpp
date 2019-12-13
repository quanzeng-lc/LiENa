#include "lienaTcpServer.h"


/**
 * @brief lienaTcpServer::lienaTcpServer
 *
 */
lienaTcpServer::lienaTcpServer(lienaGlobal *globalParameter){
    this->globalParameter = globalParameter;
    this->isConnected = false;
    this->incomingClientCount = 0;
    this->flag = true;
}

//!----------------------------------------------------------------------------------------------------------------------------
//!
//! \brief lienaTcpServer::~lienaTcpServer
//!
lienaTcpServer::~lienaTcpServer(){
    delete &isConnected;
}

//!----------------------------------------------------------------------------------------------------------------------------
//!
//! \brief lienaTcpServer::getStatus
//! \return
//!
bool lienaTcpServer::getStatus(){
    return this->isConnected;
}

//! -------------------------------------------------------------------------------------------------------------------------------
//!
//! \brief lienaTcpServer::launchServer
//! \return
//!
bool lienaTcpServer::launchServer(){
    isConnected = this->init();
    //qDebug()<<"serveur lancer...| etat retour: "<<isConnected;
    //emit localIPDetect(ret);
    return isConnected;
}

//! ----------------------------------------------------------------------------------------------------------------------------
//!
//! \brief lienaTcpServer::init
//!
bool lienaTcpServer::init(){
    socketServer = INVALID_SOCKET;

    //!create WSADATA
    WSADATA wsaData;

    //! address info for the server to listen to
    struct addrinfo *result = nullptr;
    struct addrinfo hints;

    //! initialize Winsock
    int iResult;
    iResult = WSAStartup(MAKEWORD(2, 2), LPWSADATA(&wsaData));

    if(iResult != 0){
        qDebug()<<"WSAStartup failed with error"<<iResult;
        return false;
    }

    //! set address information
    ZeroMemory(&hints, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;//!Tcp connection
    hints.ai_flags = AI_PASSIVE;

    //! Resolve the server address and port
    QString temp = QString::number(10704, 10);
    QByteArray temp1 = temp.toLatin1();
    const char *port_str = temp1.data();

    iResult = getaddrinfo(nullptr, port_str, &hints, &result);

    if(iResult != 0){
        qDebug()<<"getaddrinfo failed with error:"<<iResult;
        WSACleanup();
        return false;
    }

    //!Create a　SOCKET for connecting to server
    socketServer = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
    if( SOCKET_ERROR == int(socketServer)){
        qDebug()<<"socket failed with error:"<<WSAGetLastError();
        freeaddrinfo(result);
        WSACleanup();
        return false;
    }

    //! bind
    if(SOCKET_ERROR == bind(socketServer, result->ai_addr, int(result->ai_addrlen))){
        qDebug()<<"bind error";
        freeaddrinfo(result);
        closesocket(socketServer);
        WSACleanup();
        return false;
    }

    //!no longer need address information
    freeaddrinfo(result);

    //! listen
    if(SOCKET_ERROR == listen(socketServer, 10)){
        qDebug()<<"listen error";
        closesocket(socketServer);
        WSACleanup();
        return false;
    }

    this->start();
    return true;
}

//! -------------------------------------------------------------------------------------------------------------------------------
//!
//! \brief lienaTcpServer::run
//!
void lienaTcpServer::run(){
    while(flag){
        SOCKET sClient;

        qDebug()<<"waiting..";
        sockaddr_in sin;
        int nAddrlen = sizeof(sin);
        if(INVALID_SOCKET == (sClient = ::accept(socketServer, (SOCKADDR *)&sin,&nAddrlen))){
            continue;
        }

        QString incomingClientAddr = QString(QLatin1String(inet_ntoa(sin.sin_addr)));
        qDebug()<<"new client arrived"<<incomingClientAddr;

        //! --------------------------------------------------------------------
        //! disable nagle
        //char value = 1;
        //setsockopt(sClient, IPPROTO_TCP, TCP_NODELAY, &value, sizeof(value));

        //! Set the mode of the scoket to be nonblocking
//        u_long iMode = 1;
//        int iResult = ioctlsocket(sClient, FIONBIO, &iMode);
//        if(iResult == SOCKET_ERROR){
//            printf("ioctlsocket failed with error: %d\n",WSAGetLastError());
//            closesocket(sClient);
//            WSACleanup();
//            continue;
//        }

        emit clientArrived(sClient, incomingClientAddr);
    }

}

//! -------------------------------------------------------------------------------------------------------------------------------
//!
//! \brief lienaTcpServer::stopServer
//! \return
//!
bool lienaTcpServer::stopServer(){
    if(isConnected){
        //! TODO stop task related and queues...........
        this->flag = false;
        this->isConnected = false;
        return true;
    }
    else{
        return false;
    }

    return false;
}

//! ------------------------------------------------------------------------------------------------------------------------------
//!
//! \brief lienaTcpServer::ipDetect
//! \return
//!
QString lienaTcpServer::ipDetect(){
    QString ret;
//    QList<QHostAddress> list = QNetworkInterface::allAddresses();
//    for(int nIter = 0; nIter < list.count(); nIter++){
//        if(list[nIter].protocol() == QAbstractSocket::IPv4Protocol && list[nIter] != QHostAddress(QHostAddress::LocalHost)){
//            ret = list[nIter].toString();
//            if(ret.contains("192")){
//                break;
//            }
//        }
//    }
    return ret;
}
