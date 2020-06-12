#!/usr/bin/env python
# encoding: utf-8

import RPi.GPIO as GPIO
import time
import threading
#from RCPControl.Motor.AdvanceMotor import AdvanceMotor
#from AdvanceMotor import AdvanceMotor


# max velocity 10 mm/s
class AngioOrientalMotor(object):
    def __init__(self):

        self.orientalMotorPushLock = threading.Lock()
        self.orientalMotorPullLock = threading.Lock()
        GPIO.setmode(GPIO.BCM)
        GPIO.setwarnings(False)
        self.pushIO = 23
        self.pullIO = 24
        GPIO.setup(self.pushIO, GPIO.OUT, initial=GPIO.HIGH)
        GPIO.setup(self.pullIO, GPIO.OUT, initial=GPIO.HIGH)

        self.go_home_io = 13
        self.home_status_io = 16
        GPIO.setup(self.go_home_io, GPIO.OUT, initial=GPIO.LOW)
        GPIO.setup(self.home_status_io, GPIO.IN)

        self.context = None
        # parametertype id
        self.hapticFeedbackID = 0

        self.open_flag = True
        # enable
        self.mv_enable = True

        # mode choose / default mode:speed mode
        self.mv_mode = True
        # judge whether the motor is moving
        self.is_moving = False

        # the distance for every circle
        self.dis_circle = 2  # mm
        self.deg_pulse = 0.36  # degree for every pulse

        # home flag
        self.home_busy = False

        # Syringe diameter
        self.syringe_diameter = 41.2  # mm
        self.pi_efficient = 3.14

        # velocity mode
        self.expectedSpeed = 0  # mL/s
        self.expectedSpeedFlag = 0
        self.vel_start_flag = False
        self.count = 0
        # high/low level time interval
        self.vel_mode_interval = 0

        # position mode
        self.position = 0  # ml
        self.distance_pulse = 0  # pulse
        self.pos_mode_expectedSpeed = 0  # mL/s
        self.pos_mode_expected_flag = 0
        self.pos_start_flag = False
        self.pos_mode_interval = 0

        # actual speed mm/s
        self.actualVelocity = 0

        # count the pulse to calculate the vilocity
        self.pos_count = 0

        self.status = 0

        self.vel_move_task = threading.Thread(None, self.continuous_move)
        self.pos_move_task = threading.Thread(None, self.position_move)

        # monitoring the motor status
        # self.statusParameterTask = threading.Thread(None, self.statusMonitoring)
        # self.statusParameterTask.start()

    def open_device(self):
        if self.open_flag:
            print("Motor is already open!")
        self.open_flag = True

    def close_device(self):
        if not self.open_flag:
            print("Motor is already closed!")
        self.open_flag = False

    def set_expectedSpeed(self, speed):
        if self.mv_mode:
            advance_speed = (speed * 1000 * 4) / (self.pi_efficient * self.syringe_diameter * self.syringe_diameter)
            if advance_speed > 0:
                self.expectedSpeedFlag = 1
                self.vel_mode_interval = (self.dis_circle * self.deg_pulse) / (advance_speed * 360 * 2.0)
            elif advance_speed < 0:
                self.expectedSpeedFlag = 2
                self.vel_mode_interval = abs((self.dis_circle * self.deg_pulse) / (advance_speed * 360 * 2.0))
            elif advance_speed == 0:
                self.expectedSpeedFlag = 0
            self.expectedSpeed = abs(speed)
        else:
            self.expectedSpeedFlag = 0

    def standby(self):
        if not self.mv_enable:
            return
        self.mv_enable = False

    def enable(self):
        if self.mv_enable:
            return
        self.mv_enable = True

    def continuous_move(self):
        if self.mv_mode:
            while True:
                if self.mv_enable:
                    if self.vel_start_flag:
                        self.is_moving = True
                        if self.expectedSpeedFlag == 0:
                            self.status = 0
                            time.sleep(0.1)
                        if self.expectedSpeedFlag == 1:
                            self.push()
                        if self.expectedSpeedFlag == 2:
                            self.pull()
                    else:
                        break
                else:
                    time.sleep(0.05)
        self.is_moving = False
        self.vel_start_flag = False

    def push(self):
        self.orientalMotorPushLock.acquire()
        interval = 0
        if self.expectedSpeed == 0:
            return
        else:
            interval = self.vel_mode_interval
            # print "interval:", interval
        self.orientalMotorPushLock.release()
        # case where the interval is too large
        if interval > 1:
            return
        self.status = 1
        GPIO.output(self.pushIO, False)
        time.sleep(interval)
        GPIO.output(self.pushIO, True)
        time.sleep(interval)
        self.count += 1

    def pull(self):
        interval = 0
        if self.expectedSpeed == 0:
            return
        else:
            interval = self.vel_mode_interval
        if interval > 1:
            return
        self.status = 2
        GPIO.output(self.pullIO, False)
        time.sleep(interval)
        GPIO.output(self.pullIO, True)
        time.sleep(interval)
        self.count += 1

    # Position Mode    #############################1
    def set_position(self, position):
        if not self.mv_mode:
            self.position = abs(position)
            advance_distance = (position * 1000 * 4) / (self.pi_efficient * self.syringe_diameter * self.syringe_diameter)
            self.distance_pulse = int((advance_distance * 360) / (self.dis_circle * self.deg_pulse))
        else:
            self.position = 0
            self.distance_pulse = 0

    def set_pos_mode_expectedSpeed(self, speed):
        if not self.mv_mode:
            advance_speed = (speed * 1000 * 4) / (self.pi_efficient * self.syringe_diameter * self.syringe_diameter)
            if speed > 0:
                self.pos_mode_interval = (self.dis_circle * self.deg_pulse) / (advance_speed * 360 * 2.0)
                self.pos_mode_expected_flag = 1
            elif speed < 0:
                self.pos_mode_interval = abs((self.dis_circle * self.deg_pulse) / (advance_speed * 360 * 2.0))
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
            elif self.pos_mode_expected_flag == 2:
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
        if self.is_moving:
            return
        time.sleep(0.5)
        self.stop()
        if self.mv_mode:
            self.vel_start_flag = True
            self.vel_move_task.start()
        else:
            self.pos_start_flag = True
            self.pos_move_task.start()

    def stop(self):
        if self.mv_mode:
            self.vel_start_flag = False
            time.sleep(0.01)
            self.vel_move_task = threading.Thread(None, self.continuous_move)
        else:
            self.pos_start_flag = False
            time.sleep(0.01)
            self.pos_move_task = threading.Thread(None, self.position_move)

    def go_home_start(self):
        GPIO.output(self.go_home_io, GPIO.HIGH)
        time.sleep(0.5)
        GPIO.output(self.go_home_io, GPIO.LOW)

    def get_home_status(self):
        return GPIO.input(self.home_status_io)

    def go_home(self):
        self.home_busy = True
        self.go_home_start()
        while True:
            time.sleep(0.5)
            if self.get_home_status():
                break
        self.home_busy = False

    def is_moving_flag(self):
        if self.is_moving:
            return True
        else:
            return False


"""
motor1 = AngioOrientalMotor()
start = time.time()
motor1.set_expectedSpeed(5)
motor1.start_move()
time.sleep(2)
motor1.stop()
time.sleep(4)
motor1.set_expectedSpeed(-5)
motor1.start_move()
time.sleep(2)
motor1.stop()
print("time", time.time()-start)
"""

"""
motor1 = AngioOrientalMotor()
start = time.time()
motor1.set_mode(0)
motor1.set_position(10)
motor1.set_pos_mode_expectedSpeed(5)
motor1.start_move()
"""
