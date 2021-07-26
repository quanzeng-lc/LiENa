#from RCPControl.socketCan import socketCan
import sys
sys.path.append('./RCPControl/')
from socketCan import socketCan
import struct
#from socketCan import socketCan
import time


class maxonMotor(object):

    IsOpen = False

    def __init__(self, nodeId):
        self.nodeId = nodeId
        self.lead = 2
        self.resolution = 1000
        self.gear_ratio = 10
        self.direction = True

    def setDirection(self, direction):
        self.direction = direction

    def isSameDirection(self, value):
        if self.direction:
            return value
        else:
            return (-1 * value)

    def setParameter(self, resolution, gear, lead):
        self.lead = lead
        self.resolution = resolution
        self.gear_ratio = gear
    
    def transfromQcToPosition(self, qc):
        position = qc * self.lead / (self.resolution * self.gear_ratio);
        position = ((int)(position * pow(10, 3))) / pow(10, 3);
        return position;

    def transfromPositionToQc(self, position):
        qc = position * self.gear_ratio * self.resolution / self.lead;
        return qc;

    def transfromRPMToLineSp(self, rpm):
        lineSp = self.transfromQcToPosition(self.resolution * rpm / 60);
        return lineSp;

    def transfromLineSpToRPM(self, lineSp):
        rmp = self.transfromPositionToQc(lineSp) * 60/ self.resolution;
        return rmp;

    #sdo 
    def setProfileVelocityMode(self):
       canId = 0x600 + self.nodeId 
       sendData = list()
       sendData.append(0x22)
       sendData.append(0x60)
       sendData.append(0x60)
       sendData.append(0x00)
       sendData.append(0x03)
       sendData.append(0x00)
       sendData.append(0x00)
       sendData.append(0x00)
       socketCan.instance().sendMsg(canId, sendData)
       #socketCan.instance().receiveMsg()
       
    def enableMotor(self):
        #self.disableMotor()
        #time.sleep(0.5)
        canId = 0x600 + self.nodeId
        sendData = list()
        sendData.append(0x22)
        sendData.append(0x40)
        sendData.append(0x60)
        sendData.append(0x00)
        sendData.append(0x06)
        sendData.append(0x00)
        sendData.append(0x00)
        sendData.append(0x00)
        socketCan.instance().sendMsg(canId, sendData)
        #socketCan.instance().receiveMsg()
        time.sleep(0.05)

        canId = 0x600 + self.nodeId
        sendData = list()
        sendData.append(0x22)
        sendData.append(0x40)
        sendData.append(0x60)
        sendData.append(0x00)
        sendData.append(0x0F)
        sendData.append(0x00)
        sendData.append(0x00)
        sendData.append(0x00)
        socketCan.instance().sendMsg(canId, sendData)
        #socketCan.instance().receiveMsg()
    
    def disableMotor(self):
        canId = 0x600 + self.nodeId
        sendData = list()
        sendData.append(0x22)
        sendData.append(0x40)
        sendData.append(0x60)
        sendData.append(0x00)
        sendData.append(0x00)
        sendData.append(0x00)
        sendData.append(0x00)
        sendData.append(0x00)
        socketCan.instance().sendMsg(canId, sendData)
        #socketCan.instance().receiveMsg()
    
    ## mm/s
    def setProfileVelocityModeVelocity(self, setVelocity):
        canId = 0x600 + self.nodeId
        velocity = int(self.isSameDirection(self.transfromLineSpToRPM(setVelocity)))
        #print('velocity: ' ,velocity)
        velocityLowByte = 0x000000FF & velocity
        velocityHighByte = (0x0000FF00 & velocity)>>8
        velocityHighLowByte = (0x00FF0000 & velocity)>>16
        velocityHighHighByte = (0xFF000000 & velocity)>>24
        sendData = list()
        sendData.append(0x22)
        sendData.append(0xFF)
        sendData.append(0x60)
        sendData.append(0x00)
        sendData.append(velocityLowByte)
        sendData.append(velocityHighByte)
        sendData.append(velocityHighLowByte)
        sendData.append(velocityHighHighByte)
        socketCan.instance().sendMsg(canId, sendData)
        #socketCan.instance().receiveMsg()

    def profileVelocityModeStartMove(self):
        canId = 0x600 + self.nodeId
        sendData = list()
        sendData.append(0x22)
        sendData.append(0x40)
        sendData.append(0x60)
        sendData.append(0x00)
        sendData.append(0x0F)
        sendData.append(0x00)
        sendData.append(0x00)
        sendData.append(0x00)
        socketCan.instance().sendMsg(canId, sendData)
        #socketCan.instance().receiveMsg()

    def profileVelocityModeHalt(self):
        canId = 0x600 + self.nodeId
        sendData = list()
        sendData.append(0x22)
        sendData.append(0x40)
        sendData.append(0x60)
        sendData.append(0x00)
        sendData.append(0x0F)
        sendData.append(0x01)
        sendData.append(0x00)
        sendData.append(0x00)
        socketCan.instance().sendMsg(canId, sendData)
        #socketCan.instance().receiveMsg()

    def setProfilePositionMode(self):
        canId = 0x600 + self.nodeId 
        sendData = list()
        sendData.append(0x22)
        sendData.append(0x60)
        sendData.append(0x60)
        sendData.append(0x00)
        sendData.append(0x01)
        sendData.append(0x00)
        sendData.append(0x00)
        sendData.append(0x00)
        socketCan.instance().sendMsg(canId, sendData)
        #socketCan.instance().receiveMsg()

    def setTargetPosition(self, position):
        canId = 0x600 + self.nodeId
        qcPosition = int(self.isSameDirection(self.transfromPositionToQc(position)))
        #print('position: ' ,qcPosition)
        positionLowByte = 0x000000FF & qcPosition
        positionHighByte = (0x0000FF00 & qcPosition)>>8
        positionHighLowByte = (0x00FF0000 & qcPosition)>>16
        positionHighHighByte = (0xFF000000 & qcPosition)>>24
        sendData = list()
        sendData.append(0x22)
        sendData.append(0x7A)
        sendData.append(0x60)
        sendData.append(0x00)
        sendData.append(positionLowByte)
        sendData.append(positionHighByte)
        sendData.append(positionHighLowByte)
        sendData.append(positionHighHighByte)
        socketCan.instance().sendMsg(canId, sendData)
        #socketCan.instance().receiveMsg()

    def setProfilePositionModeVelocity(self, velocity):
        canId = 0x600 + self.nodeId
        profileVelocity = abs(int(self.isSameDirection(self.transfromLineSpToRPM(velocity))))
        #print('profileVelocity: ' ,profileVelocity)
        positionLowByte = 0x000000FF & profileVelocity
        positionHighByte = (0x0000FF00 & profileVelocity)>>8
        positionHighLowByte = (0x00FF0000 & profileVelocity)>>16
        positionHighHighByte = (0xFF000000 & profileVelocity)>>24
        sendData = list()
        sendData.append(0x23)
        sendData.append(0x81)
        sendData.append(0x60)
        sendData.append(0x00)
        sendData.append(positionLowByte)
        sendData.append(positionHighByte)
        sendData.append(positionHighLowByte)
        sendData.append(positionHighHighByte)
        """
        sendData.append(0xE8)
        sendData.append(0x03)
        sendData.append(0x00)
        sendData.append(0x00)
        """
        socketCan.instance().sendMsg(canId, sendData)
        #socketCan.instance().receiveMsg()


    def profilePositionModeRelativeStartMove(self):
        canId = 0x600 + self.nodeId
        sendData = list()
        sendData.append(0x22)
        sendData.append(0x40)
        sendData.append(0x60)
        sendData.append(0x00)
        sendData.append(0x5F)
        sendData.append(0x00)
        sendData.append(0x00)
        sendData.append(0x00)
        socketCan.instance().sendMsg(canId, sendData)
        #socketCan.instance().receiveMsg()

    def clearFault(self):
        canId = 0x600 + self.nodeId
        sendData = list()
        sendData.append(0x22)
        sendData.append(0x40)
        sendData.append(0x60)
        sendData.append(0x00)
        sendData.append(0x80)
        sendData.append(0x00)
        sendData.append(0x00)
        sendData.append(0x00)
        socketCan.instance().sendMsg(canId, sendData)
        #socketCan.instance().receiveMsg()

    def getPosActualValue(self):
        canId = 0x600 + self.nodeId
        sendData = list()
        sendData.append(0x40)
        sendData.append(0x64)
        sendData.append(0x60)
        sendData.append(0x00)
        sendData.append(0x00)
        sendData.append(0x00)
        sendData.append(0x00)
        sendData.append(0x00)
        socketCan.instance().sendMsg(canId, sendData)
        msg = socketCan.instance().receiveMsg()
        #print(len(msg.data))
        data = msg.data[4:]
        #print("datalength: ", len(data))
        posArray = struct.unpack('i'*1, data)
        #pos = posArray[0]
        pos = self.isSameDirection(self.transfromQcToPosition(posArray[0]))
        #pos = (int((msg.data[4]&0x000000ff) + (msg.data[5]&0x000000ff)<<8 + (msg.data[6]&0x000000ff)<<16) + (msg.data[7]&0x000000ff)<<24)
        #print("receiveMsg: %X"%(msg.data[0]&0x000000ff),"%X"%msg.data[1], "%X"%msg.data[2], "%X"%msg.data[3], "%X"%msg.data[4], "%X"%msg.data[5],
        #        "%X"%msg.data[6], "%X"%msg.data[7])
        #print("pos: ", pos)
        return pos





