from PyQt5.QtCore import QObject, pyqtSignal
from RCPControl.Motor.AngioOrientalMotor import AngioOrientalMotor


class nmContrastMediaControl(QObject):
    def __init__(self):
        super(nmContrastMediaControl, self).__init__()
        self.angioMotor = AngioOrientalMotor()

    def open(self):
        self.angioMotor.open_device()

    def close(self):
        self.angioMotor.close_device()

    def enable(self):
        self.angioMotor.enable()

    def standby(self):
        self.angioMotor.standby()

    def set_translational_speed(self, speed):
        self.angioMotor.set_expectedSpeed(speed)

    def set_mode(self, mode):
        self.angioMotor.set_mode(mode)

    def start_move(self):
        self.angioMotor.start_move()

    def stop(self):
        self.angioMotor.stop()

    def set_translation_position(self, position):
        self.angioMotor.set_position(position)

    def set_translation_position_speed(self, speed):
        self.angioMotor.set_pos_mode_expectedSpeed(speed)
