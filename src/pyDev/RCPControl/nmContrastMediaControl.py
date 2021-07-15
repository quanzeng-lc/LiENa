from PyQt5.QtCore import QObject, pyqtSignal
import time
from RCPControl.maxonMotor import maxonMotor 
#from maxonMotor import maxonMotor 


class nmContrastMediaControl(QObject):
    def __init__(self):
        super(nmContrastMediaControl, self).__init__()
        self.angioMotor = maxonMotor(4)
        self.angioMotor.setDirection(False)
        self.angioMotor.setParameter(500*4, 103, 10)
        self.angioMotor.setProfilePositionMode()
        self.enable()
        self.angio_status = 0

    def open(self):
        #self.angioMotor.open_device()
        pass

    def close(self):
        #self.angioMotor.close_device()
        pass

    def enable(self):
        self.angioMotor.enableMotor()

    def standby(self):
        self.angioMotor.disableMotor()

    # ml/s
    def setTranslationSpeed(self, speed):
        self.angioMotor.setProfilePositionModeVelocity(speed)

    def startMove(self):
        self.angioMotor.profilePositionModeRelativeStartMove()

    def stop(self):
        self.angioMotor.profileVelocityModeHalt()

    # ml
    def setTranslationPosition(self, position):
        self.angioMotor.setTargetPosition(position)

    def get_status(self):
        #self.angio_status = self.angioMotor.get_status()
        #return self.angio_status
        return 0

    def execute(self, speed, volume):
        self.setTranslationSpeed(speed)
        self.setTranslationPosition(volume)
        self.startMove()


"""
import time
angio_control = nmContrastMediaControl()
angio_control.enable()
angio_control.execute(6, -30)
time.sleep(5)
angio_control.stop()
angio_control.standby()
"""
