#!/usr/bin/env python
# encoding: utf-8
import RPi.GPIO as GPIO
import time
import threading
import multiprocessing as mp
#from RCPContext.RCPContext import RCPContext


class RotateOrientalMotor(object):
    def __init__(self):
        self.orientalMotorPushLock = threading.Lock()
        self.orientalMotorPullLock = threading.Lock()
        GPIO.setmode(GPIO.BCM)
        GPIO.setwarnings(False)
        self.pushIO = 19
        self.pullIO = 26
        GPIO.setup(self.pushIO, GPIO.OUT, initial=GPIO.HIGH)
        GPIO.setup(self.pullIO, GPIO.OUT, initial=GPIO.HIGH)

        # count the pulse to calculate the rotating speed deg/s
        self.context = None

        # parametertype id
        self.hapticFeedbackID = 0

        self.open_flag = True
        # enable
        self.mv_enable = mp.Value("i", 1)

        # mode choose/default mode:speed mode
        self.mv_mode = mp.Value("i", 1)
        # judge whether the motor is moving
        self.is_moving = mp.Value("i", 0)

        # parameter resolution: degree/pulse
        self.deg_pulse = 0.36
        # gear ratio
        self.gear_ratio = 2.0

        # velocity mode degree/s
        self.expectedSpeed = 0
        self.expectedSpeedFlag = mp.Value("i", 0)
        self.vel_start_flag = mp.Value("i", 0)
        self.vel_mode_interval = mp.Value("f", 0)

        # position mode
        self.rotate_angle = 0
        self.distance_pulse = 0
        self.pos_mode_expectedSpeed = 0
        self.pos_mode_expected_flag = 0
        self.pos_start_flag = False
        self.pos_mode_interval = 0

        self.pos_count = 0
        #   self.all_sleep_time = 0

        # actual speed degree/s
        self.actualVelocity = 0
        #self.vel_move_task = threading.Thread(None, self.continuous_move)
        self.vel_move_task = mp.Process(target=self.continuous_move, args=(self.mv_mode, self.mv_enable, self.vel_start_flag, self.expectedSpeedFlag, self.is_moving, self.vel_mode_interval))
        self.pos_move_task = threading.Thread(None, self.position_move)

    def open_device(self):
        if self.open_flag:
            print("Motor is already open!")
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
        if self.mv_mode.value:
            if speed > 0:
                self.expectedSpeedFlag.value = 1
                self.vel_mode_interval.value = self.deg_pulse / (self.gear_ratio * speed * 2.0)
            elif speed < 0:
                self.expectedSpeedFlag.value = 2
                self.vel_mode_interval.value = abs(self.deg_pulse / (self.gear_ratio * speed * 2.0))
            elif speed == 0:
                self.expectedSpeedFlag.value = 0
            self.expectedSpeed = abs(speed)
        else:
            self.expectedSpeedFlag = 0

    def continuous_move(self, mv_mode, mv_enable, vel_start_flag, expectedSpeedFlag, is_moving, vel_mode_interval):
        if mv_mode.vale:
            while True:
                if mv_enable.value:
                    if vel_start_flag.value:
                        is_moving.value = 1
                        if expectedSpeedFlag.value == 0:
                            time.sleep(0.1)
                        if expectedSpeedFlag.value == 1:
                            self.push()
                        if expectedSpeedFlag.value == 2:
                            self.pull()
                    else:
                        break
                else:
                    time.sleep(0.05)
        vel_start_flag.value = 0
        is_moving.value = 0

    def push(self, vel_mode_interval):
        interval = vel_mode_interval.value
        # print "interval:", interval
        GPIO.output(self.pushIO, False)
        time.sleep(interval)
        GPIO.output(self.pushIO, True)
        time.sleep(interval)
        # self.count += 1

    def pull(self, vel_mode_interval):
        interval = vel_mode_interval.value
        GPIO.output(self.pullIO, False)
        time.sleep(interval)
        GPIO.output(self.pullIO, True)
        time.sleep(interval)
        # self.count += 1

    # Position Mode
    def set_position(self, angle):
        if not self.mv_mode:
            self.rotate_angle = abs(angle)
            self.distance_pulse = int(self.gear_ratio * self.rotate_angle / self.deg_pulse)
        else:
            self.rotate_angle = 0
            self.distance_pulse = 0

    def set_pos_expectedSpeed(self, speed):
        if not self.mv_mode:
            if speed > 0:
                self.pos_mode_interval = self.deg_pulse / (self.gear_ratio * speed * 2.0)
                self.pos_mode_expected_flag = 1
            elif speed < 0:
                self.pos_mode_interval = abs(self.deg_pulse / (self.gear_ratio * speed * 2.0))
                self.pos_mode_expected_flag = 2
            elif speed == 0:
                self.pos_mode_expected_flag = 0
            self.pos_mode_expectedSpeed = abs(speed)
        else:
            self.pos_mode_expected_flag = 0

    def position_move(self):
        if not self.mv_mode:
            if self.pos_mode_expected_flag == 1:
                self.position_push()
            #   self.pos_flag = False
            elif self.pos_mode_expected_flag == 2:
                self.position_pull()
            elif self.rotate_angle == 0:
                self.rotate_angle = 0
                self.distance_pulse = 0
            #   self.pos_flag = False
        self.pos_start_flag = False
        self.is_moving = False

    def position_push(self):
        interval = 0
        if self.rotate_angle == 0 or self.pos_mode_expectedSpeed == 0:
            return
        else:
            pulse_number = self.distance_pulse
            interval = self.pos_mode_interval
        #   print("...", pulse_number)
        for i in range(0, pulse_number):
            if self.pos_start_flag:
                self.is_moving = True
                GPIO.output(self.pushIO, False)
                time.sleep(interval)
                GPIO.output(self.pushIO, True)
                time.sleep(interval)
            else:
                break

        #   self.orientalMotorPositionPushLock.release()

    def position_pull(self):
        interval = 0
        if self.rotate_angle == 0 or self.pos_mode_expectedSpeed == 0:
            return
        else:
            pulse_number = self.distance_pulse
            interval = self.pos_mode_interval
        #   print pulse_number
        for i in range(0, pulse_number):
            if self.pos_start_flag:
                self.is_moving = True
                GPIO.output(self.pullIO, False)
                time.sleep(interval)
                GPIO.output(self.pullIO, True)
                time.sleep(interval)
            else:
                break

    def set_mode(self, mode):
        self.mv_mode.value = mode

    def start_move(self):
        if self.is_moving.value:
            return
        if self.mv_mode.value:
            self.vel_start_flag.value = 1
            time.sleep(0.01)
            self.vel_move_task.start()
        else:
            self.pos_start_flag = True
            time.sleep(0.01)
            self.pos_move_task.start()

    def stop(self):
        if self.mv_mode.value:
            self.vel_start_flag.value = 0
            time.sleep(0.01)
            # self.vel_move_task = threading.Thread(None, self.continuous_move)
            self.vel_move_task = mp.Process(self.continuous_move, args=(self.mv_mode, self.mv_enable, self.vel_start_flag, self.expectedSpeedFlag, self.is_moving, self.vel_mode_interval))
        else:
            self.pos_start_flag = False
            time.sleep(0.01)
            self.pos_move_task = threading.Thread(None, self.position_move)

    def is_moving_flag(self):
        if self.is_moving.value:
            return True
        else:
            return False

"""
motor1 = RotateOrientalMotor()
start = time.time()
motor1.set_expectedSpeed(10)
motor1.start_move()
time.sleep(5)
motor1.stop()
motor1.set_expectedSpeed(-10)
motor1.start_move()
time.sleep(5)
motor1.stop()
print("time", time.time()-start)
"""

"""
motor1 = RotateOrientalMotor()
motor1.set_mode(0)
motor1.set_position(100)
motor1.set_pos_expectedSpeed(-10)
motor1.start_move()
"""
