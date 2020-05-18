#!/usr/bin/env python
# encoding: utf-8

import sys
sys.path.append("../../")
sys.path.append("../")
import RPi.GPIO as GPIO
import time
import threading
from RCPControl.Motor.AdvanceMotor import AdvanceMotor
from RCPContext.RCPContext import RCPContext

# max velocity 10 mm/s
class CatheterOrientalMotor(AdvanceMotor):
    def __init__(self):

        self.orientalMotorPushLock = threading.Lock()
        self.orientalMotorPullLock = threading.Lock()
        GPIO.setmode(GPIO.BCM)
        GPIO.setwarnings(False)
        self.pushIO = 17
        self.pullIO = 27
        GPIO.setup(self.pushIO, GPIO.OUT, initial=GPIO.HIGH)
        GPIO.setup(self.pullIO, GPIO.OUT, initial=GPIO.HIGH)

        self.context = None

        # parametertype id
        self.hapticFeedbackID = 0

        # mode choose
        self.open_flag = True
        # enable
        self.mv_enable = True

        # default:velocity
        # mode choose/default mode:speed mode
        self.mv_mode = True
        # judge whether the motor is moving
        self.is_moving = False

        # the distance for every circle
        self.roller_diameter = 26.5  # mm/s
        self.pi_efficient = 3.14
        self.deg_pulse = 0.36  # degree for one pulse

        # velocity mode
        self.expectedSpeed = 0  # mm/s
        self.expectedSpeedFlag = 0
        self.vel_start_flag = False
        self.count = 0
        # high/low level time interval
        self.vel_mode_interval = 0

        # position mode
        self.position = 0
        self.distance_pulse = 0
        self.pos_mode_expectedSpeed = 0  # mm/s
        self.pos_mode_expected_flag = 0
        self.pos_start_flag = False
        self.pos_mode_interval = 0

        # actual speed mm/s
        self.actualVelocity = 0

        # count the pulse to calculate the vilocity
        self.pos_count = 0

        self.vel_move_task = threading.Thread(None, self.continuous_move)
        self.pos_move_task = threading.Thread(None, self.position_move)

        # monitoring the motor status
        # self.statusParameterTask = threading.Thread(None, self.statusMonitoring)
        # self.statusParameterTask.start()

    def open_device(self):
        if self.open_flag:
            return
        self.open_flag = True

    def close_device(self):
        self.open_flag = False

    def standby(self):
        if not self.mv_enable:
            return
        self.mv_enable = False

    def enable(self):
        if self.mv_enable:
            return
        self.mv_enable = True

    def set_expectedSpeed(self, speed):
        if speed > 0:
            self.expectedSpeedFlag = 1
            self.vel_mode_interval = (self.roller_diameter * self.pi_efficient * self.deg_pulse) / (speed * 180 * 2.0 * 2.0)
        elif speed < 0:
            self.expectedSpeedFlag = 2
            self.vel_mode_interval = abs((self.roller_diameter * self.pi_efficient * self.deg_pulse) / (speed * 180 * 2.0 * 2.0))
        elif speed == 0:
            self.expectedSpeedFlag = 0
        self.expectedSpeed = abs(speed)

    def continuous_move(self):
        if not self.mv_mode:
            return
        while True:
            if self.mv_enable:
                if self.vel_start_flag:
                    self.is_moving = True
                    if self.expectedSpeedFlag == 0:
                        time.sleep(0.1)
                        break
                    if self.expectedSpeedFlag == 1:
                        self.push()
                    if self.expectedSpeedFlag == 2:
                        self.pull()
                else:
                    break
            else:
                time.sleep(0.05)
        self.vel_start_flag = False
        self.is_moving = False

    def push(self):
        interval = 0
        if self.expectedSpeed == 0:
            return
        else:
            interval = self.vel_mode_interval
            # print "interval:", interval
        GPIO.output(self.pushIO, False)
        time.sleep(interval)
        GPIO.output(self.pushIO, True)
        time.sleep(interval)

    def pull(self):
        interval = 0
        if self.expectedSpeed == 0:
            return
        else:
            interval = self.vel_mode_interval
        GPIO.output(self.pullIO, False)
        time.sleep(interval)
        GPIO.output(self.pullIO, True)
        time.sleep(interval)

    # Position Mode    #############################1
    def set_position(self, position):
        if not self.mv_mode:
            self.position = abs(position)
            self.distance_pulse = int((self.position * 2 * 180) / (self.roller_diameter * self.pi_efficient * self.deg_pulse))
        else:
            self.position = 0
            self.distance_pulse = 0

    #	print self.position

    def set_pos_mode_expectedSpeed(self, speed):
        if not self.mv_mode:
            if speed > 0:
                self.pos_mode_interval = (self.roller_diameter * self.pi_efficient * self.deg_pulse) / (speed * 180 * 2.0 * 2.0)
                self.pos_mode_expected_flag = 1
            elif speed < 0:
                self.pos_mode_interval = abs((self.roller_diameter * self.pi_efficient * self.deg_pulse) / (speed * 180 * 2.0 * 2.0))
                self.pos_mode_expected_flag = 2
            elif speed == 0:
                self.pos_mode_interval = 9999
                self.pos_mode_expected_flag = 0
            self.pos_mode_expectedSpeed = abs(speed)
        else:
            self.pos_mode_expected_flag = 0

    def position_move(self):
        if self.mv_mode:
            return
        if self.pos_mod_expected_flag == 1:
            self.position_push()
        elif self.pos_mod_expected_flag == 2:
            self.position_pull()
        else:
            self.position = 0
            self.distance_pulse = 0
        self.pos_start_flag = False
        self.is_moving = False

    def position_push(self):
        interval = 0
        if self.position == 0 or self.pos_mode_expectedSpeed == 0:
            return
        else:
            distance = self.distance_pulse
            interval = self.pos_mode_interval
            # print(distance)
            # print(interval)
        for i in range(0, distance):
            if self.pos_start_flag:
                self.is_moving = True
                GPIO.output(self.pushIO, False)
                time.sleep(interval)
                GPIO.output(self.pushIO, True)
                time.sleep(interval)
            else:
                break

    def position_pull(self):
        interval = 0
        if self.position == 0 or self.pos_mode_expectedSpeed == 0:
            return
        else:
            distance = self.distance_pulse
            interval = self.pos_mode_interval
            # print(distance)
            # print(interval)
        for i in range(0, distance):
            if self.pos_start_flag:
                self.is_moving = True
                GPIO.output(self.pullIO, False)
                time.sleep(interval)
                GPIO.output(self.pullIO, True)
                time.sleep(interval)
            else:
                break

    def set_mode(self, mode):
        self.mv_mode = False if mode == 0 else True

    def start_move(self):
        if self.is_moving():
            return
        if self.mv_mode:
            self.vel_start_flag = True
            time.sleep(0.01)
            self.vel_move_task.start()
        else:
            self.pos_start_flag = True
            time.sleep(0.01)
            self.pos_move_task.start()

    def stop(self):
        if self.mv_mode:
            self.vel_start_flag = False
            time.sleep(0.01)
            self.vel_move_task = threading.Thread(None, self.continuous_move)
        else:
            self.pos_start_flag = False
            time.sleep(0.01)
            self.pos_move_task = threading.Thread(None, self.continuous_move_position)

    def is_moving(self):
        if self.is_moving:
            return True
        else:
            return False

"""
motor1 = CatheterOrientalMotor()
start = time.time()
motor1.set_expectedSpeed(2)
motor1.start_move()
time.sleep(5)
motor1.stop()
motor1.set_expectedSpeed(-2)
motor1.start_move()
time.sleep(5)
motor1.stop()
print("time", time.time()-start)
"""
"""
motor1 = CatheterOrientalMotor()
motor1.set_mode(0)
motor1.set_position(10)
motor1.set_pos_mode_expectedSpeed(-2)
motor1.start_move()
"""
