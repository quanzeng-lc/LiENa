from RCPControl.socketCan import socketCan
#from socketCan import socketCan


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
        self.gear = gear
    
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
