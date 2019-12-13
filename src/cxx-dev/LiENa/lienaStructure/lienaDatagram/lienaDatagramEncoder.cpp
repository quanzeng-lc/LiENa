#include "lienaDatagramEncoder.h"


/**
 * @brief igtDatagramEncoder::igtDatagramEncoder
 */
lienaDatagramEncoder::lienaDatagramEncoder(lienaGlobal *globalParameter)
{
    this->globalParameter = globalParameter;
}

//! ---------------------------------------------------------------------------------------------------------------------------------
//!
//! \brief lienaDatagramEncoder::transformchannelClosedMessageToIgtDatagram
//! \param channelClosedMessage
//! \return
//!
lienaDatagram* lienaDatagramEncoder::transformchannelClosedMessageToIgtDatagram(lienaChannelClosedMessage* channelClosedMessage){
    lienaDatagram *datagram = nullptr;

    if(channelClosedMessage != nullptr){

        uint32_t globalDatagramSize = globalParameter->getGlobalDatagramSize();
        char* byteArray = new char[globalDatagramSize];
        uint64_t  messageId = channelClosedMessage->getMessageId();

        uint32_t targetId = channelClosedMessage->getTargetId();
        uint32_t timestampes = channelClosedMessage->getTimestampes();
        uint32_t dlc = channelClosedMessage->getDLC();

        byteArray[0] = (uchar)((0xff00000000000000&messageId)>>56);
        byteArray[1] = (uchar)((0x00ff000000000000&messageId)>>48);

        byteArray[2] = (uchar)((0x0000ff0000000000&messageId)>>40);
        byteArray[3] = (uchar)((0x000000ff00000000&messageId)>>32);

        byteArray[4] = (uchar)((0x00000000ff000000&messageId)>>24);
        byteArray[5] = (uchar)((0x0000000000ff0000&messageId)>>16);

        byteArray[6] = (uchar)((0x000000000000ff00&messageId)>>8);
        byteArray[7] = (uchar)((0x00000000000000ff&messageId));

        byteArray[8] =  (uchar)((0xff000000&targetId)>>24);
        byteArray[9] =  (uchar)((0x00ff0000&targetId)>>16);
        byteArray[10] = (uchar)((0x0000ff00&targetId)>>8);
        byteArray[11] = (uchar)((0x000000ff&targetId));

        byteArray[12] = (uchar)((0xff000000&timestampes)>>32);
        byteArray[13] = (uchar)((0x00ff0000&timestampes)>>24);
        byteArray[14] = (uchar)((0x0000ff00&timestampes)>>16);
        byteArray[15] = (uchar)((0x0000ff00&timestampes)>>8);
        byteArray[16] = (uchar)((0x000000ff&timestampes));

        byteArray[17] = (uchar)((0xff000000&dlc)>>24);
        byteArray[18] = (uchar)((0x00ff0000&dlc)>>16);
        byteArray[19] = (uchar)((0x0000ff00&dlc)>>8);
        byteArray[20] = (uchar)((0x000000ff&dlc));

        for(uint32_t i =21; i< globalDatagramSize; i++){
            byteArray[i] = '\0';
        }
        datagram = new lienaDatagram(globalDatagramSize, &byteArray);
    }
    return datagram;
}

//! ---------------------------------------------------------------------------------------------------------------------------------
//!
//! \brief lienaDatagramEncoder::transformDisengagementMessageToIgtDatagram
//! \param disengagementMessage
//! \return
//!
lienaDatagram* lienaDatagramEncoder::transformDisengagementMessageToIgtDatagram(lienaDisengagementMessage* disengagementMessage){
    lienaDatagram *datagram = nullptr;
    if(disengagementMessage != nullptr){

        uint32_t globalDatagramSize = globalParameter->getGlobalDatagramSize();

        char* byteArray = new char[globalDatagramSize];
        uint64_t  messageId = disengagementMessage->getMessageId();

        qDebug()<<messageId;

        uint32_t targetId = disengagementMessage->getTargetId();
        uint32_t timestampes = disengagementMessage->getTimestampes();
        uint32_t dlc = disengagementMessage->getDLC();

        byteArray[0] = (uchar)((0xff00000000000000&messageId)>>56);
        byteArray[1] = (uchar)((0x00ff000000000000&messageId)>>48);

        byteArray[2] = (uchar)((0x0000ff0000000000&messageId)>>40);
        byteArray[3] = (uchar)((0x000000ff00000000&messageId)>>32);

        byteArray[4] = (uchar)((0x00000000ff000000&messageId)>>24);
        byteArray[5] = (uchar)((0x0000000000ff0000&messageId)>>16);

        byteArray[6] = (uchar)((0x000000000000ff00&messageId)>>8);
        byteArray[7] = (uchar)((0x00000000000000ff&messageId));

        byteArray[8] =  (uchar)((0xff000000&targetId)>>24);
        byteArray[9] =  (uchar)((0x00ff0000&targetId)>>16);
        byteArray[10] = (uchar)((0x0000ff00&targetId)>>8);
        byteArray[11] = (uchar)((0x000000ff&targetId));

        byteArray[12] = (uchar)((0xff000000&timestampes)>>32);
        byteArray[13] = (uchar)((0x00ff0000&timestampes)>>24);
        byteArray[14] = (uchar)((0x0000ff00&timestampes)>>16);
        byteArray[15] = (uchar)((0x0000ff00&timestampes)>>8);
        byteArray[16] = (uchar)((0x000000ff&timestampes));

        byteArray[17] = (uchar)((0xff000000&dlc)>>24);
        byteArray[18] = (uchar)((0x00ff0000&dlc)>>16);
        byteArray[19] = (uchar)((0x0000ff00&dlc)>>8);
        byteArray[20] = (uchar)((0x000000ff&dlc));

        for(uint32_t i =21; i< globalDatagramSize; i++){
            byteArray[i] = '\0';
        }

        datagram = new lienaDatagram(globalDatagramSize, &byteArray);
    }
    return datagram;
}

