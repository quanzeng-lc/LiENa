#include "lienaDistributedSystemInformation.h"


/**
 * @brief lienaDistributedSystemInformation::lienaDistributedSystemInformation
 */
lienaDistributedSystemInformation::lienaDistributedSystemInformation()
{
    topology = new lienaSystemTopology();
}

//! -----------------------------------------------------------------------------
//!
//! \brief lienaDistributedSystemInformation::get_first_deviceId
//! \return
//!
unsigned int lienaDistributedSystemInformation::get_first_deviceId(){
    lienaDevice* device = new lienaDevice();
    device = this->devices[0];
    unsigned int deviceId = device->get_device_id();
    return deviceId;
}

//! -----------------------------------------------------------------------------
//!
//! \brief lienaDistributedSystemInformation::get_deviceIds
//! \return
//!
QVector<unsigned int> lienaDistributedSystemInformation::get_deviceIds(){
    return this->deviceIds;
}

//! -----------------------------------------------------------------------------
//!
//! \brief lienaDistributedSystemInformation::doReadSYstemTopologyFile
//! \param path
//!
void lienaDistributedSystemInformation::loadDSIFile(QString path){
    qDebug()<<path;
    QFile file(path);
    if(!file.open(QIODevice::ReadOnly|QIODevice::Text)){
        qDebug()<<"read only";
    }

    QByteArray all = file.readAll();
    QString all_string(all);
    QStringList device_info = all_string.split("\n");

    for(int cpt =1; cpt<device_info.size(); cpt++){

        QString device_string(device_info.at(cpt));
        QStringList current_device_info = device_string.split(",");

        lienaDevice* device = new lienaDevice();
        device->set_device_index(current_device_info.at(0).toInt());
        device->set_device_id(current_device_info.at(1).toUInt());
        device->setAddr(current_device_info.at(2));
        device->set_connection_topology(current_device_info.at(3));
        this->devices.append(device);
        this->deviceIds.append(current_device_info.at(1).toUInt());
    }
}



