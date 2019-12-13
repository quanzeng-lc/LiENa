#include "lienaRecoveryTask.h"


lienaRecoveryTask::lienaRecoveryTask(QString addr, unsigned short port)
{
    this->flag = false;
    this->addr = addr;
    this->port = port;
    this->timeCount = 0;

}

//!------------------------------------------------------------------------------------------
//!
//! \brief lienaRecoveryTask::launch
//!
void lienaRecoveryTask::launch(){
    this->flag = true;
    this->start();
}

//!------------------------------------------------------------------------------------------
//!
//! \brief lienaRecoveryTask::run
//!
void lienaRecoveryTask::run(){
    this->timeCount = 1;

    while(this->flag){

        lienaTcpClient* test_connection = new lienaTcpClient(this->addr, this->port);
        int ret = test_connection->connectera(2);

        if(ret == 1){
            qDebug()<<"reconnect success";
            emit reconnectSuccess(test_connection->getSocketCom());
            break;
        }
        else if(ret == -1){
            qDebug()<<"reconnect error";
            this->timeCount += 1;
        }

        sleep(this->timeCount);
    }

}


