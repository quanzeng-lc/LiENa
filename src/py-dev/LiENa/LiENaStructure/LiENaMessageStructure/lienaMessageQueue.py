import threading

from RCPContext.LienaControlInstruction import LienaControlInstruction


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
            ret = self.msgQueue.pop(-1)
            ci = LienaControlInstruction()

            body = ret.get_value()

            gwts = 0
            if int(body[2]) == 0:
                gwts = -1 * (int(body[3]) * 256 + int(body[4]))
                ci.set_guidewire_translational_speed(gwts)
            elif int(body[2]) == 1:
                gwts = 1 * (int(body[3]) * 256 + int(body[4]))
                ci.set_guidewire_translational_speed(gwts)

            gwrs = 0
            if int(body[7]) == 0:
                gwrs = -1 * (int(body[8]) * 256 + int(body[9]))
                ci.set_guidewire_rotational_speed(gwrs)
            elif int(body[7]) == 1:
                gwrs = 1 * (int(body[8]) * 256 + int(body[9]))
                ci.set_guidewire_rotational_speed(gwrs)

            chrs = 0
            if int(body[13]) == 0:
                chrs = -1 * (int(body[14]) * 256 + int(body[15]))
                ci.set_catheter_translational_speed(chrs)
            elif int(body[13]) == 1:
                chrs = 1 * (int(body[14]) * 256 + int(body[15]))
                ci.set_catheter_translational_speed(chrs)
            print("notify_control_instruction:", gwts, gwrs, chrs)
        self.msgQueueLock.release()
        return ret

    def clear(self):
        self.msgQueueLock.acquire()
        self.msgQueue = []
        self.msgQueueLock.release()


