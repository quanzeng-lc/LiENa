import os
import can
import threading
import time

class socketCan(object):

    _instance_lock = threading.Lock()

    def __init__(self):

        #os.system('sudo ip link set can1 type can bitrate 1000000')
        #os.system('sudo ifconfig can1 up')
        self.canOne = can.interface.Bus(channel='can1', bustype='socketcan_ctypes')

    @classmethod
    def instance(cls, *args, **kwargs):
        if not hasattr(socketCan, "_instance"):
            with socketCan._instance_lock:
                if not hasattr(socketCan, "_instance"):
                    socketCan._instance = socketCan(*args, **kwargs)
        return socketCan._instance


    def sendMsg(self, canId, sendData):
        msg = can.Message(arbitration_id = canId, data = sendData, extended_id = False)
        self.canOne.send(msg)
        time.sleep(0.005)

    def receiveMsg(self):
        msg = self.canOne.recv(2.0)
        return msg

    def close(self):
        pass
        #os.system('sudo ifconfig can0 down')

"""
canId = 0x601
sendData1 = [0x22, 0x40, 0x60, 0x00, 0x06, 0x00, 0x00, 0x00]
sendData2 = [0x22, 0x40, 0x60, 0x00, 0x0F, 0x00, 0x00, 0x00]
sendData3 = [0x22, 0x60, 0x60, 0x00, 0x03, 0x00, 0x00, 0x00]
sendData4 = [0x22, 0xFF, 0x60, 0x00, 0xE8, 0x03, 0x00, 0x00]
sendData5 = [0x22, 0x40, 0x60, 0x00, 0x0F, 0x00, 0x00, 0x00]
sendData6 = [0x22, 0x40, 0x60, 0x00, 0x0F, 0x01, 0x00, 0x00]
sendData7 = [0x22, 0x40, 0x60, 0x00, 0x00, 0x00, 0x00, 0x00]
socketcan = socketCan.instance()
socketcan.sendMsg(canId, sendData1)
socketcan.sendMsg(canId, sendData2)
socketcan.sendMsg(canId, sendData3)
socketcan.sendMsg(canId, sendData4)
socketcan.sendMsg(canId, sendData5)
time.sleep(6)
socketcan.sendMsg(canId, sendData6)
socketcan.sendMsg(canId, sendData7)
"""
