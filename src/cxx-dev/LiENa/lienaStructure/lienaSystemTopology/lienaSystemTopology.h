#ifndef LIENASYSTEMTOPOLOGY_H
#define LIENASYSTEMTOPOLOGY_H

#include <QVector>
#include <QDebug>
#include <QString>


class lienaSystemTopology
{

public:
    void init(int device_number);
    void set_value(int x, int y, bool value);
    void print();

    int doCheckGraphByLine(int y);
    bool get_value(int x, int y);
    int getGraphSize();

private:
    QVector<QVector<bool>> distributedSystemTopology;
    int device_number;

public:
    lienaSystemTopology();
};

#endif // LIENASYSTEMTOPOLOGY_H
