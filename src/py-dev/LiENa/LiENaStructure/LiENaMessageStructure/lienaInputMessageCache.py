from LiENa.LiENaStructure.LiENaMessageStructure.lienaMessageQueue import LienaMessageQueue


class LienaInputMessageCache:
    def __init__(self):
        self.inputMessageCache = []

    def get_sequence_count(self):
        return len(self.inputMessageCache)

    def remove_message_cache_by_id(self, identifier):
        self.inputMessageCache.pop(identifier)

    def append_message_by_id(self, identifier, msg):
        self.inputMessageCache[identifier].append(msg)

    def get_latest_message_by_index(self, identifier):
        #print("get_latest_message_by_index", self.inputMessageCache[identifier])
        self.inputMessageCache[identifier].pop_back()

    def get_front_message_by_index(self, identifier):
        self.inputMessageCache[identifier].pop_front()

    def generate_new_msg_seq(self, device_id):
        msgQ = LienaMessageQueue()
        msgQ.set_device_id(device_id)
        self.inputMessageCache.append(msgQ)
        return msgQ


