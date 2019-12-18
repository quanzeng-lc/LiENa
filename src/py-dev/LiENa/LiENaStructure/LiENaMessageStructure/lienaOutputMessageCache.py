from LiENa.LiENaStructure.LiENaMessageStructure.lienaMessageQueue import LienaMessageQueue


class LienaOutputMessageCache:
    def __init__(self):
        self.outputMessageCache = list()

    def add_message_sequence(self):
        self.outputMessageCache.append([])

    def remove_message_cache_by_id(self, identifier):
        self.outputMessageCache.pop(identifier)

    def append_message_by_id(self, identifier, msg):
        self.outputMessageCache[identifier].append(msg)

    def get_latest_message_by_id(self, identifier):
        self.outputMessageCache[identifier].pop(-1)

    def get_front_message_by_id(self, identifier):
        self.outputMessageCache[identifier].pop(0)

    def generate_new_msg_seq(self, device_id):
        msgQ = LienaMessageQueue()
        msgQ.set_device_id(device_id)
        self.outputMessageCache.append(msgQ)
        return msgQ


