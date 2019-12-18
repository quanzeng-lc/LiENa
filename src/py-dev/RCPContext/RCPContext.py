# -*- coding: utf-8 -*-
import threading
import time
from threading import Lock
import csv

from RCPContext.LienaControlInstruction import LienaControlInstruction
from RCPControl.SensingParameter import SensingParameter
from RCPControl.GlobalParameterType import GlobalParameterType


class RCPContext:

    def __init__(self, input_cache, output_cache):

        self.input_cache = input_cache
        self.output_cache = output_cache

        # ---------------------------------------------------------------------------------------------
        # define the mutex to avoid concurency
        # ---------------------------------------------------------------------------------------------
        self.inputLock = threading.Lock()
        self.outputLock = threading.Lock()

        # ---------------------------------------------------------------------------------------------
        # message sequences
        # ---------------------------------------------------------------------------------------------
        # catheter control commandes in speed mode
        self.controlInstruction = []

        # ---------------------------------------------------------------------------------------------
        # system status variable 
        # ---------------------------------------------------------------------------------------------
        self.systemStatus = True

        # ------------------------------------------------------------------------------------------------------------
        # control variables:
        #
        # guidewireControlState
        #  where 
        #      0: uncontrolled,
        #      1: valid, 
        #      2: nonvalid_prepare_for_push, 
        #      3: nonvalid_prepare_for_drawn,
        #      4: exception
        #
        # catheterControlState
        #   where
        #      0: uncontrolled,
        #      1: valid
        #      2: nonvalid_beyond_guidewire
        #      3: exception
        # contrastMediaControlState
        #   where
        #      0: uncontrolled,
        #      1: valid
        #      2: exception

        self.guidewireControlState = 0
        self.catheterControlState = 0
        self.contrastMediaControlState = 0
        self.globalContrastMediaVolumn = 0

        self.globalForceFeedback = 0.0
        self.globalTorqueFeedback = 0.0
        self.globalDistanceFromChuckToCatheter = 0.0
        self.globalTelescopicRodLength = 0.0
        self.globalDistanceFromCatheterToGuidewire = 0.0
        self.globalGuidewireAngle = 0.0
        self.globalTranslationVelocity = 0.0
        self.globalRotationVelocity = 0.0
        self.globalDecisionMade = 1

        parse_command_task = threading.Thread(None, self.parse_command)
        parse_command_task.start()

        feedback_task = threading.Thread(None, self.real_time_feedback)
        feedback_task.start()

        # ------ to be merged ----
        # decisionMaking_task = threading.Thread(None, self.decisionMaking)
        # decisionMaking_task.start()

        # self.storingDataLock = Lock()
        # storingDataTask = threading.Thread(None, self.storingData)
        # storingDataTask.start()

    def parse_command(self):
        while True:
            cpt = self.input_cache.get_sequence_count()
            for i in range(cpt):
                self.inputLock.acquire()
                msg = self.input_cache.get_latest_message_by_index(i)
                if msg is not None:
                    ci = LienaControlInstruction()

                    body = msg.get_value()

                    gwts = 0
                    if int(body[2]) == 0:
                      gwts = -1 * (int(body[3])*256 + int(body[4]))
                      ci.set_guidewire_translational_speed(gwts)
                    elif int(body[2]) == 1:
                      gwts =  1 * (int(body[3])*256 + int(body[4]))
                      ci.set_guidewire_translational_speed(gwts)

                    gwrs = 0
                    if int(body[7]) == 0:
                      gwrs = -1 * (int(body[8]) * 256 + int(body[9]))
                      ci.set_guidewire_rotational_speed(gwrs)
                    elif int(body[7]) == 1:
                      gwrs = 1 * (int(body[8]) * 256 + int(body[9]))
                      ci.set_guidewire_rotational_speed(gwrs)

                    chrs = 0
                    if int(body[13]) == 0:
                      chrs = -1 * (int(body[14]) * 256 + int(body[15]))
                      ci.set_catheter_translational_speed(chrs)
                    elif int(body[13]) == 1:
                      chrs = 1 * (int(body[14]) * 256 + int(body[15]))
                      ci.set_catheter_translational_speed(chrs)
                    print ("parse_command:", gwts, gwrs, chrs)
                    self.controlInstruction.append(ci)
                self.inputLock.release()

    def real_time_feedback(self):
        while True:
            parameter = SensingParameter()
            parameter.setTimestamps(10)
            parameter.setForceFeedback(self.globalForceFeedback)
            parameter.setTorqueFeedback(self.globalTorqueFeedback)
            parameter.setDistanceFromChuckToCatheter(10)
            parameter.setTelescopicRodLength(10)
            parameter.setDistanceFromCatheterToGuidewire(10)
            parameter.setGuidewireAngle(10)
            parameter.setTranslationVelocity(10)
            parameter.setRotationVelocity(10)
            # self.sensingParameterSequence.append(parameter)
            # print 'length',len(self.sensingParameterSequence)
            # print "forcefeedback ", parameter.getForceFeedback(), "torquefeedback ", parameter.getTorqueFeedback()
            time.sleep(0.03)

    def decisionMaking(self):
        while True:
            self.globalDecisionMade = 1
            time.sleep(0.01)
        # return ret

    def decision_made(self):
        ret = self.decision_made
        return ret

    def storingData(self):
        while True:
            data = list()
            self.storingDataLock.acquire()
            if len(self.sensingParameterSequence) >= 100:
                data = self.sensingParameterSequence[0:100]
                del self.sensingParameterSequence[0:100]
            self.storingDataLock.release()
            path = "./hapticData/hapticFeedback.csv"
            for var in data:
                tmpData = list()
                tmpData.append(str(var.getTimestamps()))
                tmpData.append(str(var.getForceFeedback()))
                tmpData.append(str(var.getTorqueFeedback()))
                tmpData.append(str(var.getDistanceFromChuckToCatheter()))
                tmpData.append(str(var.getTelescopicRodLength()))
                tmpData.append(str(var.getDistanceFromCatheterToGuidewire()))
                tmpData.append(str(var.getGuidewireAngle()))
                tmpData.append(str(var.getTranslationVelocity()))
                tmpData.append(str(var.getRotationVelocity()))
                # for x in tmpData:
                #    print x
                with open(path, 'a+') as f:
                    csv_writer = csv.writer(f)
                    csv_writer.writerow(tmpData)
                    # f.write(tmpData[0])

            time.sleep(1)

    def get_guidewire_control_state(self):
        return self.guidewireControlState

    def set_guidewire_control_state(self, guidewire_state):
        self.guidewireControlState = guidewire_state

    def get_catheter_control_state(self):
        return self.catheterControlState

    def set_catheter_control_state(self, catheter_state):
        self.catheterControlState = catheter_state

    def get_contrast_media_control_state(self):
        return self.contrastMediaControlstate

    def set_contrast_media_control_state(self, contrast_media_control_state):
        self.contrastMediaControlState = contrast_media_control_state

    def getGlobalForceFeedback(self):
        return self.globalForceFeedback

    def setGlobalForceFeedback(self, globalForceFeedback):
        self.globalForceFeedback = globalForceFeedback

    def getGlobalTorqueFeedback(self):
        return self.globalTorqueFeedback

    def setGlobalTorqueFeedback(self, globalTorqueFeedback):
        self.globalTorqueFeedback = globalTorqueFeedback

    def getGlobalDistanceFromChuckToCatheter(self):
        return self.globalDistanceFromChuckToCatheter

    def setGlobalDistanceFromChuckToCatheter(self, globalDistanceFromChuckToCatheter):
        self.globalDistanceFromChuckToCatheter = globalDistanceFromChuckToCatheter

    def getGlobalTelescopicRodLength(self):
        return self.globalTelescopicRodLength

    def setGlobalTelescopicRodLength(self, globalTelescopicRodLength):
        self.globalTelescopicRodLength = globalTelescopicRodLength

    def getGlobalDistanceFromCatheterToGuidewire(self):
        return self.globalDistanceFromCatheterToGuidewire

    def setGlobalDistanceFromCatheterToGuidewire(self, globalDistanceFromCatheterToGuidewire):
        self.globalDistanceFromCatheterToGuidewire = globalDistanceFromCatheterToGuidewire

    def getGlobalGuidewireAngle(self):
        return self.globalGuidewireAngle

    def setGlobalGuidewireAngle(self, globalGuidewireAngle):
        self.globalGuidewireAngle = globalGuidewireAngle

    def getGlobalTranslationVelocity(self):
        return globalTranslationVelocity

    def setGlobalTranslationVelocity(self, globalTranslationVelocity):
        self.globalTranslationVelocity = globalTranslationVelocity

    def getGlobalRotationVelocity(self):
        return self.globalRotationVelocity

    def setGlobalRotationVelocity(self, globalRotationVelocity):
        self.globalRotationVelocity = globalRotationVelocity

    def setGlobalRotationVelocity(self, globalRotationVelocity):
        self.globalRotationVelocity = globalRotationVelocity

    def getGlobalDecisionMade(self):
        ret = self.globalDecisionMade
        return ret

    def setGlobalParameter(self, ID, parameter):
        if ID is GlobalParameterType.FORCEFEEDBACK:
            self.setGlobalForceFeedback(parameter)
        elif ID is GlobalParameterType.TORQUEFEEDBACK:
            self.setGlobalTorqueFeedback(parameter)
        elif ID is GlobalParameterType.DISTANCEFROMCHUCKTOCATHETER:
            self.setGlobalDistanceFromChuckToCatheter(parameter)
        elif ID is GlobalParameterType.TELESCOPICRODLENGTH:
            self.setGlobalTelescopicRodLength(parameter)
        elif ID is GlobalParameterType.DISTANCEFROMCATHETERTOGUIDEWIRE:
            self.setGlobalDistanceFromCatheterToGuidewire(parameter)
        elif ID is GlobalParameterType.GUIDEWIREANGLE:
            self.setGlobalGuidewireAngle(parameter)
        elif ID is TRANSLATIONVELOCITY:
            self.setGlobalTranslationVelocity(parameter)
        elif ID is GlobalParameterType.ROTATIONVELOCITY:
            self.setGlobalRotationVelocity(parameter)
        else:
            print("ParameterType error")

    def close_system(self):
        self.systemStatus = False
        self.controlInstruction = []

    def open_system(self):
        self.systemStatus = True

    def get_system_status(self):
        return self.systemStatus

    def clear(self):
        self.controlInstruction = []

    def fetch_controlInstruction(self):
        self.inputLock.acquire()
        length = len(self.controlInstruction)
        ret = self.controlInstruction.pop(length - 1)
        self.inputLock.release()
        return ret

    def get_controlInstruction_length(self):
        self.inputLock.acquire()
        length = len(self.controlInstruction)
        self.inputLock.release()
        return length
