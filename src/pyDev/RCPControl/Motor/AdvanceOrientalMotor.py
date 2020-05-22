#!/usr/bin/env python
# encoding: utf-8

import RPi.GPIO as GPIO
import time
import threading
import multiprocessing as mp
from Motor.AdvanceMotor import AdvanceMotor
#from pyDev.RCPContext.RCPContext import RCPContext


# max velocity 10 mm/s

class AdvanceOrientalMotor(AdvanceMotor):
    def __init__(self):
        self.orientalMotorPushLock = threading.Lock()
        self.orientalMotorPullLock = threading.Lock()
        GPIO.setmode(GPIO.BCM)
        GPIO.setwarnings(False)
        self.pushIO = 20
        self.pullIO = 21
        GPIO.setup(self.pushIO, GPIO.OUT, initial=GPIO.HIGH)
        GPIO.setup(self.pullIO, GPIO.OUT, initial=GPIO.HIGH)

        # store the global parameter
        self.context = None
        # parameter type id
        self.hapticFeedbackID = 0

        self.open_flag = True
        # enable
        
        self.mv_enable = mp.Value('i', 1)

        # default:velocity
        # mode choose/default mode: speed mode
        self.mv_mode = mp.Value('i', 1)
        # judge whether the motor is moving
        self.is_moving = mp.Value('i', 0)

        # the distance for every circle
        self.dis_circle = 5  # mm
        self.deg_pulse = 0.36  # degree for every pulse

        # velocity mode
        self.expectedSpeed = 0
        self.expectedSpeedFlag = mp.Value('i', 0)
        self.vel_start_flag = mp.Value('i', 0)
        self.count = 0
        # high/low level time interval
        self.vel_mode_interval = mp.Value('f', 0)

        # position mode
        self.position = 0
        self.distance_pulse = mp.Value('i', 0)
        self.pos_mode_expectedSpeed = 0
        self.pos_mode_expected_flag = mp.Value('i', 0)
        self.pos_start_flag = mp.Value('i', 0)
        self.pos_mode_interval = mp.Value('f', 0)

        # actual speed mm/s
        self.actualVelocity = 0

        # count the pulse to calculate the velocity
        self.pos_count = 0

        # if self.mv_mode:
        #self.vel_move_task = threading.Thread(None, self.continuous_move)
        #self.pos_move_task = threading.Thread(None, self.position_move)
        self.vel_move_task = mp.Process(target=self.continuous_move, args=(self.mv_mode, self.mv_enable, self.vel_start_flag, self.expectedSpeedFlag, self.is_moving, self.vel_mode_interval))
        self.pos_move_task = mp.Process(target=self.position_move, args=(self.mv_mode, self.pos_mode_expected_flag, self.distance_pulse, self.pos_start_flag, self.is_moving, self.pos_mode_interval))

    def open_device(self):
        if self.open_flag:
            print("Motor is already open!")
            return
        self.open_flag = True

    def close_device(self):
        if not self.open_flag:
            print("Motor is already closed!")
            return
        self.open_flag = False

    def standby(self):
        if not self.mv_enable.value:
            # print "Warning: Motor is already not enable!"
            return
        self.mv_enable = 0

    def enable(self):
        if self.mv_enable.value:
            # print "Warning: motor is already enable!"
            return
        self.mv_enable = 1

    def set_expectedSpeed(self, speed):
        if self.mv_mode.value:
            if speed > 0:
                self.expectedSpeedFlag.value = 1
                self.vel_mode_interval.value = (self.dis_circle * self.deg_pulse) / (speed * 360 * 2.0)
                print("expected_flag", self.expectedSpeedFlag)
            elif speed < 0:
                self.expectedSpeedFlag.value = 2
                self.vel_mode_interval.value = abs((self.dis_circle * self.deg_pulse) / (speed * 360 * 2.0))
            elif speed == 0:
                self.expectedSpeedFlag.value = 0
            self.expectedSpeed = abs(speed)
        else:
            self.expectedSpeedFlag.value = 0

    def continuous_move(self, mv_mode, mv_enable, vel_start_flag, expectedSpeedFlag, is_moving, vel_mode_interval):
        if mv_mode.value:
            while True:
                if mv_enable.value:
                    if vel_start_flag.value:
                        # print('ha ha')
                        is_moving.value = True
                        if expectedSpeedFlag.value == 0:
                            time.sleep(0.5)
                            print("ha ha")
                        if expectedSpeedFlag.value == 1:
                            self.push(vel_mode_interval)
                        if expectedSpeedFlag.value == 2:
                            self.pull(vel_mode_interval)
                    else:
                        break
                else:
                    time.sleep(0.5)
        vel_start_flag.value = 0
        is_moving.value = 0

    def push(self, vel_mode_interval):
        interval = vel_mode_interval.value
        #print('push', interval)
        GPIO.output(self.pushIO, False)
        time.sleep(interval)
        GPIO.output(self.pushIO, True)
        time.sleep(interval)
        # self.count += 1

    def pull(self, vel_mode_interval):
        interval = vel_mode_interval.value
        #print('pull', interval)
        GPIO.output(self.pullIO, False)
        time.sleep(interval)
        GPIO.output(self.pullIO, True)
        time.sleep(interval)
        # self.count += 1

    # Position Mode    #############################1
    def set_position(self, position):
        if not self.mv_mode.value:
            self.position = abs(position)
            self.distance_pulse.value = int((position * 360) / (self.dis_circle * self.deg_pulse))
        else:
            self.position = 0
            self.distance_pulse.value = 0

    def set_pos_mode_expectedSpeed(self, speed):
        if not self.mv_mode.value:
            if speed > 0:
                self.pos_mode_interval.value = (self.dis_circle * self.deg_pulse) / (speed * 360 * 2.0)
                self.pos_mode_expected_flag.value = 1
            elif speed < 0:
                self.pos_mode_interval.value = abs((self.dis_circle * self.deg_pulse) / (speed * 360 * 2.0))
                self.pos_mode_expected_flag.value = 2
            elif speed == 0:
                self.pos_mode_expected_flag.value = 0
            self.pos_mode_expectedSpeed = abs(speed)
        else:
            self.pos_mode_expected_flag.value = 0
        #   print self.pos_mode_expectedSpeed

    def position_move(self, mv_mode, pos_mode_expected_flag, distance_pulse, pos_start_flag, is_moving, pos_mode_interval):
        if not mv_mode.value:
            if pos_mode_expected_flag.value == 1:
                self.position_push(pos_mode_expected_flag, distance_pulse, pos_start_flag, is_moving, pos_mode_interval)
            elif pos_mode_expected_flag.value == 2:
                self.position_pull(pos_mode_expected_flag, distance_pulse, pos_start_flag, is_moving, pos_mode_interval)
            else:
                distance_pulse.value = 0
        pos_start_flag.value = False
        is_moving.value = False

    def position_push(self, pos_mode_expected_flag, distance_pulse, pos_start_flag, is_moving, pos_mode_interval):
        interval = 0
        if pos_mode_expected_flag.value == 0:
            return
        else:
            distance = distance_pulse.value
            interval = pos_mode_interval.value
            # print(distance)
            # print(interval)
        for i in range(0, distance):
            if pos_start_flag.value:
                is_moving.value = 1
                GPIO.output(self.pushIO, False)
                time.sleep(interval)
                GPIO.output(self.pushIO, True)
                time.sleep(interval)
            else:
                break

    def position_pull(self, pos_mode_expected_flag, distance_pulse, pos_start_flag, is_moving, pos_mode_interval):
        interval = 0
        if pos_mode_expected_flag.value == 0:
            return
        else:
            distance = distance_pulse.value
            interval = pos_mode_interval.value
        for i in range(0, distance):
            if pos_start_flag.value:
                is_moving.value = 1
                GPIO.output(self.pullIO, False)
                time.sleep(interval)
                GPIO.output(self.pullIO, True)
                time.sleep(interval)
            else:
                break

    def set_mode(self, mode):
        #self.mv_mode = False if mode == 0 else True
        self.mv_mode.value = mode

    def start_move(self):
        if self.is_moving.value:
            return
        if self.mv_mode.value:
            self.vel_start_flag.value = 1
            time.sleep(0.01)
            self.vel_move_task.start()
            print("vel start!")
        else:
            self.pos_start_flag.value = 1
            time.sleep(0.01)
            self.pos_move_task.start()

    def stop(self):
        if self.mv_mode.value:
            self.vel_start_flag.value = 0
            self.is_moving.value = 0
            time.sleep(0.01)
            # self.vel_move_task = threading.Thread(None, self.continuous_move)
            self.vel_move_task = mp.Process(target=self.continuous_move, args=(
            self.mv_mode, self.mv_enable, self.vel_start_flag, self.expectedSpeedFlag, self.is_moving,
            self.vel_mode_interval))

        else:
            self.pos_start_flag.value = 0
            self.is_moving.value = 0
            time.sleep(0.01)
            # self.pos_move_task = threading.Thread(None, self.position_move)
            self.pos_move_task = mp.Process(target=self.position_move, args=(
            self.mv_mode, self.pos_mode_expected_flag, self.distance_pulse, self.pos_start_flag, self.is_moving,
            self.pos_mode_interval))

    def is_moving_flag(self):
        if self.is_moving.value:
            return True
        else:
            return False


"""
motor1 = AdvanceOrientalMotor()
motor1.set_expectedSpeed(-2)
start = time.time()
motor1.start_move()
time.sleep(5)
motor1.stop()
time.sleep(3)
motor1.set_expectedSpeed(2)
motor1.start_move()
time.sleep(5)
motor1.stop()
print('time',time.time()-start)
"""

"""
motor1 = AdvanceOrientalMotor()
start = time.time()
motor1.set_mode(0)
motor1.set_position(20)
motor1.set_pos_mode_expectedSpeed(3)
motor1.start_move()
"""
