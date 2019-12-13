#include "lienaSystemTopology.h"


/**
 * @brief lienaSystemTopology::lienaSystemTopology
 */
lienaSystemTopology::lienaSystemTopology()
{
    this->device_number = 0;
}

//! -----------------------------------------------------------------------------
//!
//! \brief lienaSystemTopology::getGraphSize
//! \return
//!
int lienaSystemTopology::getGraphSize(){
    return this->device_number;
}

//! -----------------------------------------------------------------------------
//!
//! \brief lienaSystemTopology::get_value
//! \param x
//! \param y
//! \return
//!
bool lienaSystemTopology::get_value(int x, int y){
    return this->distributedSystemTopology[x][y];
}

//! -----------------------------------------------------------------------------
//!
//! \brief lienaSystemTopology::doCheckGraphByLine
//! \param y
//! \return
//!
int lienaSystemTopology::doCheckGraphByLine(int y){
    int cpt = 0;
    for(int i = 0; i < device_number; i++){
        if(this->distributedSystemTopology[i][y]){
            cpt++;
        }
    }
    return cpt;
}

//! -----------------------------------------------------------------------------
//!
//! \brief lienaSystemTopology::init
//! \param device_number
//!
void lienaSystemTopology::init(int device_number){
    this->device_number = device_number;
    for(int i = 0; i < device_number; i++){
        QVector<bool> line;
        this->distributedSystemTopology.append(line);
        for(int j = 0; j < device_number; j++){
            this->distributedSystemTopology[i].append(false);
        }
    }
}

//! -----------------------------------------------------------------------------
//!
//! \brief lienaSystemTopology::set_value
//! \param x
//! \param y
//! \param value
//!
void lienaSystemTopology::set_value(int x, int y, bool value){
    this->distributedSystemTopology[x][y] = value;
}

//! -----------------------------------------------------------------------------
//!
//! \brief lienaSystemTopology::print
//!
void lienaSystemTopology::print(){
    for(int i = 0; i < device_number; i++){
        QString line = "";
        for(int j = 0; j < device_number; j++){
            line += this->distributedSystemTopology[i][j];
        }
        qDebug()<<line;
    }
}