//! ---------------------------------------------------------------------------------------------------------------------------------
//!
//! \brief lienaDatagramEncoder::transformDisengagementCommitMessageToIgtDatagram
//! \param disengagementCommitMessage
//! \return
//!
lienaDatagram* lienaDatagramEncoder::transformDisengagementCommitMessageToIgtDatagram(lienaDisengagementCommitMessage* disengagementCommitMessage){
    lienaDatagram *datagram = nullptr;

    if(disengagementCommitMessage != nullptr){
        uint32_t globalDatagramSize = globalParameter->getGlobalDatagramSize();
        char* byteArray = new char[globalDatagramSize];

        uint64_t messageId   = disengagementCommitMessage->getMessageId();
        uint32_t targetId    = disengagementCommitMessage->getTargetId();
        uint32_t originId    = disengagementCommitMessage->getOriginId();
        uint64_t timestampes = disengagementCommitMessage->getTimestampes();
        uint32_t dlc         = disengagementCommitMessage->getDLC();

        //-------------------------------message id-----------------------

        byteArray[0] = (uchar)((0xff00000000000000&messageId)>>56);
        byteArray[1] = (uchar)((0x00ff000000000000&messageId)>>48);
        byteArray[2] = (uchar)((0x0000ff0000000000&messageId)>>40);
        byteArray[3] = (uchar)((0x000000ff00000000&messageId)>>32);
        byteArray[4] = (uchar)((0x00000000ff000000&messageId)>>24);
        byteArray[5] = (uchar)((0x0000000000ff0000&messageId)>>16);
        byteArray[6] = (uchar)((0x000000000000ff00&messageId)>>8);
        byteArray[7] = (uchar)((0x00000000000000ff&messageId));

        //-------------------------------target id-----------------------

        byteArray[8] =  (uchar)((0xff000000&targetId)>>24);
        byteArray[9] =  (uchar)((0x00ff0000&targetId)>>16);
        byteArray[10] = (uchar)((0x0000ff00&targetId)>>8);
        byteArray[11] = (uchar)((0x000000ff&targetId));

        //-------------------------------origin id-----------------------

        byteArray[12] =  (uchar)((0xff000000&originId)>>24);
        byteArray[13] =  (uchar)((0x00ff0000&originId)>>16);
        byteArray[14] = (uchar)((0x0000ff00&originId)>>8);
        byteArray[15] = (uchar)((0x000000ff&originId));

        //-------------------------------time stamps -----------------------

        byteArray[16] = (uchar)((0xff00000000000000&timestampes)>>56);
        byteArray[17] = (uchar)((0x00ff000000000000&timestampes)>>48);
        byteArray[18] = (uchar)((0x0000ff0000000000&timestampes)>>40);
        byteArray[19] = (uchar)((0x000000ff00000000&timestampes)>>32);
        byteArray[20] = (uchar)((0x00000000ff000000&timestampes)>>24);
        byteArray[21] = (uchar)((0x0000000000ff0000&timestampes)>>16);
        byteArray[22] = (uchar)((0x000000000000ff00&timestampes)>>8);
        byteArray[23] = (uchar)((0x00000000000000ff&timestampes));

        //-------------------------------dlc -----------------------------

        byteArray[24] =  (uchar)((0xff000000&dlc)>>24);
        byteArray[25] =  (uchar)((0x00ff0000&dlc)>>16);
        byteArray[26] = (uchar)((0x0000ff00&dlc)>>8);
        byteArray[27] = (uchar)((0x000000ff&dlc));

        //-------------------------------body -----------------------

        for(uint32_t i =28; i< globalDatagramSize; i++){
            byteArray[i] = '\0';
        }

        datagram = new lienaDatagram(globalDatagramSize, &byteArray);
    }
    return datagram;
}

