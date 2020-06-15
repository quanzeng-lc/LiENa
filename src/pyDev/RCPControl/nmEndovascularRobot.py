#!/usr/bin/env python
# encoding: utf-8

import threading
import time
import sys
from enum import Enum
from PyQt5.QtCore import QObject, pyqtSignal
import serial.tools.list_ports
sys.path.append('../')
from RCPContext.RCPContext import RCPContext
from RCPContext.LienaControlInstruction import LienaControlInstruction
from RCPControl.nmGuidewireControl import nmGuidewireControl
from RCPControl.nmCatheterControl import nmCatheterControl
from RCPControl.nmContrastMediaControl import nmContrastMediaControl
from RCPControl.EmergencySwitch import EmergencySwitch

FORCEFEEDBACK = 6


class nmEndovascularRobot(QObject):
    """
        description:this class plays an role in th command and control of the interventional robot which includes:
                         -- the control of GPIOs of the raspberryPi which connet motors, sensors and grippers
                         -- the distribution of tasks in different threads
                         -- the command and control of the actions of interventional robot in surgery   
	    author:Cheng WANG
    """
    quitSystem = pyqtSignal()

    def __init__(self, context):
        super(nmEndovascularRobot, self).__init__()

        self.context = context

        # initialisation
        self.flag = True
        self.global_state = 0
        self.feedback_flag = True

        self.system_status = 0

        # ------
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

        self.guidewire_catheter_flag = False
        # signal/slots
        self.context.controlMessageArrived[LienaControlInstruction].connect(self.reaction)
        self.context.nonProvedControlMessageArrived.connect(self.standby)
        self.context.closeSystemMessageArrived.connect(self.close_app)
        self.context.endovascularMultiTimeAdvanceArrived.connect(self.guidewire_catheter_both)
        self.context.endovascularGoHomeArrived.connect(self.guidewire_go_home)

    # ----------------------------------------------------------------------------------------------------
    # disable all sub-module of the execution unit
    def open(self):
        self.guidewireControl.open()
        self.catheterControl.open()
        self.contrastMediaControl.open()

    # ----------------------------------------------------------------------------------------------------
    # disable all sub-module of the execution unit
    def close(self):
        self.guidewireControl.close()
        self.catheterControl.close()
        self.contrastMediaControl.close()
        self.feedback_task_close()
        self.quitSystem.emit()
        print("close")

    def feedback_task_close(self):
        self.feedback_flag = False

    def close_app(self):
        self.close()

    # ----------------------------------------------------------------------------------------------------
    # enable all sub-module of the execution unit
    def enable(self):
        self.guidewireControl.enable()
        self.catheterControl.enable()
        self.contrastMediaControl.enable()

    # ----------------------------------------------------------------------------------------------------
    # all sub-control-module enter into standby status
    def standby(self):
        self.guidewireControl.standby()
        self.catheterControl.standby()
        self.contrastMediaControl.standby()

    # ----------------------------------------------------------------------------------------------------
    # to check the emergency switch status.
    def get_robot_status(self):
        return self.switch.read_current_state()

    # ----------------------------------------------------------------------------------------------------
    # execute action according to the incoming message
    def reaction(self, msg):
        if self.get_robot_status() == 1:
            self.standby()
            return
        elif self.get_robot_status() == 0:
            self.enable()
            if self.decision_making() is not 1:
                return

            self.catheterControl.set_translational_speed(msg.get_catheter_translational_speed() / 5.0)
            self.catheterControl.start_move()

            if self.guidewireControl.get_status() != 2:
                # print("reaction", msg.get_guidewire_translational_speed())
                self.guidewireControl.set_both(msg.get_guidewire_translational_speed() / 10.0,
                                               msg.get_guidewire_rotational_speed() / 10.0)
                self.guidewireControl.start_move()

            # print("contrastMediaControl:", msg.get_contrast_media_speed()/100.0, msg.get_contrast_media_volume()/100.0)
            if msg.get_contrast_media_speed() > 100:
                self.contrastMediaControl.set_mode(0)
                self.contrastMediaControl.execute(msg.get_contrast_media_speed()/100.0, msg.get_contrast_media_volume()/100.0)
                self.contrastMediaControl.start_move()

    def guidewire_catheter_both(self):
        if self.guidewire_catheter_flag:
            return
        self.guidewire_catheter_flag = True
        guidewire_catheter_multi_advance = threading.Thread(target=self.guidewire_catheter_advance, args=(5,))
        guidewire_catheter_multi_advance.start()
        self.guidewire_catheter_flag = False

    def guidewire_catheter_advance(self, times):
        # print("guidewire_catheter_advance")
        self.guidewireControl.set_both(20, 0)
        self.guidewireControl.start_move()
        self.catheterControl.set_translational_speed(10)
        self.catheterControl.start_move()
        while self.guidewireControl.get_status() == 1:
            time.sleep(0.5)
        self.catheterControl.set_translational_speed(0)
        while self.guidewireControl.get_status() == 2 or self.guidewireControl.get_status() == 3:
            time.sleep(0.5)
        times -= 1
        if times == 0:
            return
        self.guidewire_catheter_advance(times)

    # ----------------------------------------------------------------------------------------------------
    # acquire feedback information
    def feedback(self):
        while True:
            if not self.feedback_flag:
                return
            tf, rf = self.guidewireControl.get_haptic_information()
            self.define_system_status()
            self.context.real_time_feedback(self.system_status, 0, 0, 0, 0, tf, rf, 0, 0, 0, 0, 0, 0)
            time.sleep(0.1)

    def set_global_state(self, state):
        self.global_state = state

    def define_system_status(self):
        system_status = 0
        if self.guidewireControl.get_status() == 0:
            system_status = system_status | 0x0000
        elif self.guidewireControl.get_status() == 1:
            system_status = system_status | 0x0040
        elif self.guidewireControl.get_status() == 2:
            system_status = system_status | 0x0080
        elif self.guidewireControl.get_status() == 3:
            system_status = system_status | 0x00C0
        if self.catheterControl.get_status() == 0:
            system_status = system_status | 0x0000
        elif self.catheterControl.get_status() == 1:
            system_status = system_status | 0x0010
        elif self.catheterControl.get_status() == 2:
            system_status = system_status | 0x0020
        if self.contrastMediaControl.get_status() == 0:
            system_status = system_status | 0x0000
        elif self.contrastMediaControl.get_status() == 1:
            system_status = system_status | 0x0004
        elif self.contrastMediaControl.get_status() == 2:
            system_status = system_status | 0x0008
        self.system_status = system_status
        # print(self.guidewireControl.get_status(), self.catheterControl.get_status(), self.contrastMediaControl.get_status(), self.system_status)

    def guidewire_go_home(self):
        self.guidewireControl.set_both(20, 0)
        self.guidewireControl.start_move()

    def decision_making(self):
        ret = 1
        # determine control availability
        # ret = self.context.getGlobalDecisionMade()
        return ret


# endovascular = nmEndovascularRobot(1)
# endovascular.guidewire_catheter_advance(3)
