from PyQt5.QtCore import QObject, pyqtSignal
import time
from RCPControl.maxonMotor import maxonMotor 
#from RCPControl.Motor.AngioOrientalMotor import AngioOrientalMotor
# from Motor.AngioOrientalMotor import AngioOrientalMotor


class nmContrastMediaControl(QObject):
    def __init__(self):
        super(nmContrastMediaControl, self).__init__()
        self.angioMotor = maxonMotor(4)
        self.angioMotor.setDirection(True)
        self.angioMotor.setParameter(500*4, 103, 2)
        self.angioMotor.setProfileVelocityMode()
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

    def set_translational_speed(self, speed):
        self.angioMotor.setProfileVelocityModeVelocity(speed)

    def set_mode(self, mode):
        #self.angioMotor.set_mode(mode)
        pass

    def start_move(self):
        self.angioMotor.profileVelocityModeStartMove()

    def stop(self):
        self.angioMotor.profileVelocityModeHalt()

    def set_translation_position(self, position):
        #self.angioMotor.set_position(position)
        pass

    def set_translation_position_speed(self, speed):
        #self.angioMotor.set_pos_mode_expectedSpeed(speed)
        pass

    def get_status(self):
        #self.angio_status = self.angioMotor.get_status()
        #return self.angio_status
        return 0

    def execute(self, speed, volume):
        #self.set_translation_position_speed(speed)
        #self.set_translation_position(volume)
        pass

"""
angio_control = nmContrastMediaControl()
angio_control.set_translational_speed(3)
angio_control.start_move()
time.sleep(3)
angio_control.set_translational_speed(-3)
angio_control.start_move()
time.sleep(3)
angio_control.set_translational_speed(0)
"""
