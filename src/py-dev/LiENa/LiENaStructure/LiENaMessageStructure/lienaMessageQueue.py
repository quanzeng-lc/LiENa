import threading


class LienaMessageQueue:
    def __init__(self):
        self.msgQueueLock = threading.Lock()
        self.msgQueue = list()
        self.index = 0
        self.empty = 0

    def set_device_id(self, device_id):
        self.device_id = device_id

    def get_device_id(self):
        return self.device_id

    def append(self, msg):
        self.msgQueueLock.acquire()
        self.msgQueue.append(msg)
        self.msgQueueLock.release()

    def is_empty(self):
        if len(self.msgQueue) > 0:
            self.empty = 1
        return self.empty

    def get_size(self):
        self.msgQueueLock.acquire()
        length = len(self.msgQueue)
        self.msgQueueLock.release()
        return length

    def pop_front(self):
        ret = None
        self.msgQueueLock.acquire()
        if len(self.msgQueue) > 0:
            ret = self.msgQueue.pop(0)
        self.msgQueueLock.release()
        return ret

    def pop_back(self):
        ret = None
        self.msgQueueLock.acquire()
        if len(self.msgQueue) > 0:
            ret = self.msgQueue.pop(len(self.msgQueue)-1)
        self.msgQueueLock.release()
        return ret

    def clear(self):
        self.msgQueueLock.acquire()
        self.msgQueue = []
        self.msgQueueLock.release()


