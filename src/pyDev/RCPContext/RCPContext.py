# -*- coding: utf-8 -*-

from PyQt5.QtCore import QObject, pyqtSignal
import threading
import time
from threading import Lock
import csv
from LiENa.LiENaBasic.lienaDefinition import *
from LiENa.LiENaStructure.LiENaMessage.LienaCustomizedMessage import LienaCustomizedMessage
from RCPContext.LienaControlInstruction import LienaControlInstruction
from RCPControl.SensingParameter import SensingParameter
from RCPControl.GlobalParameterType import GlobalParameterType


def positive_or_negtive(value):
    if value >= 0:
        return 0
    else:
        return 1


class RCPContext(QObject):
    controlMessageArrived = pyqtSignal(LienaControlInstruction)
    nonProvedControlMessageArrived = pyqtSignal()
    closeSystemMessageArrived = pyqtSignal()
    endovascularPrepareAnotherTour= pyqtSignal()
    endovascularGoHomeArrived = pyqtSignal()
    endovascularMultiTimeGuidewirePullArrived = pyqtSignal()
    posFollowMotion = pyqtSignal()

    def __init__(self, input_cache, output_cache):
        super(RCPContext, self).__init__()

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
                msg = self.input_cache.get_front_message_by_index(i)
                if msg is not None:
                    ci = LienaControlInstruction()
                    body = msg.get_message_body()
                    if int(body[0]) == 0:
                        self.closeSystemMessageArrived.emit()
                    elif int(body[0]) == 1:
                        self.nonProvedControlMessageArrived.emit()
                    elif int(body[0]) == 2:
                        gwts = 0
                        if int(body[2]) == 0:
                            gwts = -1 * (int(body[3]) * 256 + int(body[4]))
                            ci.set_guidewire_translational_speed(gwts)
                        elif int(body[2]) == 1:
                            gwts = 1 * (int(body[3]) * 256 + int(body[4]))
                            ci.set_guidewire_translational_speed(gwts)

                        gwrs = 0
                        if int(body[7]) == 0:
                            gwrs = -1 * (int(body[8]) * 256 + int(body[9]))
                            ci.set_guidewire_rotational_speed(gwrs)
                        elif int(body[7]) == 1:
                            gwrs = 1 * (int(body[8]) * 256 + int(body[9]))
                            ci.set_guidewire_rotational_speed(gwrs)
                        #print("gwts: ", gwts/100, gwrs/100)

                        chars = 0
                        if int(body[13]) == 0:
                            chars = -1 * (int(body[14]) * 256 + int(body[15]))
                            ci.set_catheter_translational_speed(chars)
                        elif int(body[13]) == 1:
                            chars = 1 * (int(body[14]) * 256 + int(body[15]))
                            ci.set_catheter_translational_speed(chars)
                        # print ("parse_command:", gwts, gwrs, chrs)

                        chars1 = int(body[22]) * 256 + int(body[23])
                        if int(body[19]) == 0:
                            ci.set_contrast_media_volume(-1*chars1)
                        elif int(body[19]) == 1:
                            ci.set_contrast_media_volume(chars1)
                        chars = int(body[20]) * 256 + int(body[21])
                        ci.set_contrast_media_speed(chars)
                        self.controlMessageArrived.emit(ci)
                    elif int(body[0]) == 3:
                        self.endovascularPrepareAnotherTour.emit()
                    # self.controlInstruction.append(ci)
                    elif int(body[0]) == 4:
                        self.endovascularGoHomeArrived.emit()
                    elif int(body[0] == 5):
                        self.endovascularMultiTimeGuidewirePullArrived.emit()
                    elif int(body[0] == 6):
                        #quanzeng to do...
                        gwts = 0
                        if int(body[1]) == 0:
                            gwts = -1 * (int(body[2]) * 256 + int(body[3]))
                            ci.set_guidewire_translational_speed(gwts)
                        elif int(body[1]) == 1:
                            gwts = 1 * (int(body[2]) * 256 + int(body[3]))
                            ci.set_guidewire_translational_speed(gwts)

                        gwrs = 0
                        if int(body[4]) == 0:
                            gwrs = -1 * (int(body[5]) * 256 + int(body[6]))
                            ci.set_guidewire_rotational_speed(gwrs)
                        elif int(body[4]) == 1:
                            gwrs = 1 * (int(body[5]) * 256 + int(body[6]))
                            ci.set_guidewire_rotational_speed(gwrs)
                        print("gwts: ", gwts/100, gwrs/100)
                        self.posFollowMotion.emit()

                self.inputLock.release()
            time.sleep(0.05)

    def get_current_time_in_microsecond(self):
        return round((time.time() % 86400) * 1000000)

    def real_time_feedback(self, sys_status=0, gtv=0, grv=0, gtd=0, grd=0, gtf=0, grf=0, ctv=0, ctd=0, ctf=0, lvr=0, lis=0, ltf=0):
        msg = LienaCustomizedMessage(NORMAN_ENDOVASCULAR_ROBOTIC_FEEDBACK_INFORMATION,
                                     SIAT_COCKPIT_VERSION_1,
                                     NORMAN_ENDOVASCULAR_ROBOTIC_VERSION_1,
                                     self.get_current_time_in_microsecond(),
                                     36)

        msg.define_body_length(128 - HEAD_SIZE)
        msg.append_uint8(sys_status)
        msg.append_uint8(positive_or_negtive(gtv))
        msg.append_uint16(abs(gtv))
        msg.append_uint8(positive_or_negtive(grv))
        msg.append_uint16(abs(grv))
        msg.append_uint8(positive_or_negtive(gtd))
        msg.append_uint16(abs(gtd))
        msg.append_uint8(positive_or_negtive(grd))
        msg.append_uint16(abs(grd))
        msg.append_uint8(positive_or_negtive(gtf))
        msg.append_uint16(abs(gtf))
        msg.append_uint8(positive_or_negtive(grf))
        msg.append_uint16(abs(grf))
        msg.append_uint8(positive_or_negtive(ctv))
        msg.append_uint16(abs(ctv))
        msg.append_uint8(positive_or_negtive(ctd))
        msg.append_uint16(abs(ctd))
        msg.append_uint8(positive_or_negtive(ctf))
        msg.append_uint16(abs(ctf))
        msg.append_uint8(positive_or_negtive(lvr))
        msg.append_uint16(abs(lvr))
        msg.append_uint8(positive_or_negtive(lis))
        msg.append_uint16(abs(lis))
        msg.append_uint8(positive_or_negtive(ltf))
        msg.append_uint16(abs(ltf))
        # msg.print_self()
        self.output_cache.write_message_by_index(0, msg)

        # while True:
        #     parameter = SensingParameter()
        #     parameter.setTimestamps(10)
        #     parameter.setForceFeedback(self.globalForceFeedback)
        #     parameter.setTorqueFeedback(self.globalTorqueFeedback)
        #     parameter.setDistanceFromChuckToCatheter(10)
        #     parameter.setTelescopicRodLength(10)
        #     parameter.setDistanceFromCatheterToGuidewire(10)
        #     parameter.setGuidewireAngle(10)
        #     parameter.setTranslationVelocity(10)
        #     parameter.setRotationVelocity(10)
        #     # self.sensingParameterSequence.append(parameter)
        #     # print 'length',len(self.sensingParameterSequence)
        #     # print "forcefeedback ", parameter.getForceFeedback(), "torquefeedback ", parameter.getTorqueFeedback()
        #     time.sleep(0.3)

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