//! ---------------------------------------------------------------------------------------------------------------------------------
//!
//! \brief lienaDatagramEncoder::transformReHandShakeCommitMessageToIgtDatagram
//! \return
//!
lienaDatagram* lienaDatagramEncoder::transformReHandShakeCommitMessageToIgtDatagram(lienaReHandshakeCommitMessage* msg){
    lienaDatagram *datagram = nullptr;
    if(msg != nullptr){

        uint32_t globalDatagramSize = globalParameter->getGlobalDatagramSize();
        char* byteArray = new char[globalDatagramSize];

        uint64_t messageId   = msg->getMessageId();
        uint32_t targetId    = msg->getTargetId();
        uint32_t originId    = msg->getOriginId();
        uint64_t timestampes = msg->getTimestampes();
        uint32_t dlc         = msg->getDLC();

        //-------------------------------message id-----------------------

        byteArray[0] = (uchar)((0xff00000000000000&messageId)>>56);
        byteArray[1] = (uchar)((0x00ff000000000000&messageId)>>48);
        byteArray[2] = (uchar)((0x0000ff0000000000&messageId)>>40);
        byteArray[3] = (uchar)((0x000000ff00000000&messageId)>>32);
        byteArray[4] = (uchar)((0x00000000ff000000&messageId)>>24);
        byteArray[5] = (uchar)((0x0000000000ff0000&messageId)>>16);
        byteArray[6] = (uchar)((0x000000000000ff00&messageId)>>8);
        byteArray[7] = (uchar)((0x00000000000000ff&messageId));

        //-------------------------------target id-----------------------

        byteArray[8] =  (uchar)((0xff000000&targetId)>>24);
        byteArray[9] =  (uchar)((0x00ff0000&targetId)>>16);
        byteArray[10] = (uchar)((0x0000ff00&targetId)>>8);
        byteArray[11] = (uchar)((0x000000ff&targetId));

        //-------------------------------origin id-----------------------

        byteArray[12] =  (uchar)((0xff000000&originId)>>24);
        byteArray[13] =  (uchar)((0x00ff0000&originId)>>16);
        byteArray[14] = (uchar)((0x0000ff00&originId)>>8);
        byteArray[15] = (uchar)((0x000000ff&originId));

        //-------------------------------time stamps -----------------------

        byteArray[16] = (uchar)((0xff00000000000000&timestampes)>>56);
        byteArray[17] = (uchar)((0x00ff000000000000&timestampes)>>48);
        byteArray[18] = (uchar)((0x0000ff0000000000&timestampes)>>40);
        byteArray[19] = (uchar)((0x000000ff00000000&timestampes)>>32);
        byteArray[20] = (uchar)((0x00000000ff000000&timestampes)>>24);
        byteArray[21] = (uchar)((0x0000000000ff0000&timestampes)>>16);
        byteArray[22] = (uchar)((0x000000000000ff00&timestampes)>>8);
        byteArray[23] = (uchar)((0x00000000000000ff&timestampes));

        //-------------------------------dlc -----------------------------

        byteArray[24] =  (uchar)((0xff000000&dlc)>>24);
        byteArray[25] =  (uchar)((0x00ff0000&dlc)>>16);
        byteArray[26] = (uchar)((0x0000ff00&dlc)>>8);
        byteArray[27] = (uchar)((0x000000ff&dlc));

        //-------------------------------body -----------------------

        for(uint32_t i =28; i< globalDatagramSize; i++){
            byteArray[i] = '\0';
        }

        datagram = new lienaDatagram(globalDatagramSize, &byteArray);
    }
    return datagram;
}

