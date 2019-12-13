#ifndef LIENADEVICE_H
#define LIENADEVICE_H

#include <QString>
#include <QDebug>

class lienaDevice
{
private:
    int device_index;
    unsigned int device_id;
    unsigned int device_priority;

    unsigned char addr[4];
    unsigned short port;

    unsigned short transmission_task_period;
    unsigned short encoding_task_period;
    unsigned short reception_task_period;
    unsigned short decoding_task_period;

    QString connect_topology;

public:
    QString getAddr();
    void  setAddr(QString address);
    void  setAddr(uchar addrZero, uchar addrOne, uchar addrTwo, uchar addrThree);

    void set_device_index(int device_index);
    void set_device_id(unsigned int device_id);
    void set_device_priority(unsigned int device_priority);
    void set_port(unsigned short port);

    void set_transmission_period(unsigned short period);
    void set_encode_period(unsigned short period);
    void set_reception_period(unsigned short period);
    void set_decode_period(unsigned short period);

    unsigned short get_transimission_task();
    unsigned short get_decode_task();
    unsigned short get_encode_period();
    unsigned short get_reception_period();

    int get_device_index();
    unsigned int get_device_id();
    unsigned int get_device_priority();

    void set_connection_topology(QString connect_topology);

public:
    lienaDevice();
};

#endif // LIENADEVICE_H
