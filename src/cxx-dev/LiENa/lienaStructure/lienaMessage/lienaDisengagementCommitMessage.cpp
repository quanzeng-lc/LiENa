#include "lienaDisengagementCommitMessage.h"


/**
 * @brief lienaDisengagementCommitMessage::lienaDisengagementCommitMessage
 * @param messageId
 * @param targetId
 * @param timestampes
 * @param DLC
 */
lienaDisengagementCommitMessage::lienaDisengagementCommitMessage(uint64_t messageId,
                                                                 uint32_t targetId,
                                                                 uint32_t originId,
                                                                 uint64_t timestampes,
                                                                 uint32_t DLC) : lienaMessage(messageId, targetId, originId,timestampes, DLC)
{


}

//! ----------------------------------------------------------------------------------------------
//!
//! \brief lienaDisengagementCommitMessage::~lienaDisengagementCommitMessage
//!
lienaDisengagementCommitMessage::~lienaDisengagementCommitMessage(){

}
