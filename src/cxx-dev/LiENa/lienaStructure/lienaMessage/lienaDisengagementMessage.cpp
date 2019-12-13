#include "lienaDisengagementMessage.h"


/**
 * @brief lienaDisengagementMessage::lienaDisengagementMessage
 */
lienaDisengagementMessage::lienaDisengagementMessage(unsigned long long messageId,
                                                     unsigned int targetId,
                                                     unsigned int originId,
                                                     unsigned long long timestampes,
                                                     unsigned int DLC) : lienaMessage(messageId, targetId, originId,timestampes, DLC)
{

}

//! -------------------------------------------------------------------------
//!
//! \brief lienaDisengagementMessage::~lienaDisengagementMessage
//!
lienaDisengagementMessage::~lienaDisengagementMessage(){

}
