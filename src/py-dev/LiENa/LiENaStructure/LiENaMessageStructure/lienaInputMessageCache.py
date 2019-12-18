from LiENaStructure.LiENaMessageStructure.lienaMessageQueue import LienaMessageQueue


class LienaInputMessageCache:
    def __init__(self):
        self.inputMessageCache = list()

    def add_message_sequence(self):
        self.inputMessageCache.append([])

    def remove_message_cache_by_id(self, identifier):
        self.inputMessageCache.pop(identifier)

    def append_message_by_id(self, identifier, msg):
        self.inputMessageCache[identifier].append(msg)

    def get_latest_message_by_id(self, identifier):
        self.inputMessageCache[identifier].pop(-1)

    def get_front_message_by_id(self, identifier):
        self.inputMessageCache[identifier].pop(0)

    def generate_new_msg_seq(self, device_id):
        msgQ = LienaMessageQueue()
        msgQ.set_device_id(device_id)
        self.inputMessageCache.append(msgQ)
        return msgQ