//! ---------------------------------------------------------------------------------------------------------------------------------
//!
//! \brief lienaDatagramEncoder::transformReHandShakeMessageToIgtDatagram
//! \param msg
//! \return
//!
lienaDatagram* lienaDatagramEncoder::transformReHandShakeMessageToIgtDatagram(lienaReHandshakeMessage* msg){
    lienaDatagram *datagram = nullptr;

    if(msg != nullptr){

        uint32_t globalDatagramSize = globalParameter->getGlobalDatagramSize();
        char* byteArray = new char[globalDatagramSize];

        uint64_t messageId   = msg->getMessageId();
        uint32_t targetId    = msg->getTargetId();
        uint32_t originId    = msg->getOriginId();
        uint64_t timestampes = msg->getTimestampes();
        uint32_t dlc         = msg->getDLC();

        //-------------------------------message id-----------------------

        byteArray[0] = (uchar)((0xff00000000000000&messageId)>>56);
        byteArray[1] = (uchar)((0x00ff000000000000&messageId)>>48);
        byteArray[2] = (uchar)((0x0000ff0000000000&messageId)>>40);
        byteArray[3] = (uchar)((0x000000ff00000000&messageId)>>32);
        byteArray[4] = (uchar)((0x00000000ff000000&messageId)>>24);
        byteArray[5] = (uchar)((0x0000000000ff0000&messageId)>>16);
        byteArray[6] = (uchar)((0x000000000000ff00&messageId)>>8);
        byteArray[7] = (uchar)((0x00000000000000ff&messageId));

        //-------------------------------target id-----------------------

        byteArray[8] =  (uchar)((0xff000000&targetId)>>24);
        byteArray[9] =  (uchar)((0x00ff0000&targetId)>>16);
        byteArray[10] = (uchar)((0x0000ff00&targetId)>>8);
        byteArray[11] = (uchar)((0x000000ff&targetId));

        //-------------------------------origin id-----------------------

        byteArray[12] =  (uchar)((0xff000000&originId)>>24);
        byteArray[13] =  (uchar)((0x00ff0000&originId)>>16);
        byteArray[14] = (uchar)((0x0000ff00&originId)>>8);
        byteArray[15] = (uchar)((0x000000ff&originId));

        //-------------------------------time stamps -----------------------

        byteArray[16] = (uchar)((0xff00000000000000&timestampes)>>56);
        byteArray[17] = (uchar)((0x00ff000000000000&timestampes)>>48);
        byteArray[18] = (uchar)((0x0000ff0000000000&timestampes)>>40);
        byteArray[19] = (uchar)((0x000000ff00000000&timestampes)>>32);
        byteArray[20] = (uchar)((0x00000000ff000000&timestampes)>>24);
        byteArray[21] = (uchar)((0x0000000000ff0000&timestampes)>>16);
        byteArray[22] = (uchar)((0x000000000000ff00&timestampes)>>8);
        byteArray[23] = (uchar)((0x00000000000000ff&timestampes));

        //-------------------------------dlc -----------------------------

        byteArray[24] =  (uchar)((0xff000000&dlc)>>24);
        byteArray[25] =  (uchar)((0x00ff0000&dlc)>>16);
        byteArray[26] = (uchar)((0x0000ff00&dlc)>>8);
        byteArray[27] = (uchar)((0x000000ff&dlc));

        //-------------------------------body -----------------------

        for(uint32_t i =28; i< globalDatagramSize; i++){
            byteArray[i] = '\0';
        }

        datagram = new lienaDatagram(globalDatagramSize, &byteArray);
    }
    return datagram;
}

