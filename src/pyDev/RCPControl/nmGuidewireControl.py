from PyQt5.QtCore import QObject, pyqtSignal
import sys
sys.path.append("../")
from RCPControl.maxonMotor import maxonMotor
from RCPControl.Gripper import Gripper
from RCPControl.ForceSensor import ForceSensor
from RCPControl.InfraredReflectiveSensor import InfraredReflectiveSensor
from RCPControl.SensingParameter import SensingParameter
import threading
import time
import csv


class nmGuidewireControl(QObject):

    # controlMessageArrived = pyqtSignal()

    def __init__(self):
        super(nmGuidewireControl, self).__init__()

        self.needToRetract = False
        self.forbid = 0
        self.speedProgress = 8
        #self.speedRetract = 2 * self.speedProgress
        self.speedRotate = 80
        self.rotateTime = 500 / self.speedRotate
        self.homeSpeed = 1
        self.number_of_cycles = 0
        self.number = ""
        self.guidewireProgressHome = False
        self.global_state = 0
        #self.guidewire_status = 0
        self.start_move_flag = False
        self.recorde_flag=False
        self.retract_flag=False
        self.multi_pull_times = 0

        self.guidewireProgressMotor = maxonMotor(1)
        self.guidewireProgressMotor.setParameter(500*4, 1, 4) 
        self.guidewireProgressMotor.setDirection(False)
        self.guidewireProgressMotor.setProfileVelocityMode()
        
        self.guidewireRotateMotor = maxonMotor(2)
        self.guidewireRotateMotor.setParameter(500*4, 1, 360)
        self.guidewireRotateMotor.setDirection(False)
        self.guidewireRotateMotor.setProfileVelocityMode()
        
        #self.open()
        self.enable()

        self.gripperFront = Gripper(26)
        self.gripperBack = Gripper(19)

        self.infraredReflectiveSensor = InfraredReflectiveSensor()

        self.translationalForceSensor = ForceSensor("/dev/ttyUSB0", 9600, 8, 'N', 1)
        #self.rotationalForceSensor = ForceSensor("/dev/ttyusb_torque", 9600, 8, 'N', 1)


        self.round_count = 0
        self.guidewire_round_dst = 0    #mm

        self.analyseTask = threading.Thread(None, self.analyse)
        self.analyseTask.start()

        self.force_quire_task = threading.Thread(None, self.force_aquire)
        self.force_quire_task.start()

        # self.controlMessageArrived[LienaControlInstruction].connect(self.reaction)

    def open(self):
        #self.guidewireProgressMotor.open_device()
        #self.guidewireRotateMotor.open_device()
        pass

    def close(self):
        #self.guidewireProgressMotor.close_device()
        #self.guidewireRotateMotor.close_device()
        # ....
        pass

    def standby(self):
        self.guidewireProgressMotor.disableMotor()
        self.guidewireRotateMotor.disableMotor()
        self.guidewire_status = 0

    def enable(self):
        self.guidewireProgressMotor.enableMotor()
        self.guidewireRotateMotor.enableMotor()

    def start_move(self, progressVelocity, rotateVelocity):
        # print("ha ha start!")
        self.start_move_flag = True
        self.guidewireProgressMotor.setProfileVelocityModeVelocity(progressVelocity)
        self.guidewireRotateMotor.setProfileVelocityModeVelocity(rotateVelocity)
        self.guidewireProgressMotor.profileVelocityModeStartMove()
        self.guidewireRotateMotor.profileVelocityModeStartMove()

    def stop(self):
        self.guidewireProgressMotor.profileVelocityModeHalt()
        self.guidewireRotateMotor.profileVelocityModeHalt()


    def GripperLoosen(self):
        self.gripperFront.loosen()
        self.gripperBack.loosen()


    def analyse(self):
        while True:
            # self.needToRetract or self.guidewireProgressHome is true : forbid
            self.global_state = self.infraredReflectiveSensor.read_current_state()
            print("global_state: ", self.global_state)
            """
            if self.global_state == 0:
                self.forbid = 0
            if self.global_state == 2:
                self.forbid = 1
                time.sleep(0.1)
                self.guidewire_status = 2
                #retract_task = threading.Thread(None, self.prepare_for_another_tour)
                #retract_task.start()
            elif self.global_state == 1:
                self.forbid = 2
                time.sleep(0.1)
                self.guidewire_status = 3
                #home_task = threading.Thread(target=self.push_guidewire_home, args=(True,))
                #home_task.start()
            elif self.global_state == 3 or self.global_state == 4:
                self.forbid = 3
                self.start_move(0, 0)
            """
            time.sleep(1)

    def getGlobalState(self):
        #print("global_state:", self.global_state)
        return self.global_state

    def push_guidewire_home(self, flag=False):
        # self.context.clear_guidewire_message()
        self.start_move(self.homeSpeed, 0)
        self.global_state = self.infraredReflectiveSensor.read_current_state()
        while self.global_state == 1:
            time.sleep(0.2)
            print("home")
            self.global_state = self.infraredReflectiveSensor.read_current_state()
        self.start_move(0, 0)
        if flag:
            self.forbid = 0
            self.guidewire_status = 0
            self.clear_guidewire_position()
        # self.guidewireRotateMotor.rm_move_to_position(90, -8000)
        # time.sleep(4)

    def clear_guidewire_position(self):
        pass
        #self.guidewireProgressMotor.clear_current_position()

    def get_guidewire_position(self):
        return 0
        #return self.guidewireProgressMotor.get_current_position()

    def get_guidewire_absolute_position(self):
        return 0
        """
        if self.guidewire_status == 2 or self.guidewire_status == 5:
            return self.guidewire_round_dst     # mm
        else:
            return self.guidewire_round_dst + self.get_guidewire_position()
        """    
    # pull back
    def prepare_for_another_tour(self):
        
        self.recorde_flag = False
        self.round_count += 1
        self.guidewire_round_dst += self.get_guidewire_position()
        # fasten front gripper
        self.gripperFront.gripper_chuck_fasten()
        # self_tightening chunck
        self.gripperBack.gripper_chuck_fasten()
        time.sleep(1)
        self.start_move(0, -1*self.speedRotate)
        time.sleep(self.rotateTime)
        self.start_move(0, 0)
        time.sleep(1)
        self.start_move(-self.speedProgress, 0)
        # time.sleep(3)
        while self.infraredReflectiveSensor.read_current_state() != 1:
            print("retract..")
            time.sleep(0.2)
        print("back limitation arrived")
        time.sleep(0.5)
        self.push_guidewire_home()
        #self.clear_guidewire_position()
        self.start_move(0, self.speedRotate+1)  # -
        time.sleep(self.rotateTime + 3)
        self.start_move(0, 0)
        time.sleep(0.5)
        self.gripperFront.gripper_chuck_loosen()
        self.gripperBack.gripper_chuck_loosen()
        # self.context.clear_guidewire_message()
        # advance Home
        self.forbid = 0
        self.guidewire_status = 0
        #self.number_of_cycles -= 1
        """
        if self.number_of_cycles > 0:
            while self.needToRetract or self.guidewireProgressHome:
                time.sleep(0.5)
            self.push_guidewire_advance()
        """
    # pull back multi
    def multi_pull_guidewire(self, times):
        if self.forbid > 0:
            return
        self.forbid = 3
        self.guidewire_status = 4
        self.retract_flag=True
        for i in range(times):
            self.multi_pull_times = i + 1
            # print("times", times)
            # fasten front gripper
            self.gripperFront.gripper_chuck_loosen()
            # self_tightening chunck
            self.gripperBack.gripper_chuck_loosen()
            time.sleep(1)
            self.recorde_flag = True
            self.set_translational_speed(-self.speedProgress)
            self.start_move()
            self.set_rotational_speed(0)
            self.start_move()
            self.global_state = self.infraredReflectiveSensor.read_current_state()
            while self.global_state != 1:
                time.sleep(0.2)
                print("pull")
                self.global_state = self.infraredReflectiveSensor.read_current_state()
            # fasten front gripper
            #time.sleep(0.5)
            self.recorde_flag = False
            self.push_guidewire_home()
            self.guidewire_round_dst += self.get_guidewire_position()
            self.clear_guidewire_position()
            time.sleep(0.5)
            # self_tightening chunck
            self.gripperFront.gripper_chuck_fasten()
            self.gripperBack.gripper_chuck_fasten()
            time.sleep(1)
            self.set_rotational_speed(-1 * self.speedRotate)  # +/loosen
            time.sleep(self.rotateTime)
            self.set_rotational_speed(0)
            self.set_translational_speed(self.speedProgress)
            # print("self.set_translational_speed(self.speedProgress)")
            self.global_state = self.infraredReflectiveSensor.read_current_state()
            self.guidewire_status = 5
            while self.global_state != 2:
                time.sleep(0.2)
                # print("advance")
                self.global_state = self.infraredReflectiveSensor.read_current_state()
            self.set_translational_speed(0)
            self.guidewire_round_dst -= self.get_guidewire_position()
            self.guidewire_status = 4
            time.sleep(0.5)
            self.set_rotational_speed(self.speedRotate)
            time.sleep(self.rotateTime + 0.3)
            self.set_rotational_speed(0)
            time.sleep(0.5)
            # fasten front gripper
            self.gripperFront.gripper_chuck_loosen()
            # self_tightening chunck
            self.gripperBack.gripper_chuck_loosen()
            time.sleep(1)
            self.set_translational_speed(-2 * self.homeSpeed)
            while self.global_state == 2:
                time.sleep(0.2)
                print("retract")
                self.global_state = self.infraredReflectiveSensor.read_current_state()
        self.set_translational_speed(0)
        time.sleep(0.3)
        self.forbid = 0
        self.guidewire_status = 0

    def set_normal_both(self, translation_speed, rotational_speed):
        self.start_move(translation_speed, rotational_speed)
        if translation_speed > 0 or rotational_speed > 0:
            self.guidewire_status = 1
        else:
            self.guidewire_status = 0

    def get_haptic_information(self):
        #time_stamps = time.time()
        #rf = self.rotationalForceSensor.get_value()
        tf = self.translationalForceSensor.get_value_filter()
        rf = 0
        #return (time_stamps, tf, status, rf)
        return (tf, rf)

    def get_status(self):
        return self.guidewire_status

    def is_forbidden_reaction(self):
        return self.forbid

    def translational_go_home(self):
        self.guidewireProgressMotor.go_home()

    def force_aquire(self):
        compteur = 0
        while True:
            #print("force")
            data = self.get_haptic_information()
            path = "./Data/hapticFeedback.csv"
            tmpData = list()
            tmpData.append(str(compteur))
            tmpData.append(str(time.time()))
            tmpData.append(str(data[0]))
            #tmpData.append(str(data[1]))
            with open(path, 'a+') as f:
                csv_writer = csv.writer(f)
                csv_writer.writerow(tmpData)
                # f.write(tmpData[0])
            time.sleep(0.01)
            compteur = compteur+1 
            
            
    #   test guidewire advance
    def push_guidewire_advance(self):
        # self.guidewireRotateMotor.set_expectedSpeed(0)
        # self.guidewireRotateMotor.start_move()
        self.recorde_flag = True        
        self.start_move(2, 20)
        print("number_of_cycles", self.number_of_cycles)

    def define_number_of_cycles(self):
        """
        define the number of cycels of the robot operation
        """
        self.number = input("please input the number of cycles:")
        self.number_of_cycles = int(self.number)

    def multitime_push_guidewire(self):
        self.define_number_of_cycles()
        self.push_guidewire_advance()


"""
import sys
guidewireControl = nmGuidewireControl()
guidewireControl.multitime_push_guidewire()
#guidewireControl.multi_pull_guidewire(3)
"""

"""
# test advance speed
guidewireControl = nmGuidewireControl()
guidewireControl.guidewireProgressMotor.set_expectedSpeed(6)
guidewireControl.guidewireProgressMotor.start_move()
start = time.time()
time.sleep(4)
guidewireControl.guidewireProgressMotor.set_expectedSpeed(0)
print("time: ", time.time()-start)
"""

"""
# test advance distance error
guidewireControl = nmGuidewireControl()
start = time.time()
guidewireControl.guidewireProgressMotor.set_mode(0)
guidewireControl.guidewireProgressMotor.set_position(20)
guidewireControl.guidewireProgressMotor.set_pos_mode_expectedSpeed(3)
guidewireControl.guidewireProgressMotor.start_move()
"""
"""
# test rotate angle
guidewireControl = nmGuidewireControl()
guidewireControl.guidewireRotateMotor.set_mode(0)
guidewireControl.guidewireRotateMotor.set_position(5)
guidewireControl.guidewireRotateMotor.set_pos_mode_expectedSpeed(1)
guidewireControl.guidewireRotateMotor.start_move()
"""

