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

        self.standBy = False
        # initialisation
        self.flag = True
        self.global_state = 0
        self.feedback_flag = True

        self.system_status = 0
        self.guidewire_dst = 0

        # ------
        self.number_of_cycles = 0
        self.guidewireProgressHome = False
        self.guidewire_back_flag = False

        self.emSwitch = 1
        self.lastSwitch = 0
        self.em_count = 0
        self.startPrepare = False

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
        self.initPos = 0

        # sub-module
        self.guidewireControl = nmGuidewireControl()
        self.catheterControl = nmCatheterControl()
        self.contrastMediaControl = nmContrastMediaControl()
        #self.switch = EmergencySwitch()

        # real time task to parse commands in context
        self.feedbackTask = threading.Thread(None, self.feedback)
        self.feedbackTask.start()

        self.open()

        self.guidewire_catheter_flag = False

        # signal/slots
        self.context.controlMessageArrived[LienaControlInstruction].connect(self.reaction)
        self.context.nonProvedControlMessageArrived.connect(self.standby)
        self.context.closeSystemMessageArrived.connect(self.close_app)
        # multiadvance
        self.context.endovascularPrepareAnotherTour.connect(self.prepareAnotherTour)
        self.context.endovascularGoHomeArrived.connect(self.guidewire_go_home)
        self.context.endovascularMultiTimeGuidewirePullArrived.connect(self.multi_pull_guidewire_reaction)
        self.context.posFollowMotion.connect(self.followMotion)
        self.context.saveInitPos.connect(self.saveRobotInitPos)

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
        self.guidewireControl.stop()
        self.catheterControl.stop()
        self.contrastMediaControl.stop()
        self.standBy = True
        print("-----------standBy")


    # ----------------------------------------------------------------------------------------------------
    # to check the emergency switch status.
    def get_robot_status(self):
        #return self.switch.read_current_state()
        return 0

    def saveRobotInitPos(self):
        # to do
        self.initPos = self.guidewireControl.getGuidewirePosActualValue()
        print("initPos!!!", self.initPos)

    # quanzeng ???????????????????????????????????????
    def followMotion(self, msg):
        Kp = 1
        Kd = 0.5
        K = 10
        KdKInverse = 1/(Kd*K)
        KInverse = 1/K
        XmPos = msg.get_guidewire_translational_speed()/100.0
        XmSpeed = msg.get_guidewire_rotational_speed()/100.0
        XsPos =  self.guidewireControl.getGuidewirePosActualValue()
        XsPosInit = self.initPos
        XsSpeed = KInverse*XmSpeed + KdKInverse*(Kp*XmPos - K*(XsPos - XsPosInit))
        self.guidewireControl.translateStartMove(XsSpeed)
        print("follow!!!", XsPosInit, XsPos, XmPos, XmSpeed, XsSpeed)



    # ----------------------------------------------------------------------------------------------------
    # execute action according to the incoming message
    def reaction(self, msg):
        if self.startPrepare:
            return
        #print("robot_status ", self.get_robot_status())
        if self.get_robot_status() == 1:
            self.standby()
            return
        elif self.get_robot_status() == 0:
            self.standBy = False
            if self.decision_making() is not 1:
                return
            if self.guidewire_catheter_flag:
                return
            #print('reaction', msg.get_catheter_translational_speed() / 100.0, msg.get_guidewire_translational_speed() / 100, msg.get_guidewire_rotational_speed() / 100.0)
            
            self.catheterControl.set_translational_speed(msg.get_catheter_translational_speed() / 100.0)
            self.catheterControl.start_move()

            if self.guidewireControl.getGlobalState() == 3:
                self.guidewireControl.start_move(0, 0)
            elif self.guidewireControl.getGlobalState() == 0:
                self.guidewireControl.start_move(msg.get_guidewire_translational_speed() / 100.0, msg.get_guidewire_rotational_speed() / 100.0)
            elif self.guidewireControl.getGlobalState() == 2:
                if msg.get_guidewire_translational_speed() < 0:
                    self.guidewireControl.start_move(msg.get_guidewire_translational_speed() / 100.0, msg.get_guidewire_rotational_speed() / 100.0)
                if msg.get_guidewire_translational_speed() > 0:
                    self.guidewireControl.start_move(0, msg.get_guidewire_rotational_speed() / 100.0)
            elif self.guidewireControl.getGlobalState() == 1:
                if msg.get_guidewire_translational_speed() > 0:
                    self.guidewireControl.start_move(msg.get_guidewire_translational_speed() / 100.0, msg.get_guidewire_rotational_speed() / 100.0)
                if msg.get_guidewire_translational_speed() < 0:
                    self.guidewireControl.start_move(0, msg.get_guidewire_rotational_speed() / 100.0)

            # print("contrastMediaControl:", msg.get_contrast_media_speed()/100.0, msg.get_contrast_media_volume()/100.0)
            if msg.get_contrast_media_speed() > 100:
                #print(".........contract...... ", msg.get_contrast_media_speed()/100.0, msg.get_contrast_media_volume()/100.0)
                self.contrastMediaControl.execute(msg.get_contrast_media_speed()/100.0, msg.get_contrast_media_volume()/100.0)

    # guidewire catheter move together
    def prepareAnotherTour(self):
        """
        if self.guidewire_catheter_flag:
            return
        self.guidewire_catheter_flag = True
        time.sleep(0.1)
        if self.guidewireControl.is_forbidden_reaction():
            self.guidewire_catheter_flag = False
            return
        guidewire_catheter_multi_advance = threading.Thread(target=self.guidewire_catheter_advance, args=(5,))
        guidewire_catheter_multi_advance.start()
        """
        if self.standBy:
            print("prepare!!!")
            self.startPrepare = True
            guidewirePrepareAnotherTour = threading.Thread(target=self.prepareAnotherTourFunc, args=())
            guidewirePrepareAnotherTour.start()
            #pass
            
    def prepareAnotherTourFunc(self):
        self.guidewireControl.prepare_for_another_tour()
        time.sleep(0.5)
        self.startPrepare = False
        #self.guidewireControl.GripperLoosen()


    # push guidewire multi-time
    def guidewire_catheter_advance(self, times):
        # print("guidewire_catheter_advance")
        #self.guidewireControl.set_normal_both(20, 0)
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
            self.guidewire_catheter_flag = False
            return
        self.guidewire_catheter_advance(times)

    def multi_pull_guidewire_reaction(self):
        # print("multi_pull_guidewire")
        if self.guidewire_catheter_flag:
            return
        self.guidewire_catheter_flag = True
        time.sleep(0.1)
        multi_guidewire_pull_task = threading.Thread(target=self.robot_multi_pull_guidewire, args=(7,))
        multi_guidewire_pull_task.start()

    def robot_multi_pull_guidewire(self, times):
        self.guidewireControl.multi_pull_guidewire(times)
        self.guidewire_catheter_flag = False

    # ----------------------------------------------------------------------------------------------------
    # acquire feedback information
    def feedback(self):
        while True:
            if not self.feedback_flag:
                return
            tf, rf = self.guidewireControl.get_haptic_information()
            #tf = 1
            #rf = 2
            #self.define_system_status()
            #globalState = self.guidewireControl.getGlobalState()
            self.get_guidewire_dst()
            self.context.real_time_feedback(0, 0, 0, self.guidewire_dst, 0, tf, rf, 0, 0, 0, 0, 0, 0)
            time.sleep(0.2)

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

    def get_guidewire_dst(self):
        # print("get_guidewire_dst: ", self.guidewire_dst)
        self.guidewire_dst = int(self.guidewireControl.get_guidewire_absolute_position())

    def guidewire_go_home(self):
        #self.guidewireControl.set_normal_both(-20, 0)
        #self.guidewireControl.start_move()
        print("go home!!!")

    def decision_making(self):
        ret = 1
        # determine control availability
        # ret = self.context.getGlobalDecisionMade()
        return ret


# endovascular = nmEndovascularRobot(1)
# endovascular.guidewire_catheter_advance(3)
