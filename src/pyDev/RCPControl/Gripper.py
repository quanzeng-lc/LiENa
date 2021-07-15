#!/usr/bin/env python
# encoding: utf-8

import RPi.GPIO as GPIO
import time


class Gripper(object):

    def __init__(self, io):
        GPIO.setmode(GPIO.BCM)
        GPIO.setwarnings(False)
        self.flag = True	
        self.count = 0
        self.io = io
        GPIO.setup(self.io, GPIO.OUT, initial=GPIO.LOW)

    def gripper_chuck_fasten(self):
        #print("----------fasten--------")     
        GPIO.output(self.io, True)
	
    def gripper_chuck_loosen(self):
        #print("---------loosen---------")
        GPIO.output(self.io, False)	

"""
grip = Gripper(7)

for i in range(10):
    grip.gripper_chuck_fasten()
    time.sleep(1)
    grip.gripper_chuck_loosen()
    time.sleep(1)
"""