//! ---------------------------------------------------------------------------------------------------------------------------------
//!
//! \brief lienaDatagramEncoder::transformChannelReOpenedMessageToIgtDatagram
//! \param msg
//! \return
//!
lienaDatagram* lienaDatagramEncoder::transformChannelReOpenedMessageToIgtDatagram(lienaChannelReOpened* msg){
    lienaDatagram *datagram = nullptr;

    if(msg != nullptr){

        uint32_t globalDatagramSize = globalParameter->getGlobalDatagramSize();
        char* byteArray = new char[globalDatagramSize];

        uint64_t messageId   = msg->getMessageId();
        uint32_t targetId    = msg->getTargetId();
        uint32_t originId    = msg->getOriginId();
        uint64_t timestampes = msg->getTimestampes();
        uint32_t dlc         = msg->getDLC();

        //-------------------------------message id-----------------------

        byteArray[0] = (uchar)((0xff00000000000000&messageId)>>56);
        byteArray[1] = (uchar)((0x00ff000000000000&messageId)>>48);
        byteArray[2] = (uchar)((0x0000ff0000000000&messageId)>>40);
        byteArray[3] = (uchar)((0x000000ff00000000&messageId)>>32);
        byteArray[4] = (uchar)((0x00000000ff000000&messageId)>>24);
        byteArray[5] = (uchar)((0x0000000000ff0000&messageId)>>16);
        byteArray[6] = (uchar)((0x000000000000ff00&messageId)>>8);
        byteArray[7] = (uchar)((0x00000000000000ff&messageId));

        //-------------------------------target id-----------------------

        byteArray[8] =  (uchar)((0xff000000&targetId)>>24);
        byteArray[9] =  (uchar)((0x00ff0000&targetId)>>16);
        byteArray[10] = (uchar)((0x0000ff00&targetId)>>8);
        byteArray[11] = (uchar)((0x000000ff&targetId));

        //-------------------------------origin id-----------------------

        byteArray[12] =  (uchar)((0xff000000&originId)>>24);
        byteArray[13] =  (uchar)((0x00ff0000&originId)>>16);
        byteArray[14] = (uchar)((0x0000ff00&originId)>>8);
        byteArray[15] = (uchar)((0x000000ff&originId));

        //-------------------------------time stamps -----------------------

        byteArray[16] = (uchar)((0xff00000000000000&timestampes)>>56);
        byteArray[17] = (uchar)((0x00ff000000000000&timestampes)>>48);
        byteArray[18] = (uchar)((0x0000ff0000000000&timestampes)>>40);
        byteArray[19] = (uchar)((0x000000ff00000000&timestampes)>>32);
        byteArray[20] = (uchar)((0x00000000ff000000&timestampes)>>24);
        byteArray[21] = (uchar)((0x0000000000ff0000&timestampes)>>16);
        byteArray[22] = (uchar)((0x000000000000ff00&timestampes)>>8);
        byteArray[23] = (uchar)((0x00000000000000ff&timestampes));

        //-------------------------------dlc -----------------------------

        byteArray[24] =  (uchar)((0xff000000&dlc)>>24);
        byteArray[25] =  (uchar)((0x00ff0000&dlc)>>16);
        byteArray[26] = (uchar)((0x0000ff00&dlc)>>8);
        byteArray[27] = (uchar)((0x000000ff&dlc));

        //-------------------------------body -----------------------

        for(uint32_t i =28; i< globalDatagramSize; i++){
            byteArray[i] = '\0';
        }

        datagram = new lienaDatagram(globalDatagramSize, &byteArray);
    }
    return datagram;
}

lienaDatagram* lienaDatagramEncoder::encode_customized_message(lienaCustomizedMessage* customizedMessage){
    lienaDatagram *datagram = nullptr;
    if(customizedMessage != nullptr){
        uint32_t globalDatagramSize = globalParameter->getGlobalDatagramSize();
        char* byteArray = new char[globalDatagramSize];

        uint64_t messageId   = customizedMessage->getMessageId();
        uint32_t targetId    = customizedMessage->getTargetId();
        uint32_t originId    = customizedMessage->getOriginId();
        uint64_t timestampes = customizedMessage->getTimestampes();
        uint32_t dlc         = customizedMessage->getDLC();

        //-------------------------------message id-----------------------

        byteArray[0] = (uchar)((0xff00000000000000&messageId)>>56);
        byteArray[1] = (uchar)((0x00ff000000000000&messageId)>>48);
        byteArray[2] = (uchar)((0x0000ff0000000000&messageId)>>40);
        byteArray[3] = (uchar)((0x000000ff00000000&messageId)>>32);
        byteArray[4] = (uchar)((0x00000000ff000000&messageId)>>24);
        byteArray[5] = (uchar)((0x0000000000ff0000&messageId)>>16);
        byteArray[6] = (uchar)((0x000000000000ff00&messageId)>>8);
        byteArray[7] = (uchar)((0x00000000000000ff&messageId));

        //-------------------------------target id-----------------------

        byteArray[8] =  (uchar)((0xff000000&targetId)>>24);
        byteArray[9] =  (uchar)((0x00ff0000&targetId)>>16);
        byteArray[10] = (uchar)((0x0000ff00&targetId)>>8);
        byteArray[11] = (uchar)((0x000000ff&targetId));

        //-------------------------------origin id-----------------------

        byteArray[12] =  (uchar)((0xff000000&originId)>>24);
        byteArray[13] =  (uchar)((0x00ff0000&originId)>>16);
        byteArray[14] = (uchar)((0x0000ff00&originId)>>8);
        byteArray[15] = (uchar)((0x000000ff&originId));

        //-------------------------------time stamps -----------------------

        byteArray[16] = (uchar)((0xff00000000000000&timestampes)>>56);
        byteArray[17] = (uchar)((0x00ff000000000000&timestampes)>>48);
        byteArray[18] = (uchar)((0x0000ff0000000000&timestampes)>>40);
        byteArray[19] = (uchar)((0x000000ff00000000&timestampes)>>32);
        byteArray[20] = (uchar)((0x00000000ff000000&timestampes)>>24);
        byteArray[21] = (uchar)((0x0000000000ff0000&timestampes)>>16);
        byteArray[22] = (uchar)((0x000000000000ff00&timestampes)>>8);
        byteArray[23] = (uchar)((0x00000000000000ff&timestampes));

        //-------------------------------dlc -----------------------------

        byteArray[24] =  (uchar)((0xff000000&dlc)>>24);
        byteArray[25] =  (uchar)((0x00ff0000&dlc)>>16);
        byteArray[26] = (uchar)((0x0000ff00&dlc)>>8);
        byteArray[27] = (uchar)((0x000000ff&dlc));

        //-------------------------------body -----------------------

        uint8_t *v = customizedMessage->get_message_body();

        for(uint32_t i =28; i< globalDatagramSize; i++){
            byteArray[i] = v[i-28];
        }

        datagram = new lienaDatagram(globalDatagramSize, &byteArray);

    }
    return datagram;
}

