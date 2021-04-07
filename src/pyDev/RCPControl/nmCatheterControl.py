import threading
import time
import sys
from enum import Enum
from PyQt5.QtCore import QObject, pyqtSignal
import serial.tools.list_ports
from RCPControl.maxonMotor import maxonMotor
#from RCPControl.Motor.CatheterOrientalMotor import CatheterOrientalMotor
#from Motor.CatheterOrientalMotor import CatheterOrientalMotor


class nmCatheterControl(QObject):
    def __init__(self):
        super(nmCatheterControl, self).__init__()
        # micro-catheter control
        self.catheterMotor = maxonMotor(3)
        self.catheterMotor.setDirection(True)
        self.catheterMotor.setParameter(500*4, 1, 39)
        self.catheterMotor.setProfileVelocityMode()
        self.catheter_status = 0
        self.enable()

    def open(self):
        #self.catheterMotor.open_device()
        pass

    def close(self):
        #self.catheterMotor.close_device()
        pass

    def enable(self):
        self.catheterMotor.enableMotor()

    def standby(self):
        self.catheterMotor.disableMotor()

    def set_translational_speed(self, speed):
        self.catheterMotor.setProfileVelocityModeVelocity(speed)

    def set_mode(self, mode):
        #self.catheterMotor.set_mode(mode)
        pass

    def start_move(self):
        self.catheterMotor.profileVelocityModeStartMove()

    def stop(self):
        self.catheterMotor.profileVelocityModeHalt()

    def set_translation_position(self, position):
        #self.catheterMotor.set_position(position)
        pass

    def set_translation_position_speed(self, speed):
        #self.catheterMotor.set_pos_mode_expectedSpeed(speed)
        pass

    def get_status(self):
        #return self.catheterMotor.get_status()
        return 0

"""
catheter_control = nmCatheterControl()
catheter_control.set_translational_speed(10)
catheter_control.start_move()
time.sleep(3)
catheter_control.set_translational_speed(0)
"""
