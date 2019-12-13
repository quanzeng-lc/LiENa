#include "lienaHeartBeatMessage.h"


lienaHeartBeatMessage::lienaHeartBeatMessage(unsigned long long messageId,
                                             unsigned int targetId,
                                             unsigned int originId,
                                             unsigned int timestampes,
                                             unsigned int DLC) : lienaMessage(messageId, targetId, originId, timestampes, DLC)
{

}