//! ---------------------------------------------------------------------------------------------------------------------------------
//!
//! \brief lienaDatagramEncoder::transformHandShakeCommitMessageToIgtDatagram
//! \param handshakeCommitMessage
//! \return
//!
lienaDatagram* lienaDatagramEncoder::transformHandShakeCommitMessageToIgtDatagram(lienaHandShakeCommitMessage* handshakeCommitMessage){
    lienaDatagram *datagram = nullptr;
    if(handshakeCommitMessage != nullptr){

        uint32_t globalDatagramSize = globalParameter->getGlobalDatagramSize();
        char* byteArray = new char[globalDatagramSize];

        uint64_t messageId   = handshakeCommitMessage->getMessageId();
        uint32_t targetId    = handshakeCommitMessage->getTargetId();
        uint32_t originId    = handshakeCommitMessage->getOriginId();
        uint64_t timestampes = handshakeCommitMessage->getTimestampes();
        uint32_t dlc         = handshakeCommitMessage->getDLC();

        //-------------------------------message id-----------------------

        byteArray[0] = (uchar)((0xff00000000000000&messageId)>>56);
        byteArray[1] = (uchar)((0x00ff000000000000&messageId)>>48);
        byteArray[2] = (uchar)((0x0000ff0000000000&messageId)>>40);
        byteArray[3] = (uchar)((0x000000ff00000000&messageId)>>32);
        byteArray[4] = (uchar)((0x00000000ff000000&messageId)>>24);
        byteArray[5] = (uchar)((0x0000000000ff0000&messageId)>>16);
        byteArray[6] = (uchar)((0x000000000000ff00&messageId)>>8);
        byteArray[7] = (uchar)((0x00000000000000ff&messageId));

        //-------------------------------target id-----------------------

        byteArray[8] =  (uchar)((0xff000000&targetId)>>24);
        byteArray[9] =  (uchar)((0x00ff0000&targetId)>>16);
        byteArray[10] = (uchar)((0x0000ff00&targetId)>>8);
        byteArray[11] = (uchar)((0x000000ff&targetId));

        //-------------------------------origin id-----------------------

        byteArray[12] =  (uchar)((0xff000000&originId)>>24);
        byteArray[13] =  (uchar)((0x00ff0000&originId)>>16);
        byteArray[14] = (uchar)((0x0000ff00&originId)>>8);
        byteArray[15] = (uchar)((0x000000ff&originId));

        //-------------------------------time stamps -----------------------

        byteArray[16] = (uchar)((0xff00000000000000&timestampes)>>56);
        byteArray[17] = (uchar)((0x00ff000000000000&timestampes)>>48);
        byteArray[18] = (uchar)((0x0000ff0000000000&timestampes)>>40);
        byteArray[19] = (uchar)((0x000000ff00000000&timestampes)>>32);
        byteArray[20] = (uchar)((0x00000000ff000000&timestampes)>>24);
        byteArray[21] = (uchar)((0x0000000000ff0000&timestampes)>>16);
        byteArray[22] = (uchar)((0x000000000000ff00&timestampes)>>8);
        byteArray[23] = (uchar)((0x00000000000000ff&timestampes));

        //-------------------------------dlc -----------------------------

        byteArray[24] =  (uchar)((0xff000000&dlc)>>24);
        byteArray[25] =  (uchar)((0x00ff0000&dlc)>>16);
        byteArray[26] = (uchar)((0x0000ff00&dlc)>>8);
        byteArray[27] = (uchar)((0x000000ff&dlc));

        //-------------------------------body -----------------------

        for(uint32_t i =28; i< globalDatagramSize; i++){
            byteArray[i] = '\0';
        }

        datagram = new lienaDatagram(globalDatagramSize, &byteArray);
    }
    return datagram;
}

