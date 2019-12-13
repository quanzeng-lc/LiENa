from LiENaStructure.LiENaMessage.LienaMessage import LienaMessage


class LienaChannelClosedMessage(LienaMessage):
    def __init__(self, message_id, target_id, origin_id, timestamps, dlc):
        LienaMessage.__init__(self, message_id, target_id, origin_id, timestamps, dlc)
        self.count = 0
