# #!/usr/bin/env python
# # encoding: utf-8
#
# import threading
# import time
# import sys
# from enum import Enum
# from PyQt5.QtCore import QObject, pyqtSignal
# import serial.tools.list_ports
# sys.path.append("../")
# from RCPContext.RCPContext import RCPContext
# from RCPControl.Motor.AdvanceOrientalMotor import AdvanceOrientalMotor
# from RCPControl.Motor.AngioOrientalMotor import AngioOrientalMotor
# from RCPControl.Motor.RotateOrientalMotor import RotateOrientalMotor
# from RCPControl.Motor.CatheterOrientalMotor import CatheterOrientalMotor
# from RCPControl.Gripper import Gripper
# from RCPControl.MaxonMotor import MaxonMotor
# from RCPControl.InfraredReflectiveSensor import InfraredReflectiveSensor
# from RCPControl.EmergencySwitch import EmergencySwitch
# from RCPContext.LienaControlInstruction import LienaControlInstruction
# from RCPControl.SensingParameter import SensingParameter
# from RCPControl.GlobalParameterType import GlobalParameterType
# from RCPControl.ForceSensor import ForceSensor
#
# # FORCEFEEDBACK = 6
#
#
# class NewDispatcher(QObject):
#     """
#         description:this class plays an role in th command and control of the interventional robot which includes:
#                          -- the control of GPIOs of the raspberryPi which connet motors, sensors and grippers
#                          -- the distribution of tasks in different threads
#                          -- the command and control of the actions of interventional robot in surgery
# 	    author:Cheng WANG
#     """
#
#     def __init__(self, context, local_mode=0):
#         super(NewDispatcher, self).__init__()
#         self.context = context
#
#         # ---------------------------------------------------------------------------------------------
#         # initialisation
#         # ---------------------------------------------------------------------------------------------
#         self.flag = True
#         self.cptt = 0
#         self.global_state = 0
#         self.needToRetract = False
#         self.draw_back_guidewire_curcuit_flag = True
#         self.number_of_cycles = 0
#         self.guidewireProgressHome = False
#         self.guidewire_back_flag = False
#         # ---------------------------------------------------------------------------------------------
#         # execution units of the interventional robot
#         # ---------------------------------------------------------------------------------------------
#         self.guidewireProgressMotor = AdvanceOrientalMotor()
#         self.guidewireProgressMotor.open_device()
#         self.guidewireProgressMotor.setParameterTypeID(GlobalParameterType.TRANSLATIONVELOCITY)
#         self.guidewireProgressMotor.open_device()
#         self.guidewireRotateMotor = RotateOrientalMotor()
#         self.guidewireRotateMotor.setParameterTypeID(GlobalParameterType.ROTATIONVELOCITY)
#         self.guidewireRotateMotor.open_device()
#         self.catheterMotor = CatheterOrientalMotor()
#         self.catheterMotor.open_device()
#         self.angioMotor = AngioOrientalMotor()
#         self.angioMotor.open_device()
#         self.gripperFront = Gripper(7)
#         self.gripperBack = Gripper(8)
#
#         # seedinterventionSystem
#         self.agencyMotor = MaxonMotor(2, "EPOS2", "MAXON SERIAL v2", "USB", "USB0", 1000000)
#         self.particleMotor = MaxonMotor(1, "EPOS2", "MAXON SERIAL V2", "USB", "USB1", 1000000)
#
#         # ---------------------------------------------------------------------------------------------
#         # sensors
#         # ---------------------------------------------------------------------------------------------
#         self.infraredReflectiveSensor = InfraredReflectiveSensor()
#
#         # --------------------------------------------------------------
#         # feedback
#         #
#         # -------------------------------------------------------------
#         # port_list = list(serial.tools.list_ports.comports())
#         # portListUSB = list()
#         # if len(port_list) == 0:
#         #     print("no serial port found")
#         # else:
#         #     for i in range(0, len(port_list)):
#         #         port = list(port_list[i])[0]
#         #         portListUSB.append(port)
#
#         self.translationalForceSensor = ForceSensor("/dev/ttyusb_force", 9600, 8, 'N', 1)
#         # self.rotationalForceSensor = ForceSensor("/dev/ttyusb_torque", 9600, 8, 'N', 1)
#
#         # ---------------------------------------------------------------------------------------------
#         # EmergencySwitch
#         # ---------------------------------------------------------------------------------------------
#         self.switch = EmergencySwitch()
#         self.emSwitch = 1
#         self.lastSwitch = 0
#         self.em_count = 0
#
#         # ---------------------------------------------------------------------------------------------
#         # speed parameters
#         # ---------------------------------------------------------------------------------------------
#         self.speedProgress = 15
#         self.speedRetract = 2 * self.speedProgress
#         self.speedRotate = 30
#         self.speedCatheter = 2
#         self.rotateTime = 360 / self.speedRotate
#         self.homeSpeed = 3
#         # self.pos_speed = 2
#         # self.position_cgf = 1
#         # self.position_cgb = 2
#
#         # -------------------------------------------------------------------------
#         # real time task to parse commandes in context
#         # ---------------------------------------------------------------------------------
#         self.context.controlMessageArrived[LienaControlInstruction].connect(self.execute)
#         self.context.nonProvedControlMessageArrived.connect(self.hold)
#         self.context.closeSystemMessageArrived.connect(self.close)
#
#         self.analyseTask = threading.Thread(None, self.analyse)
#         self.analyseTask.start()
#
#     #        self.aquirefeedbackTask = threading.Thread(None, self.aquirefeedback_context)
#     #        self.aquirefeedbackTask.start()
#
#     def close(self):
#         self.guidewireRotateMotor.close_device()
#         self.guidewireProgressMotor.close_device()
#         self.catheterMotor.close_device()
#         self.angioMotor.close_device()
#
#     def hold(self):
#
#         if self.needToRetract or self.guidewireProgressHome:
#             return
#
#         self.guidewireRotateMotor.standby()
#         self.guidewireProgressMotor.standby()
#         self.catheterMotor.standby()
#         self.angioMotor.standby()
#         self.agencyMotor.rm_move(0)
#
#     def enable(self):
#         self.guidewireRotateMotor.enable()
#         self.guidewireProgressMotor.enable()
#         self.catheterMotor.enable()
#         self.angioMotor.enable()
#
#     def get_my_status(self):
#         return 0  # self.switch.read_current_state()
#
#     def execute(self, msg):
#         # pass
#         # print("in dispatcher", msg.get_guidewire_translational_speed(), msg.get_guidewire_rotational_speed(), msg.get_catheter_translational_speed())
#
#         # emergency status switch
#
#         if self.get_my_status() == 1:
#             self.hold()
#             return
#
#         elif self.get_my_status() == 0:
#             self.enable()
#             if self.decision_making() is not 1:
#                 return
#
#             if self.needToRetract or self.guidewireProgressHome:
#                 return
#
#             # self.catheterMotor.set_expectedSpeed(msg.get_catheter_translational_speed() / 40.0)
#
#             # self.guidewireProgressMotor.set_expectedSpeed(msg.get_guidewire_translational_speed() / 40.0)
#
#             # self.guidewireRotateMotor.set_expectedSpeed(msg.get_guidewire_rotational_speed() / 40.0)
#
#             # print("progress speed", msg.get_guidewire_translational_speed())
#             velocity = self.transform_receive_value_into_velocity(msg.get_guidewire_translational_speed())
#             self.agencyMotor.rm_move(-1*velocity)
#
#             # self.angioMotor.set_pos_speed(msg.get_speed() / 40.0)
#             # self.angioMotor.set_position(msg.get_volume() / 4.5)
#             # self.angioMotor.pull_contrast_media()
#
#     def transform_receive_value_into_velocity(self, receive_value):
#         velocity = 0
#         if receive_value < 0:
#             velocity = int((receive_value / 700)*8000)
#         else:
#             velocity = int((receive_value / 500)*8000)
#         return velocity
#
#     def analyse(self):
#         while True:
#             if self.needToRetract or self.guidewireProgressHome is not True:
#                 if self.infraredReflectiveSensor.read_current_state() == 2:
#                     self.guidewireProgressMotor.set_expectedSpeed(0)
#                     self.needToRetract = True
#                     retract_task = threading.Thread(None, self.push_guidewire_back)
#                     retract_task.start()
#
#                 elif self.infraredReflectiveSensor.read_current_state() == 1:
#                     self.guidewireProgressHome = True
#                     home_task = threading.Thread(None, self.push_guidewire_home)
#                     home_task.start()
#
#                 elif self.global_state == 3:
#                     self.guidewireProgressMotor.set_expectedSpeed(0)
#             self.feedback()
#             time.sleep(0.1)
#
#     def feedback(self):
#         # rf = self.rotationalForceSensor.get_value()
#         tf = self.translationalForceSensor.get_value()
#
#         self.context.real_time_feedback(0, 0, 0, 0, tf, 0, 0, 0, 0, 0, 0, 0)
#
#     def set_global_state(self, state):
#         self.global_state = state
#
#     def decision_making(self):
#         ret = 1
#
#         # determine control availability
#         # ret = self.context.getGlobalDecisionMade()
#
#         return ret
#
#     def push_contrast_agent(self):
#         """
#         Contrast agent push
#         """
#         self.angioMotor.set_pos_speed(self.pos_speed)
#         self.angioMotor.set_position(self.position_cgf / 4.5)
#         self.angioMotor.push_contrast_media()
#
#     def pull_contrast_agent(self):
#         self.angioMotor.set_pos_speed(self.pos_speed)
#         self.angioMotor.set_position(self.position_cgb / 4.5)
#         self.angioMotor.pull_contrast_media()
#
#     def push_guidewire_back(self):
#         """
#         the shifboard get back when guidewire progress
# 	    """
#         self.draw_back_guidewire_curcuit_flag = False
#
#         # self.gripperFront.gripper_chuck_loosen()
#         # self.gripperBack.gripper_chuck_loosen()
#         # time.sleep(1)
#
#         # fasten front gripper
#         self.gripperFront.gripper_chuck_fasten()
#
#         # self-tightening chunck
#         self.gripperBack.gripper_chuck_fasten()
#         time.sleep(1)
#         self.guidewireRotateMotor.set_expectedSpeed(-1 * self.speedRotate)  # +/loosen
#         time.sleep(self.rotateTime)
#         self.guidewireRotateMotor.set_expectedSpeed(0)
#
#         # self.gripperFront.gripper_chuck_loosen()
#         # time.sleep(1)
#         self.guidewireProgressMotor.set_expectedSpeed(-self.speedProgress)
#         # time.sleep(3)
#
#         while self.infraredReflectiveSensor.read_current_state() != 1:
#             # self.global_state = self.infraredReflectiveSensor.read_current_state()
#             # if self.global_state == 4:
#             # self.global_state = self.infraredReflectiveSensor.read_current_state()
#             # continue
#             time.sleep(0.5)
#             print("retracting", self.infraredReflectiveSensor.read_current_state(), self.global_state)
#         print("back limitation arrived")
#
#         self.guidewireProgressMotor.set_expectedSpeed(0)
#         self.guidewireRotateMotor.set_expectedSpeed(self.speedRotate)  # -
#         time.sleep(self.rotateTime + 3)
#         self.guidewireRotateMotor.set_expectedSpeed(0)
#
#         self.gripperFront.gripper_chuck_loosen()
#         self.gripperBack.gripper_chuck_loosen()
#         self.draw_back_guidewire_curcuit_flag = True
#         # self.context.clear_guidewire_message()
#         self.needToRetract = False
#
#     def push_guidewire_advance(self):
#         """
#         the shiftboard advance with pushing guidewire
# 	"""
#         # self.context.clear_guidewire_message()
#         self.guidewireProgressMotor.set_expectedSpeed(self.speedProgress)
#         self.global_state = self.infraredReflectiveSensor.read_current_state()
#         while self.global_state != 2:
#             time.sleep(0.5)
#             self.global_state = self.infraredReflectiveSensor.read_current_state()
#         # print "pushing", self.global_state
#         # print "front limitation arrived"
#         self.guidewireProgressMotor.set_expectedSpeed(0)
#         # self.guidewireRotateMotor.rm_move_to_position(90, -8000)
#         # time.sleep(4)
#
#     def push_guidewire_home(self):
#         # self.context.clear_guidewire_message()
#         self.guidewireProgressMotor.enable()
#         self.guidewireProgressMotor.set_expectedSpeed(self.homeSpeed)
#         self.global_state = self.infraredReflectiveSensor.read_current_state()
#         while self.global_state == 1:
#             time.sleep(0.5)
#             print("home")
#             self.global_state = self.infraredReflectiveSensor.read_current_state()
#         # print "pushing", self.global_state
#         # print "front limitation arrived"
#
#         self.guidewireProgressMotor.set_expectedSpeed(0)
#         # self.context.clear_guidewire_message()
#         self.guidewireProgressHome = False
#         # self.guidewireRotateMotor.rm_move_to_position(90, -8000)
#         # time.sleep(4)
#
#     def multitime_push_guidewire(self):
#         """
#         the process of pushing guidewire for several times
# 	"""
#         self.define_number_of_cycles()
#         for i in range(0, self.number_of_cycles):
#             self.push_guidewire_advance()
#             self.push_guidewire_back()
#             print(i)
#
#     def draw_guidewire_back(self):
#         """
#         the shiftboard go back with drawing back guidewire
# 	    """
#         self.guidewireRotateMotor.set_expectedSpeed(self.speedRotate)
#         time.sleep(self.rotateTime)
#         self.guidewireRotateMotor.set_expectedSpeed(0)
#         self.gripperBack.gripper_chuck_loosen()
#         time.sleep(1)
#         self.guidewireProgressMotor.set_expectedSpeed(-self.speedRetract)
#         self.global_state = self.infraredReflectiveSensor.read_current_state()
#         while self.global_state != 1:
#             time.sleep(0.5)
#             self.global_state = self.infraredReflectiveSensor.read_current_state()
#             # print "retracting", self.global_state
#         # print "back limitation arrived"
#
#         self.guidewireProgressMotor.set_expectedSpeed(0)
#         # self.guidewireRotateMotor.rm_move_to_position(85, -4000)
#         # time.sleep(4)
#         # self.draw_back_guidewire_curcuit_flag == True
#         # self.needToRetract = False
#
#     def chuck_loosen(self):
#         self.gripperBack.gripper_chuck_fasten()
#         time.sleep(1)
#         self.guidewireRotateMotor.set_expectedSpeed(-self.speedRotate)
#         time.sleep(self.rotateTime)
#         self.guidewirRotateMotor.set_expectedSpeed(0)
#
#     def chuck_fasten(self):
#         self.gripperBack.gripper_chuck_fasten()
#         time.sleep(1)
#         self.guidewireRotateMotor.set_expectedSpeed(self.speedRotate)
#         time.sleep(self.rotateTime)
#         self.guidewireRotateMotor.set_expectedSpeed(0)
#
#     def draw_guidewire_advance(self):
#         """
#         the shiftboard advance in the process of drawing back of guidewire
# 	"""
#         self.gripperFront.gripper_chuck_loosen()
#         self.gripperBack.gripper_chuck_loosen()
#         time.sleep(1)
#         self.gripperFront.gripper_chuck_fasten()
#         self.gripperBack.gripper_chuck_fasten()
#         time.sleep(1)
#         self.guidewireRotateMotor.set_expectedSpeed(-self.speedRotate)
#         time.sleep(self.rotateTime)
#         self.guidewireRotateMotor.set_expectedSpeed(0)
#         self.guidewireProgressMotor.set_expectedSpeed(self.speedProgress)
#         self.global_state = self.infraredReflectiveSensor.read_current_state()
#         while self.global_state != 2:
#             time.sleep(0.5)
#             self.global_state = self.infraredReflectiveSensor.read_current_state()
#         # print "advancing", self.global_state
#         # print "front limitation arrived"
#
#         self.guidewireProgressMotor.set_expectedSpeed(0)
#         # self.gripperFront.gripper_chuck_fasten()
#         # self.gripperBack.gripper_chuck_fasten()
#         time.sleep(1)
#         # self.guidewireRotateMotor.rm_move_to_position(80, -4000)
#         # time.sleep(3)
#
#         # self.gripperFront.gripper_chuck_fasten()
#         # time.sleep(1)
#
#     # self.gripperFront.gripper_chuck_loosen()
#     # self.gripperBack.gripper_chuck_loosen()
#     # time.sleep(1)
#
#     def multitime_draw_back_guidewire(self):
#         """
#         the process of drawing back guidewire for several times
# 	"""
#         self.define_number_of_cycles()
#         for i in range(0, self.number_of_cycles):
#             self.draw_guidewire_advance()
#             self.draw_guidewire_back()
#             print(i)
#
#     def automatic_procedure(self):
#         self.angioMotor.set_pos_speed(4)
#         self.angioMotor.set_position(10)
#         self.angioMotor.push_contrast_media()
#         print("angiographing finish")
#         time.sleep(5)
#         self.multitime_push_guidewire()
#
#     def push_and_pull(self):
#         """
#         the test of processing and drawing back guidewire for several times
# 	    """
#         self.multitime_push_guidewire()
#         self.multitime_draw_back_guidewire()
#
#     def loosen(self):
#         """
#         the test of gripper
# 	"""
#         self.gripperBack.gripper_chuck_fasten()
#         time.sleep(1)
#         self.gripperBack.gripper_chuck_loosen()
#         # self.gripperBack.gripper_chuck_loosen()
#         time.sleep(1)
#
#     def catheter_advance(self):
#         """
#         the process of guidewire and cathter advance by turns
# 	"""
#         self.gripperFront.gripper_chuck_loosen()
#         self.gripperBack.gripper_chuck_loosen()
#         self.draw_back_guidewire_curcuit_flag = True
#         self.needToRetract = False
#
#         self.guidewireProgressMotor.set_expectedSpeed(self.speedProgress)
#         self.catheterMotor.set_expectedSpeed(self.speedCatheter)
#         self.global_state = self.infraredReflectiveSensor.read_current_state()
#         while self.global_state != 2:
#             time.sleep(0.5)
#             self.global_state = self.infraredReflectiveSensor.read_current_state()
#         # print "pushing", self.global_state
#         # print "front limitation arrived"
#
#         self.guidewireProgressMotor.set_expectedSpeed(0)
#         self.catheterMotor.set_expectedSpeed(0)
#         # self.guidewireRotateMotor.rm_move_to_position(90, -8000)
#         # time.sleep(4)
#
#         # self.context.clear()
#         self.draw_back_guidewire_curcuit_flag = False
#
#         # self.gripperFront.gripper_chuck_loosen()
#         # self.gripperBack.gripper_chuck_loosen()
#         # time.sleep(1)
#
#         # fasten front gripper
#         self.gripperFront.gripper_chuck_fasten()
#
#         # self-tightening chunck
#         self.gripperBack.gripper_chuck_fasten()
#         time.sleep(1)
#         self.guidewireRotateMotor.set_expectedSpeed(-self.speedRotate)  # +/loosen
#         time.sleep(self.rotateTime)
#         self.guidewireRotateMotor.set_expectedSpeed(0)
#
#         # self.gripperFront.gripper_chuck_loosen()
#         # time.sleep(1)
#         self.guidewireProgressMotor.set_expectedSpeed(-self.speedProgress)
#
#         self.global_state = self.infraredReflectiveSensor.read_current_state()
#         while self.global_state != 1:
#             time.sleep(0.5)
#             self.global_state = self.infraredReflectiveSensor.read_current_state()
#             print("retracting", self.global_state)
#         print("back limitation arrived")
#
#         self.guidewireProgressMotor.set_expectedSpeed(0)
#         self.guidewireRotateMotor.set_expectedSpeed(self.speedRotate)
#         time.sleep(self.rotateTime)
#         self.guidewireRotateMotor.set_expectedSpeed(0)
#
#     def multitime_catheter_advance(self):
#         """
#         the process of guidewire and cathter advance by turns for several times
# 	"""
#         self.define_number_of_cycles()
#         for i in range(0, self.number_of_cycles):
#             self.catheter_advance()
#             # print(i)
#
#     def test(self):
#         self.gripperBack.gripper_chuck_fasten()
#
#     def catheter_back(self):
#         """
#         the process of guidewire and cathter  by turns for several times
# 	"""
#         self.define_number_of_cycles()
#         for i in range(0, self.number_of_cycles):
#             self.draw_guidewire_back()
#             self.catheterMotor.set_expectedSpeed(self.speedCatheter)
#             print(i)
#
#     def initialization(self):
#         """
#         the initialization of the status of robot
# 	"""
#         self.draw_back_guidewire_curcuit_flag = False
#
#         # self.gripperFront.gripper_chuck_loosen()
#         # self.gripperBack.gripper_chuck_loosen()
#         # time.sleep(3)
#
#         # fasten front gripper
#         # self.gripperFront.gripper_chuck_fasten()
#
#         # self-tightening chunck
#         # self.gripperBack.gripper_chuck_fasten()
#         # time.sleep(1)
#         # self.guidewireRotateMotor.rm_move_to_position(90, 4000) # +/loosen
#         # time.sleep(3)
#
#         # self.gripperFront.gripper_chuck_loosen()
#         # time.sleep(1)
#         self.guidewireProgressMotor.set_expectedSpeed(-self.speedProgress)
#
#         self.global_state = self.infraredReflectiveSensor.read_current_state()
#         while self.global_state != 1:
#             time.sleep(0.5)
#             self.global_state = self.infraredReflectiveSensor.read_current_state()
#             print("retracting", self.global_state)
#         print("back limitation arrived")
#
#         self.guidewireProgressMotor.set_expectedSpeed(0)
#         self.guidewireRotateMotor.set_expectedSpeed(self.speedRotate)
#         time.sleep(self.rotateTime)
#         self.guidewireRotateMotor.set_expectedSpeed(0)
#
#         self.gripperFront.gripper_chuck_loosen()
#         self.gripperBack.gripper_chuck_loosen()
#         self.draw_back_guidewire_curcuit_flag = True
#         self.needToRetract = False
#
#     def catheter(self):
#         self.catheterMotor.set_expectedSpeed(self.speedCatheter)
#
#     def define_number_of_cycles(self):
#         """
#         define the number of cycels of the robot operation
# 	"""
#         self.number_of_cycles = input("please input the number of cycles")
#
#     """
#     def aquirefeedback_context(self):
#         while True:
#             feedbackMsg = FeedbackMsg()
#             forcevalue = self.forceFeedback.aquireForce()
#             torquevalue = self.torqueFeedback.aquireForce()
#
#             forcedirection = 0
#             if forcevalue < 0:
#                 forcedirection = 1
#             else:
#                 forcedirection = 0
#             forcevalue = abs(forcevalue)
#
#             torquedirection = 0
#             if torquevalue < 0:
#                 torquedirection = 1
#             else:
#                 torquedirection = 0
#             torquevalue = abs(torquevalue)
#
#             feedbackMsg.set_force_direction(forcedirection)
#             feedbackMsg.set_force_value(forcevalue)
#             feedbackMsg.set_torque_direction(torquedirection)
#             feedbackMsg.set_torque_value(torquevalue)
#             self.context.append_latest_forceFeedback_msg(feedbackMsg)
#             #print("data", forcevalue, torquevalue)
#         """
#
#     def guidewire_back(self):
#         """
#         the shifboard get guidewire back
#         """
#         self.guidewire_back_flag = True
#         # fasten front gripper
#         self.gripperFront.gripper_chuck_loosen()
#         # self-tightening chunck
#         self.gripperBack.gripper_chuck_fasten()
#         time.sleep(1)
#         self.guidewireProgressMotor.set_expectedSpeed(-self.speedProgress)
#         while self.infraredReflectiveSensor.read_current_state() != 1:
#             time.sleep(0.5)
#             print("retracting", self.infraredReflectiveSensor.read_current_state())
#         print("back limitation arrived")
#
#         for i in range(3):
#             # two cycle
#             self.gripperFront.gripper_chuck_fasten()
#             self.gripperBack.gripper_chuck_loosen()
#             time.sleep(1)
#             self.guidewireRotateMotor.set_expectedSpeed(-1 * self.speedRotate)  # +/loosen
#             time.sleep(self.rotateTime)
#             self.guidewireRotateMotor.set_expectedSpeed(0)
#             time.sleep(1)
#
#             self.guidewireProgressMotor.set_expectedSpeed(self.speedProgress)
#             while self.infraredReflectiveSensor.read_current_state() != 2:
#                 time.sleep(0.5)
#                 print("advance", self.infraredReflectiveSensor.read_current_state())
#             self.gripperFront.gripper_chuck_loosen()
#             self.gripperBack.gripper_chuck_fasten()
#             self.guidewireRotateMotor.set_expectedSpeed(self.speedRotate)  # -/fasten
#             time.sleep(self.rotateTime+5)
#             self.guidewireRotateMotor.set_expectedSpeed(0)
#             time.sleep(1)
#             self.guidewireProgressMotor.set_expectedSpeed(-self.speedProgress)
#             while self.infraredReflectiveSensor.read_current_state() != 1:
#                 time.sleep(0.5)
#                 print("retracting", self.infraredReflectiveSensor.read_current_state())
#             print("back limitation arrived")
#             self.guidewireProgressMotor.set_expectedSpeed(0)
#         self.guidewire_back_flag = False
#
#
# # test push guidewire automatically for several times"
# """
# import sys
# dispatcher =  Dispatcher(1, 1)
# dispatcher.multitime_push_guidewire()
# """
# # test guidewire and cahteter advance by turns
# """
# import sys
# dispatcher =  Dispatcher(1, 1)
# dispatcher.multitime_catheter_advance()
# """
# # test injection of contrast media and push guidewire automatically for several times
# """
# import sys
# dispatcher =  Dispatcher(1, 1)
# dispatcher.automatic_procedure()
# """
# # test draw_back catheter"
# """
# import sys
# dispatcher =  Dispatcher(1, 1)
# dispatcher.catheter()
# """
# # draw back guidewire
# """
# import sys
# dispatcher =  Dispatcher(1, 1)
# dispatcher.multitime_draw_back_guidewire()
# """
# # test initialization of robot"
# """
# import sys
# dispatcher =  Dispatcher(1, 1)
# dispatcher.initialization()
# """
# # test of gripper
# """
# import sys
# dispatcher =  Dispatcher(1, 1)
# self.gripperFront.gripper_chuck_loosen()
# """
# # test of motor
# """
# import sys
# dispatcher =  Dispatcher(1, 1)
# dispatcher.guidewireProgressMotor.set_expectedSpeed(400)
# time.sleep(2)
# dispatcher.guidewireProgressMotor.set_expectedSpeed(0)
# """
#
# # test of contrast agent
# """
# import sys
# dispatcher = Dispatcher(1, 1)
# dispatcher.push_contrast_agent()
# #dispatcher.pull_contrast_agent()
# """