"""
import time
maxonMotor = maxonMotor(1)
maxonMotor.setDirection(False)
maxonMotor.setParameter(500*4, 1, 4)
maxonMotor.enableMotor()
maxonMotor.setProfileVelocityMode()
maxonMotor.setProfileVelocityModeVelocity(2)
maxonMotor.profileVelocityModeStartMove()
time.sleep(2)
maxonMotor.profileVelocityModeHalt()
maxonMotor.disableMotor()
"""

"""
import time
maxonMotor = maxonMotor(1)
maxonMotor.setDirection(False)
maxonMotor.setParameter(500*4, 1, 4)
maxonMotor.enableMotor()
maxonMotor.setProfilePositionMode()
maxonMotor.setProfilePositionModeVelocity(4)
maxonMotor.setTargetPosition(10)
maxonMotor.profilePositionModeRelativeStartMove()
time.sleep(10)
maxonMotor.profileVelocityModeHalt()
maxonMotor.disableMotor()
"""

"""
import time
maxonMotor = maxonMotor(1)
maxonMotor.clearFault()
"""

# 设置了导程、分辨率 
"""
import time
maxonMotor = maxonMotor(1)
maxonMotor.clearFault()
while(1):
    time.sleep(0.1)
    print(maxonMotor.getPosActualValue())
"""
