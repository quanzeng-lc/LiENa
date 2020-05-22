from PyQt5.QtCore import QObject, pyqtSignal

from Motor.AdvanceOrientalMotor import AdvanceOrientalMotor
from Motor.RotateOrientalMotor import RotateOrientalMotor
from Gripper import Gripper
from ForceSensor import ForceSensor
from InfraredReflectiveSensor import InfraredReflectiveSensor
from SensingParameter import SensingParameter
import time
import threading
import csv


class nmGuidewireControl(QObject):

    #controlMessageArrived = pyqtSignal()

    def __init__(self):
        super(nmGuidewireControl, self).__init__()

        self.needToRetract = False
        self.inRetractStatus = True
        self.speedProgress = 30
        self.speedRetract = 2 * self.speedProgress
        self.speedRotate = 200
        self.rotateTime = 360 / self.speedRotate
        self.homeSpeed = 3
        self.number_of_cycles = 0
        self.guidewireProgressHome = False
        self.global_state = 0

        self.guidewireProgressMotor = AdvanceOrientalMotor()
        self.guidewireRotateMotor = RotateOrientalMotor()

        self.gripperFront = Gripper(7)
        self.gripperBack = Gripper(8)

        self.infraredReflectiveSensor = InfraredReflectiveSensor()
        self.translationalForceSensor = ForceSensor("/dev/ttyusb_force", 9600, 8, 'N', 1)
        self.rotationalForceSensor = ForceSensor("/dev/ttyusb_torque", 9600, 8, 'N', 1)

        self.enable()

        self.analyseTask = threading.Thread(None, self.analyse)
        self.analyseTask.start()

        self.force_quire_task = threading.Thread(None, self.force_quire)
        self.force_quire_task.start()

        # self.controlMessageArrived[LienaControlInstruction].connect(self.reaction)

    def open(self):
        self.guidewireProgressMotor.open_device()
        self.guidewireRotateMotor.open_device()

    def close(self):
        self.guidewireProgressMotor.close_device()
        self.guidewireRotateMotor.close_device()
        # ....

    def standby(self):
        self.guidewireProgressMotor.standby()
        self.guidewireRotateMotor.standby()

    def enable(self):
        self.guidewireProgressMotor.enable()
        self.guidewireRotateMotor.enable()

    def start_move(self):
        print("ha ha start!")
        self.guidewireProgressMotor.start_move()
        self.guidewireRotateMotor.start_move()

    def analyse(self):
        while True:
            # self.needToRetract or self.guidewireProgressHome is true : forbid
            if not(self.needToRetract or self.guidewireProgressHome):
                if self.infraredReflectiveSensor.read_current_state() == 2:
                    self.needToRetract = True
                    retract_task = threading.Thread(None, self.prepare_for_another_tour)
                    retract_task.start()
                elif self.infraredReflectiveSensor.read_current_state() == 1:
                    self.guidewireProgressHome = True
                    home_task = threading.Thread(None, self.push_guidewire_home)
                    home_task.start()
                elif self.global_state == 3:
                    self.guidewireProgressMotor.set_expectedSpeed(0)
            time.sleep(0.5)

    def push_guidewire_home(self):
        # self.context.clear_guidewire_message()
        self.guidewireProgressMotor.enable()
        self.guidewireProgressMotor.set_expectedSpeed(self.homeSpeed)
        self.guidewireProgressMotor.start_move()
        self.global_state = self.infraredReflectiveSensor.read_current_state()
        while self.global_state == 1:
            time.sleep(0.5)
            print("home")
            self.global_state = self.infraredReflectiveSensor.read_current_state()

        self.guidewireProgressMotor.set_expectedSpeed(0)
        self.guidewireProgressHome = False
        # self.guidewireRotateMotor.rm_move_to_position(90, -8000)
        # time.sleep(4)

    def prepare_for_another_tour(self):

        self.guidewireProgressMotor.set_expectedSpeed(0)
        self.inRetractStatus = False
        # fasten front gripper
        self.gripperFront.gripper_chuck_fasten()
        # self-tightening chunck
        self.gripperBack.gripper_chuck_fasten()
        time.sleep(1)

        self.guidewireRotateMotor.set_expectedSpeed(-1 * self.speedRotate)  # +/loosen
        self.guidewireRotateMotor.start_move()
        time.sleep(self.rotateTime)
        self.guidewireRotateMotor.set_expectedSpeed(0)
        time.sleep(1)

        self.guidewireProgressMotor.set_expectedSpeed(-self.speedProgress)
        self.guidewireProgressMotor.start_move()
        # time.sleep(3)

        while self.infraredReflectiveSensor.read_current_state() != 1:
            time.sleep(0.5)
            print("retracting", self.infraredReflectiveSensor.read_current_state(), self.global_state)
        print("back limitation arrived")

        self.guidewireProgressMotor.set_expectedSpeed(0)
        self.guidewireRotateMotor.set_expectedSpeed(self.speedRotate)  # -
        time.sleep(self.rotateTime + 3)
        self.guidewireRotateMotor.set_expectedSpeed(0)

        self.gripperFront.gripper_chuck_loosen()
        self.gripperBack.gripper_chuck_loosen()
        self.inRetractStatus = True
        # self.context.clear_guidewire_message()
        self.needToRetract = False
        self.number_of_cycles -= 1
        print("number_of_cycles", self.number_of_cycles)
        if self.number_of_cycles > 0:
            self.push_guidewire_advance()

    def set_rotational_speed(self, rotational_speed):
        self.guidewireRotateMotor.set_expectedSpeed(rotational_speed)

    def set_translational_speed(self, translation_speed):
        self.guidewireProgressMotor.set_expectedSpeed(translation_speed)

    def set_both(self, translation_speed, rotational_speed):
        if self.needToRetract or self.guidewireProgressHome:
            return
        self.guidewireProgressMotor.set_expectedSpeed(translation_speed)
        self.guidewireRotateMotor.set_expectedSpeed(rotational_speed)

    def get_haptic_information(self):
        rf = self.rotationalForceSensor.get_value()
        tf = self.translationalForceSensor.get_value()
        return tf, rf

    def force_quire(self):
        while True:
            data = self.get_haptic_information()
            """
            path = "./hapticFeedback.csv"
            #print(data)
            tmpData = list()
            tmpData.append(str(data[0]))
            tmpData.append(str(data[1]))
            with open(path, 'a+') as f:
                csv_writer = csv.writer(f)
                csv_writer.writerow(data)
                # f.write(tmpData[0])
            time.sleep(0.01)
            """

    #   test guidewire advance
    def push_guidewire_advance(self):
        # self.guidewireRotateMotor.set_expectedSpeed(0)
        # self.guidewireRotateMotor.start_move()
        self.guidewireProgressMotor.set_expectedSpeed(self.speedProgress)
        print("speedProgress:", self.speedProgress)
        self.guidewireProgressMotor.start_move()

    def define_number_of_cycles(self):
        """
        define the number of cycels of the robot operation
        """
        self.number_of_cycles = int(input("please input the number of cycles"))

    def multitime_push_guidewire(self):
        self.define_number_of_cycles()
        self.push_guidewire_advance()


import sys
guidewireControl = nmGuidewireControl()
guidewireControl.multitime_push_guidewire()