//! ---------------------------------------------------------------------------------------------------------------------------------
//!
//! \brief lienaDatagramEncoder::transformChannelOpenedMessageToIgtDatagram
//! \param openChannelMessage
//! \return
//!
lienaDatagram* lienaDatagramEncoder::transformChannelOpenedMessageToIgtDatagram(lienaChannelOpenedMessage* openChannelMessage){
    lienaDatagram *datagram = nullptr;

    if(openChannelMessage != nullptr){

        uint32_t globalDatagramSize = globalParameter->getGlobalDatagramSize();
        char* byteArray = new char[globalDatagramSize];

        uint64_t messageId   = openChannelMessage->getMessageId();
        uint32_t targetId    = openChannelMessage->getTargetId();
        uint32_t originId    = openChannelMessage->getOriginId();
        uint64_t timestampes = openChannelMessage->getTimestampes();
        uint32_t dlc         = openChannelMessage->getDLC();

        //-------------------------------message id-----------------------

        byteArray[0] = (uchar)((0xff00000000000000&messageId)>>56);
        byteArray[1] = (uchar)((0x00ff000000000000&messageId)>>48);
        byteArray[2] = (uchar)((0x0000ff0000000000&messageId)>>40);
        byteArray[3] = (uchar)((0x000000ff00000000&messageId)>>32);
        byteArray[4] = (uchar)((0x00000000ff000000&messageId)>>24);
        byteArray[5] = (uchar)((0x0000000000ff0000&messageId)>>16);
        byteArray[6] = (uchar)((0x000000000000ff00&messageId)>>8);
        byteArray[7] = (uchar)((0x00000000000000ff&messageId));

        //-------------------------------target id-----------------------

        byteArray[8] =  (uchar)((0xff000000&targetId)>>24);
        byteArray[9] =  (uchar)((0x00ff0000&targetId)>>16);
        byteArray[10] = (uchar)((0x0000ff00&targetId)>>8);
        byteArray[11] = (uchar)((0x000000ff&targetId));

        //-------------------------------origin id-----------------------

        byteArray[12] =  (uchar)((0xff000000&originId)>>24);
        byteArray[13] =  (uchar)((0x00ff0000&originId)>>16);
        byteArray[14] = (uchar)((0x0000ff00&originId)>>8);
        byteArray[15] = (uchar)((0x000000ff&originId));

        //-------------------------------time stamps -----------------------

        byteArray[16] = (uchar)((0xff00000000000000&timestampes)>>56);
        byteArray[17] = (uchar)((0x00ff000000000000&timestampes)>>48);
        byteArray[18] = (uchar)((0x0000ff0000000000&timestampes)>>40);
        byteArray[19] = (uchar)((0x000000ff00000000&timestampes)>>32);
        byteArray[20] = (uchar)((0x00000000ff000000&timestampes)>>24);
        byteArray[21] = (uchar)((0x0000000000ff0000&timestampes)>>16);
        byteArray[22] = (uchar)((0x000000000000ff00&timestampes)>>8);
        byteArray[23] = (uchar)((0x00000000000000ff&timestampes));

        //-------------------------------dlc -----------------------------

        byteArray[24] =  (uchar)((0xff000000&dlc)>>24);
        byteArray[25] =  (uchar)((0x00ff0000&dlc)>>16);
        byteArray[26] = (uchar)((0x0000ff00&dlc)>>8);
        byteArray[27] = (uchar)((0x000000ff&dlc));

        //-------------------------------body -----------------------

        for(uint32_t i =28; i< globalDatagramSize; i++){
            byteArray[i] = '\0';
        }

        datagram = new lienaDatagram(globalDatagramSize, &byteArray);
    }
    return datagram;
}

//! ---------------------------------------------------------------------------------------------------------------------------------
//!
//! \brief lienaDatagramEncoder::transformHelloMessageToIgtDatagram
//! \return
//!
lienaDatagram *lienaDatagramEncoder::transformHelloMessageToIgtDatagram(lienaHelloMessage* helloMessage){
    lienaDatagram *datagram = nullptr;

    if(helloMessage == nullptr){
        return datagram;
    }
    return datagram;
}

