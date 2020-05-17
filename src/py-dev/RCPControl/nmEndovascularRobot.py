#!/usr/bin/env python
# encoding: utf-8

import threading
import time
import sys
from enum import Enum
from PyQt5.QtCore import QObject, pyqtSignal
import serial.tools.list_ports

sys.path.append("../")

from RCPContext.RCPContext import RCPContext

from RCPControl.nmGuidewireControl import nmGuidewireControl
from RCPControl.nmCatheterControl import nmCatheterControl
from RCPControl.nmContrastMediaControl import nmContrastMediaControl
from RCPControl.EmergencySwitch import EmergencySwitch
from RCPContext.LienaControlInstruction import LienaControlInstruction
from RCPControl.GlobalParameterType import GlobalParameterType

FORCEFEEDBACK = 6


class nmEndovascularRobot(QObject):
    """
        description:this class plays an role in th command and control of the interventional robot which includes:
                         -- the control of GPIOs of the raspberryPi which connet motors, sensors and grippers
                         -- the distribution of tasks in different threads
                         -- the command and control of the actions of interventional robot in surgery   
	    author:Cheng WANG
    """

    def __init__(self, context):
        super(nmEndovascularRobot, self).__init__()

        self.context = context

        # initialisation
        self.flag = True
        self.global_state = 0

        self.number_of_cycles = 0
        self.guidewireProgressHome = False
        self.guidewire_back_flag = False

        self.emSwitch = 1
        self.lastSwitch = 0
        self.em_count = 0

        # speed parameters
        self.speedProgress = 15
        self.speedRetract = 2 * self.speedProgress
        self.speedRotate = 30
        self.speedCatheter = 2
        self.rotateTime = 360 / self.speedRotate
        self.homeSpeed = 3
        # self.pos_speed = 2
        # self.position_cgf = 1
        # self.position_cgb = 2

        # sub-module
        self.guidewireControl = nmGuidewireControl()
        self.catheterControl = nmCatheterControl()
        self.contrastMediaControl = nmContrastMediaControl()
        self.switch = EmergencySwitch()

        # real time task to parse commands in context
        self.feedbackTask = threading.Thread(None, self.feedback)
        self.feedbackTask.start()

        self.open()

        # signal/slots
        self.context.controlMessageArrived[LienaControlInstruction].connect(self.reaction)
        self.context.nonProvedControlMessageArrived.connect(self.hold)
        self.context.closeSystemMessageArrived.connect(self.close)

    # ----------------------------------------------------------------------------------------------------
    # disable all sub-module of the execution unit
    def open(self):
        self.guidewireControl.open()
        self.catheterControl.open()
        self.contrastMediaControl.open()

    # ----------------------------------------------------------------------------------------------------
    # enable all sub-module of the execution unit
    def enable(self):
        self.guidewireControl.enable()
        self.catheterControl.enable()
        self.contrastMediaControl.enable()

    # ----------------------------------------------------------------------------------------------------
    # disable all sub-module of the execution unit
    def close(self):
        self.guidewireControl.close()
        self.catheterControl.close()
        self.contrastMediaControl.close()

    # ----------------------------------------------------------------------------------------------------
    # all sub-control-module enter into standby status
    def hold(self):

        if self.needToRetract or self.guidewireProgressHome:
            return

        self.guidewireRotateMotor.standby()
        self.guidewireProgressMotor.standby()
        self.catheterMotor.standby()
        self.angioMotor.standby()

    # ----------------------------------------------------------------------------------------------------
    # to check the emergency switch status.
    def get_robot_status(self):
        return self.switch.read_current_state()

    # ----------------------------------------------------------------------------------------------------
    # execute action according to the incoming message
    def reaction(self, msg):

        if self.get_robot_status() == 1:
            self.hold()
            return

        elif self.get_robot_status() == 0:
            self.enable()

            if self.decision_making() is not 1:
                return

            self.catheterControl.set_translational_speed(msg.get_catheter_translational_speed() / 25.0)
            self.guidewireControl.set_both(msg.get_guidewire_translational_speed() / 40.0, msg.get_guidewire_rotational_speed() / 40.0)
            self.contrastMediaControl.execute(msg.get_speed(), msg.get_volume())

    # ----------------------------------------------------------------------------------------------------
    # acquire feedback information
    def feedback(self):
        while True:
            tf, rf = self.guidewireControl.get_haptic_information()
            self.context.real_time_feedback(0, 0, 0, 0, tf, rf, 0, 0, 0, 0, 0, 0)
            time.sleep(0.1)

    def set_global_state(self, state):
        self.global_state = state

    def decision_making(self):
        ret = 1
        # determine control availability
        # ret = self.context.getGlobalDecisionMade()
        return ret

    def push_contrast_agent(self):
        """
        Contrast agent push
        """
        self.angioMotor.set_pos_speed(self.pos_speed)
        self.angioMotor.set_position(self.position_cgf / 4.5)
        self.angioMotor.push_contrast_media()

    def pull_contrast_agent(self):
        self.angioMotor.set_pos_speed(self.pos_speed)
        self.angioMotor.set_position(self.position_cgb / 4.5)
        self.angioMotor.pull_contrast_media()

    def push_guidewire_advance(self):
        self.guidewireProgressMotor.set_expectedSpeed(self.speedProgress)
        self.global_state = self.infraredReflectiveSensor.read_current_state()
        while self.global_state != 2:
            time.sleep(0.5)
            self.global_state = self.infraredReflectiveSensor.read_current_state()

        self.guidewireProgressMotor.set_expectedSpeed(0)

    def multitime_push_guidewire(self):
        self.define_number_of_cycles()
        for i in range(0, self.number_of_cycles):
            self.push_guidewire_advance()
            self.prepare_for_another_tour()
            print(i)

    def draw_guidewire_back(self):
        self.guidewireRotateMotor.set_expectedSpeed(self.speedRotate)
        time.sleep(self.rotateTime)
        self.guidewireRotateMotor.set_expectedSpeed(0)
        self.gripperBack.gripper_chuck_loosen()
        time.sleep(1)
        self.guidewireProgressMotor.set_expectedSpeed(-self.speedRetract)
        self.global_state = self.infraredReflectiveSensor.read_current_state()
        while self.global_state != 1:
            time.sleep(0.5)
            self.global_state = self.infraredReflectiveSensor.read_current_state()
            # print "retracting", self.global_state
        # print "back limitation arrived"

        self.guidewireProgressMotor.set_expectedSpeed(0)
        # self.guidewireRotateMotor.rm_move_to_position(85, -4000)
        # time.sleep(4)
        # self.draw_back_guidewire_curcuit_flag == True
        # self.needToRetract = False

    def chuck_loosen(self):
        self.gripperBack.gripper_chuck_fasten()
        time.sleep(1)
        self.guidewireRotateMotor.set_expectedSpeed(-self.speedRotate)
        time.sleep(self.rotateTime)
        self.guidewirRotateMotor.set_expectedSpeed(0)

    def chuck_fasten(self):
        self.gripperBack.gripper_chuck_fasten()
        time.sleep(1)
        self.guidewireRotateMotor.set_expectedSpeed(self.speedRotate)
        time.sleep(self.rotateTime)
        self.guidewireRotateMotor.set_expectedSpeed(0)

    def draw_guidewire_advance(self):
        """
        the shiftboard advance in the process of drawing back of guidewire
	"""
        self.gripperFront.gripper_chuck_loosen()
        self.gripperBack.gripper_chuck_loosen()
        time.sleep(1)
        self.gripperFront.gripper_chuck_fasten()
        self.gripperBack.gripper_chuck_fasten()
        time.sleep(1)
        self.guidewireRotateMotor.set_expectedSpeed(-self.speedRotate)
        time.sleep(self.rotateTime)
        self.guidewireRotateMotor.set_expectedSpeed(0)
        self.guidewireProgressMotor.set_expectedSpeed(self.speedProgress)
        self.global_state = self.infraredReflectiveSensor.read_current_state()
        while self.global_state != 2:
            time.sleep(0.5)
            self.global_state = self.infraredReflectiveSensor.read_current_state()
        # print "advancing", self.global_state
        # print "front limitation arrived"

        self.guidewireProgressMotor.set_expectedSpeed(0)
        # self.gripperFront.gripper_chuck_fasten()
        # self.gripperBack.gripper_chuck_fasten()
        time.sleep(1)
        # self.guidewireRotateMotor.rm_move_to_position(80, -4000)
        # time.sleep(3)

        # self.gripperFront.gripper_chuck_fasten()
        # time.sleep(1)

    # self.gripperFront.gripper_chuck_loosen()
    # self.gripperBack.gripper_chuck_loosen()
    # time.sleep(1)

    def multitime_draw_back_guidewire(self):
        """
        the process of drawing back guidewire for several times
	"""
        self.define_number_of_cycles()
        for i in range(0, self.number_of_cycles):
            self.draw_guidewire_advance()
            self.draw_guidewire_back()
            print(i)

    def automatic_procedure(self):
        self.angioMotor.set_pos_speed(4)
        self.angioMotor.set_position(10)
        self.angioMotor.push_contrast_media()
        print("angiographing finish")
        time.sleep(5)
        self.multitime_push_guidewire()

    def push_and_pull(self):
        """
        the test of processing and drawing back guidewire for several times
	    """
        self.multitime_push_guidewire()
        self.multitime_draw_back_guidewire()

    def loosen(self):
        """
        the test of gripper
	"""
        self.gripperBack.gripper_chuck_fasten()
        time.sleep(1)
        self.gripperBack.gripper_chuck_loosen()
        # self.gripperBack.gripper_chuck_loosen()
        time.sleep(1)

    def catheter_advance(self):
        """
        the process of guidewire and cathter advance by turns
	"""
        self.gripperFront.gripper_chuck_loosen()
        self.gripperBack.gripper_chuck_loosen()
        self.draw_back_guidewire_curcuit_flag = True
        self.needToRetract = False

        self.guidewireProgressMotor.set_expectedSpeed(self.speedProgress)
        self.catheterMotor.set_expectedSpeed(self.speedCatheter)
        self.global_state = self.infraredReflectiveSensor.read_current_state()
        while self.global_state != 2:
            time.sleep(0.5)
            self.global_state = self.infraredReflectiveSensor.read_current_state()
        # print "pushing", self.global_state
        # print "front limitation arrived"

        self.guidewireProgressMotor.set_expectedSpeed(0)
        self.catheterMotor.set_expectedSpeed(0)
        # self.guidewireRotateMotor.rm_move_to_position(90, -8000)
        # time.sleep(4)

        # self.context.clear()
        self.draw_back_guidewire_curcuit_flag = False

        # self.gripperFront.gripper_chuck_loosen()
        # self.gripperBack.gripper_chuck_loosen()
        # time.sleep(1)

        # fasten front gripper
        self.gripperFront.gripper_chuck_fasten()

        # self-tightening chunck
        self.gripperBack.gripper_chuck_fasten()
        time.sleep(1)
        self.guidewireRotateMotor.set_expectedSpeed(-self.speedRotate)  # +/loosen
        time.sleep(self.rotateTime)
        self.guidewireRotateMotor.set_expectedSpeed(0)

        # self.gripperFront.gripper_chuck_loosen()
        # time.sleep(1)
        self.guidewireProgressMotor.set_expectedSpeed(-self.speedProgress)

        self.global_state = self.infraredReflectiveSensor.read_current_state()
        while self.global_state != 1:
            time.sleep(0.5)
            self.global_state = self.infraredReflectiveSensor.read_current_state()
            print("retracting", self.global_state)
        print("back limitation arrived")

        self.guidewireProgressMotor.set_expectedSpeed(0)
        self.guidewireRotateMotor.set_expectedSpeed(self.speedRotate)
        time.sleep(self.rotateTime)
        self.guidewireRotateMotor.set_expectedSpeed(0)

    def multitime_catheter_advance(self):
        """
        the process of guidewire and cathter advance by turns for several times
	"""
        self.define_number_of_cycles()
        for i in range(0, self.number_of_cycles):
            self.catheter_advance()
            # print(i)

    def test(self):
        self.gripperBack.gripper_chuck_fasten()

    def catheter_back(self):
        """
        the process of guidewire and cathter  by turns for several times
	"""
        self.define_number_of_cycles()
        for i in range(0, self.number_of_cycles):
            self.draw_guidewire_back()
            self.catheterMotor.set_expectedSpeed(self.speedCatheter)
            print(i)

    def initialization(self):
        """
        the initialization of the status of robot
	"""
        self.draw_back_guidewire_curcuit_flag = False

        # self.gripperFront.gripper_chuck_loosen()
        # self.gripperBack.gripper_chuck_loosen()
        # time.sleep(3)

        # fasten front gripper
        # self.gripperFront.gripper_chuck_fasten()

        # self-tightening chunck
        # self.gripperBack.gripper_chuck_fasten()
        # time.sleep(1)
        # self.guidewireRotateMotor.rm_move_to_position(90, 4000) # +/loosen
        # time.sleep(3)

        # self.gripperFront.gripper_chuck_loosen()
        # time.sleep(1)
        self.guidewireProgressMotor.set_expectedSpeed(-self.speedProgress)

        self.global_state = self.infraredReflectiveSensor.read_current_state()
        while self.global_state != 1:
            time.sleep(0.5)
            self.global_state = self.infraredReflectiveSensor.read_current_state()
            print("retracting", self.global_state)
        print("back limitation arrived")

        self.guidewireProgressMotor.set_expectedSpeed(0)
        self.guidewireRotateMotor.set_expectedSpeed(self.speedRotate)
        time.sleep(self.rotateTime)
        self.guidewireRotateMotor.set_expectedSpeed(0)

        self.gripperFront.gripper_chuck_loosen()
        self.gripperBack.gripper_chuck_loosen()
        self.draw_back_guidewire_curcuit_flag = True
        self.needToRetract = False

    def define_number_of_cycles(self):
        """
        define the number of cycels of the robot operation
	    """
        self.number_of_cycles = input("please input the number of cycles")

    """
    def aquirefeedback_context(self):
        while True:
            feedbackMsg = FeedbackMsg()
            forcevalue = self.forceFeedback.aquireForce()
            torquevalue = self.torqueFeedback.aquireForce()
            
            forcedirection = 0
            if forcevalue < 0:
                forcedirection = 1
            else:
                forcedirection = 0
            forcevalue = abs(forcevalue)
            
            torquedirection = 0
            if torquevalue < 0:
                torquedirection = 1
            else:
                torquedirection = 0
            torquevalue = abs(torquevalue)
            
            feedbackMsg.set_force_direction(forcedirection)
            feedbackMsg.set_force_value(forcevalue)
            feedbackMsg.set_torque_direction(torquedirection)
            feedbackMsg.set_torque_value(torquevalue)
            self.context.append_latest_forceFeedback_msg(feedbackMsg)
            #print("data", forcevalue, torquevalue)
        """

    """ 
    def guidewire_back(self):
        self.guidewire_back_flag = True
        # fasten front gripper
        self.gripperFront.gripper_chuck_loosen()
        # self-tightening chunck
        self.gripperBack.gripper_chuck_fasten()
        time.sleep(1)
        self.guidewireProgressMotor.set_expectedSpeed(-self.speedProgress)
        while self.infraredReflectiveSensor.read_current_state() != 1:
            time.sleep(0.5)
            print("retracting", self.infraredReflectiveSensor.read_current_state())
        print("back limitation arrived")

        for i in range(3):
            # two cycle
            self.gripperFront.gripper_chuck_fasten()
            self.gripperBack.gripper_chuck_loosen()
            time.sleep(1)
            self.guidewireRotateMotor.set_expectedSpeed(-1 * self.speedRotate)  # +/loosen
            time.sleep(self.rotateTime)
            self.guidewireRotateMotor.set_expectedSpeed(0)
            time.sleep(1)

            self.guidewireProgressMotor.set_expectedSpeed(self.speedProgress)
            while self.infraredReflectiveSensor.read_current_state() != 2:
                time.sleep(0.5)
                print("advance", self.infraredReflectiveSensor.read_current_state())
            self.gripperFront.gripper_chuck_loosen()
            self.gripperBack.gripper_chuck_fasten()
            self.guidewireRotateMotor.set_expectedSpeed(self.speedRotate)  # -/fasten
            time.sleep(self.rotateTime + 5)
            self.guidewireRotateMotor.set_expectedSpeed(0)
            time.sleep(1)
            self.guidewireProgressMotor.set_expectedSpeed(-self.speedProgress)
            while self.infraredReflectiveSensor.read_current_state() != 1:
                time.sleep(0.5)
                print("retracting", self.infraredReflectiveSensor.read_current_state())
            print("back limitation arrived")
            self.guidewireProgressMotor.set_expectedSpeed(0)
        self.guidewire_back_flag = False
    """

