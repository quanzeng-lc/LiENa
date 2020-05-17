from PyQt5.QtCore import QObject, pyqtSignal
from RCPControl.Motor.AngioOrientalMotor import AngioOrientalMotor


class nmContrastMediaControl(QObject):
    def __init__(self):
        super(nmContrastMediaControl, self).__init__()
        self.angioMotor = AngioOrientalMotor()

    def open(self):
        self.angioMotor.open_device()

    def disable(self):
        self.angioMotor.close_device()

    def enable(self):
        self.angioMotor.enable()

    def standby(self):
        self.angioMotor.standby()

    def execute(self, _volume, _speed):
        self.angioMotor.set_pos_speed(_speed / 40.0)
        self.angioMotor.set_position(_volume / 4.5)
        self.angioMotor.pull_contrast_media()