//! ---------------------------------------------------------------------------------------------------------------------------------
//!
//! \brief lienaDatagramEncoder::encode_handshake_message
//! \return
//!
lienaDatagram* lienaDatagramEncoder:: encode_handshake_message(lienaHandShakeMessage* handshakeMessage){
    lienaDatagram* datagram = nullptr;
    uint32_t globalDatagramSize = this->globalParameter->getGlobalDatagramSize();
    char* byteArray = new char[globalDatagramSize];

    if(handshakeMessage != nullptr){

        uint32_t globalDatagramSize = globalParameter->getGlobalDatagramSize();
        char* byteArray = new char[globalDatagramSize];

        uint64_t messageId   = handshakeMessage->getMessageId();
        uint32_t targetId    = handshakeMessage->getTargetId();
        uint32_t originId    = handshakeMessage->getOriginId();
        uint64_t timestampes = handshakeMessage->getTimestampes();
        uint32_t dlc         = handshakeMessage->getDLC();

        QString address = handshakeMessage->getIpAddresse();
        QStringList addrs = address.split(".");

        uint8_t addr1 = (uint8_t)addrs[0].toInt();
        uint8_t addr2 = (uint8_t)addrs[1].toInt();
        uint8_t addr3 = (uint8_t)addrs[2].toInt();
        uint8_t addr4 = (uint8_t)addrs[3].toInt();

        uint16_t port = (uint16_t)handshakeMessage->getPort();

        //-------------------------------message id-----------------------
        byteArray[0] = (uchar)((0xff00000000000000&messageId)>>56);
        byteArray[1] = (uchar)((0x00ff000000000000&messageId)>>48);
        byteArray[2] = (uchar)((0x0000ff0000000000&messageId)>>40);
        byteArray[3] = (uchar)((0x000000ff00000000&messageId)>>32);
        byteArray[4] = (uchar)((0x00000000ff000000&messageId)>>24);
        byteArray[5] = (uchar)((0x0000000000ff0000&messageId)>>16);
        byteArray[6] = (uchar)((0x000000000000ff00&messageId)>>8);
        byteArray[7] = (uchar)((0x00000000000000ff&messageId));

        //-------------------------------target id-----------------------

        byteArray[8] =  (uchar)((0xff000000&targetId)>>24);
        byteArray[9] =  (uchar)((0x00ff0000&targetId)>>16);
        byteArray[10] = (uchar)((0x0000ff00&targetId)>>8);
        byteArray[11] = (uchar)((0x000000ff&targetId));

        //-------------------------------origin id-----------------------

        byteArray[12] =  (uchar)((0xff000000&originId)>>24);
        byteArray[13] =  (uchar)((0x00ff0000&originId)>>16);
        byteArray[14] = (uchar)((0x0000ff00&originId)>>8);
        byteArray[15] = (uchar)((0x000000ff&originId));

        //-------------------------------time stamps -----------------------

        byteArray[16] = (uchar)((0xff00000000000000&timestampes)>>56);
        byteArray[17] = (uchar)((0x00ff000000000000&timestampes)>>48);
        byteArray[18] = (uchar)((0x0000ff0000000000&timestampes)>>40);
        byteArray[19] = (uchar)((0x000000ff00000000&timestampes)>>32);
        byteArray[20] = (uchar)((0x00000000ff000000&timestampes)>>24);
        byteArray[21] = (uchar)((0x0000000000ff0000&timestampes)>>16);
        byteArray[22] = (uchar)((0x000000000000ff00&timestampes)>>8);
        byteArray[23] = (uchar)((0x00000000000000ff&timestampes));

        //-------------------------------dlc -----------------------------

        byteArray[24] =  (uchar)((0xff000000&dlc)>>24);
        byteArray[25] =  (uchar)((0x00ff0000&dlc)>>16);
        byteArray[26] = (uchar)((0x0000ff00&dlc)>>8);
        byteArray[27] = (uchar)((0x000000ff&dlc));


        byteArray[28] =  (addr1);
        byteArray[29] =  (addr2);
        byteArray[30] =  (addr3);
        byteArray[31] =  (addr4);

        byteArray[32] = (uchar)((0x0000ff00&port)>>8);
        byteArray[33] = (uchar)((0x000000ff&port));

        for(uint32_t i =34; i< globalDatagramSize; i++){
            byteArray[i] = '\0';
        }

        //-------------------------------body -----------------------


        datagram = new lienaDatagram(globalDatagramSize, &byteArray);
       }
    return datagram;
}


//! ---------------------------------------------------------------------------------------------------------------------------------
//!
//! \brief igtDatagramAnalyser::transformCloseSessionMessageToIgtDatagram
//! \param datagram
//! \param injectionCommand
//!
lienaDatagram * lienaDatagramEncoder::transformCloseSessionMessageToIgtDatagram(lienaCloseSessionMessage * injectionCommand){
    lienaDatagram *datagram = nullptr;

    if(injectionCommand == nullptr){
        return datagram;
    }

//    datagram = new lienaDatagram();

//    datagram->setDataType(10);
//    datagram->setOrigineId(1);
//    datagram->setTargetId(1);
//    datagram->setTimeStampes(12);
//    datagram->setDLC(0);

//    QByteArray ret;
//    datagram->setIgtDatagramBody(ret);

    return datagram;
}


