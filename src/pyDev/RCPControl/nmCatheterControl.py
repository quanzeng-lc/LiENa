import threading
import time
import sys
from enum import Enum
from PyQt5.QtCore import QObject, pyqtSignal
import serial.tools.list_ports
from Motor.CatheterOrientalMotor import CatheterOrientalMotor


class nmCatheterControl(QObject):
    def __init__(self):
        super(nmCatheterControl, self).__init__()
        # micro-catheter control
        self.catheterMotor = CatheterOrientalMotor()
        self.enable()

    def open(self):
        self.catheterMotor.open_device()

    def close(self):
        self.catheterMotor.close_device()

    def enable(self):
        self.catheterMotor.enable()

    def standby(self):
        self.catheterMotor.standby()

    def set_translational_speed(self, speed):
        self.catheterMotor.set_expectedSpeed(speed)

    def set_mode(self, mode):
        self.catheterMotor.set_mode(mode)

    def start_move(self):
        self.catheterMotor.start_move()

    def stop(self):
        self.catheterMotor.stop()

    def set_translation_position(self, position):
        self.catheterMotor.set_position(position)

    def set_translation_position_speed(self, speed):
        self.catheterMotor.set_pos_mode_expectedSpeed(speed)