# test push guidewire automatically for several times"
"""
import sys        
dispatcher =  Dispatcher(1, 1)
dispatcher.multitime_push_guidewire()
"""
# test guidewire and cahteter advance by turns
"""
import sys
dispatcher =  Dispatcher(1, 1)
dispatcher.multitime_catheter_advance()
"""
# test injection of contrast media and push guidewire automatically for several times
"""
import sys
dispatcher =  Dispatcher(1, 1)
dispatcher.automatic_procedure()
"""
# test draw_back catheter"
"""
import sys
dispatcher =  Dispatcher(1, 1)
dispatcher.catheter()
"""
# draw back guidewire
"""
import sys
dispatcher =  Dispatcher(1, 1)
dispatcher.multitime_draw_back_guidewire()
"""
# test initialization of robot"
"""
import sys
dispatcher =  Dispatcher(1, 1)
dispatcher.initialization()
"""
# test of gripper
"""
import sys
dispatcher =  Dispatcher(1, 1)
self.gripperFront.gripper_chuck_loosen()
"""
# test of motor
"""
import sys
dispatcher =  Dispatcher(1, 1)
dispatcher.guidewireProgressMotor.set_expectedSpeed(400)
time.sleep(2)
dispatcher.guidewireProgressMotor.set_expectedSpeed(0)
"""

# test of contrast agent
"""
import sys
dispatcher = Dispatcher(1, 1)
dispatcher.push_contrast_agent()
#dispatcher.pull_contrast_agent()
"""
