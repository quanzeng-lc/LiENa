from LiENa.LiENaStructure.LiENaMessageStructure.lienaMessageQueue import LienaMessageQueue


class LienaOutputMessageCache:
    def __init__(self):
        self.outputMessageCache = list()

    def add_message_sequence(self):
        self.outputMessageCache.append([])

    def remove_message_cache_by_id(self, identifier):
        self.outputMessageCache.pop(identifier)

    def write_message_by_index(self, index, msg):
        if len(self.outputMessageCache) > 0:
            self.outputMessageCache[index].append(msg)

    def get_latest_message_by_id(self, identifier):
        self.outputMessageCache[identifier].pop_back()

    def get_front_message_by_id(self, identifier):
        self.outputMessageCache[identifier].pop_front()

    def generate_new_msg_sequence(self, device_id):
        msgQ = LienaMessageQueue()
        msgQ.set_device_id(device_id)
        self.outputMessageCache.append(msgQ)
        return msgQ


