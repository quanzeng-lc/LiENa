from PyQt5.QtCore import QObject, pyqtSignal
from RCPControl.Motor.AdvanceOrientalMotor import AdvanceOrientalMotor
from RCPControl.Motor.RotateOrientalMotor import RotateOrientalMotor
from RCPControl.Gripper import Gripper
from RCPControl.ForceSensor import ForceSensor
from RCPControl.InfraredReflectiveSensor import InfraredReflectiveSensor
from RCPControl.SensingParameter import SensingParameter
import time
import threading
import csv
# from Motor.AdvanceOrientalMotor import AdvanceOrientalMotor
# from Motor.RotateOrientalMotor import RotateOrientalMotor
# from Gripper import Gripper
# from ForceSensor import ForceSensor
# from InfraredReflectiveSensor import InfraredReflectiveSensor
# from SensingParameter import SensingParameter


class nmGuidewireControl(QObject):

    # controlMessageArrived = pyqtSignal()

    def __init__(self):
        super(nmGuidewireControl, self).__init__()

        self.needToRetract = False
        self.speedProgress = 30
        self.speedRetract = 2 * self.speedProgress
        self.speedRotate = 200
        self.rotateTime = 360 / self.speedRotate
        self.homeSpeed = 3
        self.number_of_cycles = 0
        self.guidewireProgressHome = False
        self.global_state = 0
        self.guidewire_status = 0

        self.guidewireProgressMotor = AdvanceOrientalMotor()
        self.guidewireRotateMotor = RotateOrientalMotor()

        self.gripperFront = Gripper(7)
        self.gripperBack = Gripper(8)

        self.infraredReflectiveSensor = InfraredReflectiveSensor()
        self.translationalForceSensor = ForceSensor("/dev/ttyusb_force", 9600, 8, 'N', 1)
        self.rotationalForceSensor = ForceSensor("/dev/ttyusb_torque", 9600, 8, 'N', 1)

        self.time_stamp_list = list()
        self.speed_list = list()
        self.guidewire_advance_distance = 0    # mm
        self.mutex = threading.Lock()

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
        self.guidewire_status = 0

    def enable(self):
        self.guidewireProgressMotor.enable()
        self.guidewireRotateMotor.enable()

    def start_move(self):
        # print("ha ha start!")
        self.guidewireProgressMotor.start_move()
        self.guidewireRotateMotor.start_move()

    def analyse(self):
        while True:
            # self.needToRetract or self.guidewireProgressHome is true : forbid
            if self.guidewire_status == 2 or self.guidewire_status == 3 or self.guidewire_status == 4:
                continue
            self.global_state = self.infraredReflectiveSensor.read_current_state()
            if self.global_state == 2:
                self.guidewire_status = 2
                retract_task = threading.Thread(None, self.prepare_for_another_tour)
                retract_task.start()
            elif self.global_state == 1:
                self.guidewire_status = 3
                home_task = threading.Thread(None, self.push_guidewire_home)
                home_task.start()
            elif self.global_state == 3 or self.global_state == 4:
                self.set_translational_speed(0)
            time.sleep(0.5)

    def push_guidewire_home(self):
        # self.context.clear_guidewire_message()
        self.enable()
        self.set_translational_speed(self.homeSpeed)
        self.start_move()
        self.global_state = self.infraredReflectiveSensor.read_current_state()
        while self.global_state == 1:
            time.sleep(0.5)
            print("home")
            self.global_state = self.infraredReflectiveSensor.read_current_state()
        self.set_translational_speed(0)
        self.guidewire_status = 0
        # self.guidewireRotateMotor.rm_move_to_position(90, -8000)
        # time.sleep(4)

    def prepare_for_another_tour(self):
        self.set_translational_speed(0)
        # fasten front gripper
        self.gripperFront.gripper_chuck_fasten()
        # self_tightening chunck
        self.gripperBack.gripper_chuck_fasten()
        time.sleep(1)

        self.set_rotational_speed(-1 * self.speedRotate)  # +/loosen
        self.start_move()
        time.sleep(self.rotateTime)
        self.set_rotational_speed(0)
        time.sleep(1)
        self.set_translational_speed(-self.speedProgress)
        self.start_move()
        # time.sleep(3)
        while self.infraredReflectiveSensor.read_current_state() != 1:
            time.sleep(0.5)
            print("retracting", self.infraredReflectiveSensor.read_current_state(), self.global_state)
        print("back limitation arrived")
        self.set_translational_speed(0)
        self.set_rotational_speed(self.speedRotate)  # -
        time.sleep(self.rotateTime + 3)
        self.set_rotational_speed(0)

        self.gripperFront.gripper_chuck_loosen()
        self.gripperBack.gripper_chuck_loosen()
        # self.context.clear_guidewire_message()
        # advance Home
        self.push_guidewire_home()
        # self.number_of_cycles -= 1
        # if self.number_of_cycles > 0:
        #     while self.needToRetract or self.guidewireProgressHome:
        #         time.sleep(0.5)
        #     self.push_guidewire_advance()

    def multi_pull_guidewire(self, times):
        self.guidewire_status = 4
        for i in range(times):
            print("times", times)
            # fasten front gripper
            self.gripperFront.gripper_chuck_loosen()
            # self_tightening chunck
            self.gripperBack.gripper_chuck_loosen()
            time.sleep(1)
            self.set_both(-self.speedProgress, 0)
            self.start_move()
            self.global_state = self.infraredReflectiveSensor.read_current_state()
            while self.global_state != 1:
                time.sleep(0.5)
                print("pull")
                self.global_state = self.infraredReflectiveSensor.read_current_state()
            # fasten front gripper
            self.set_both(0, 0)
            self.gripperFront.gripper_chuck_fasten()
            # self_tightening chunck
            self.gripperBack.gripper_chuck_fasten()
            self.set_both(0, -1 * self.speedRotate)  # +/loosen
            self.start_move()
            time.sleep(self.rotateTime)
            self.set_both(0, 0)
            self.push_guidewire_home()
            self.set_both(self.speedProgress, 0)
            self.global_state = self.infraredReflectiveSensor.read_current_state()
            while self.global_state != 2:
                time.sleep(0.5)
                print("advance")
                self.global_state = self.infraredReflectiveSensor.read_current_state()
            self.set_both(0, self.speedRotate)
            time.sleep(self.rotateTime + 3)
            self.set_rotational_speed(0)
            self.gripperBack.gripper_chuck_loosen()
            self.gripperFront.gripper_chuck_loosen()
            time.sleep(1)
        self.guidewire_status = 0

    def set_rotational_speed(self, rotational_speed):
        self.guidewireRotateMotor.set_expectedSpeed(rotational_speed)

    def set_translational_speed(self, translation_speed):
        self.time_stamp_list.append(time.time())
        self.speed_list.append(translation_speed)
        self.guidewireProgressMotor.set_expectedSpeed(translation_speed)

    def set_both(self, translation_speed, rotational_speed):
        if self.needToRetract or self.guidewireProgressHome:
            return
        self.set_translational_speed(translation_speed)
        self.set_rotational_speed(rotational_speed)
        if translation_speed > 0 or rotational_speed > 0:
            self.guidewire_status = 1
        else:
            self.guidewire_status = 0

    def get_haptic_information(self):
        rf = self.rotationalForceSensor.get_value()
        tf = self.translationalForceSensor.get_value()
        return tf, rf

    def get_status(self):
        return self.guidewire_status

    def get_translation_distance(self):
        self.mutex.acquire()
        count = len(self.time_stamp_list)
        for i in range(count-1):
            self.guidewire_advance_distance += self.speed_list[i]*(self.time_stamp_list[i+1]-self.time_stamp_list[i])
        time_now = time.time()
        speed_now = self.speed_list[count-1]
        self.guidewire_advance_distance += speed_now*(time_now - self.time_stamp_list[count-1])
        self.time_stamp_list.clear()
        self.speed_list.clear()
        self.speed_list.append(speed_now)
        self.time_stamp_list.append(time_now)
        self.mutex.release()

    def translational_go_home(self):
        self.guidewireProgressMotor.go_home()

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
        self.guidewireProgressMotor.start_move()
        print("number_of_cycles", self.number_of_cycles)

    def define_number_of_cycles(self):
        """
        define the number of cycels of the robot operation
        """
        self.number_of_cycles = int(input("please input the number of cycles:"))

    def multitime_push_guidewire(self):
        self.define_number_of_cycles()
        self.push_guidewire_advance()

"""
import sys
guidewireControl = nmGuidewireControl()
guidewireControl.multitime_push_guidewire()
"""