from ctypes import *
import time


#Type redefine!
BOOL = c_int
DWORD = c_ulong
HANDLE = c_void_p
UINT = c_uint
CHAR = c_char_p
USHORT = c_ushort
LONG = c_long
INT = c_int


# Rotation Motor Class
class MaxonMotor(object):

    BOOL = c_int
    DWORD = c_ulong
    HANDLE = c_void_p
    UINT = c_uint
    CHAR = c_char_p
    USHORT = c_ushort
    LONG = c_long
    INT = c_int
    
    """rotation motor"""
    def __init__(self, RMNodeId, pDeviceName, pProtocolStackName, pInterfaceName, pPortName, lBaudrate):
        #  Type redefine!
        
        self.RMNodeId = USHORT(RMNodeId)
        self.pDeviceName = CHAR(pDeviceName.encode('utf-8'))
        self.pProtocolStackName = CHAR(pProtocolStackName.encode('utf-8'))
        self.pInterfaceName = CHAR(pInterfaceName.encode('utf-8'))
        self.pPortName = CHAR(pPortName.encode('utf-8'))
        self.lBaudrate = UINT(lBaudrate)
        self.RMHandle = HANDLE(0)
        self.errorCode = UINT(0)
        self.lTimeout = UINT(0)
        #  self.relativePosition = LONG(relativePosition)
        self.rmPosition = INT(0)
        self.rmVelosity = INT(0)


        self.rotationMotor = cdll.LoadLibrary("libEposCmd.so")
        #  Open Device
        self.OpenDevice = self.rotationMotor.VCS_OpenDevice
        self.OpenDevice.argtypes = [CHAR, CHAR, CHAR, CHAR, POINTER(UINT)]
        self.OpenDevice.restype = HANDLE

        #  Communication Info
        self.GetProtocolStackSettings = self.rotationMotor.VCS_GetProtocolStackSettings
        self.GetProtocolStackSettings.argtypes = [HANDLE, POINTER(UINT), POINTER(UINT), POINTER(UINT)]
        self.GetProtocolStackSettings.restype = BOOL

        self.SetProtocolStackSettings = self.rotationMotor.VCS_SetProtocolStackSettings
        self.SetProtocolStackSettings.argtypes = [HANDLE, UINT, UINT, POINTER(UINT)]
        self.SetProtocolStackSettings.restype = BOOL

        #  Enable Motor
        self.SetEnableState = self.rotationMotor.VCS_SetEnableState
        self.SetEnableState.argtypes = [HANDLE, USHORT, POINTER(UINT)]
        self.SetEnableState.restype = BOOL

        self.GetEnableState = self.rotationMotor.VCS_GetEnableState
        self.GetEnableState.argtypes = [HANDLE, USHORT, POINTER(BOOL), POINTER(UINT)]
        self.GetEnableState.restype = BOOL

        self.SetDisableState = self.rotationMotor.VCS_SetDisableState
        self.SetDisableState.argtypes = [HANDLE, USHORT, POINTER(UINT)]
        self.SetDisableState.restype = BOOL

        self.GetDisableState = self.rotationMotor.VCS_GetDisableState
        self.GetDisableState.argtypes = [HANDLE, USHORT, POINTER(BOOL), POINTER(UINT)]
        self.GetDisableState.restype = BOOL

        #  Clear Fault
        self.GetFaultState = self.rotationMotor.VCS_GetFaultState
        self.GetFaultState.argtypes = [HANDLE, USHORT, POINTER(BOOL), POINTER(UINT)]
        self.GetFaultState.restype = BOOL

        self.ClearFault = self.rotationMotor.VCS_ClearFault
        self.ClearFault.argtypes = [HANDLE, USHORT, POINTER(UINT)]
        self.ClearFault.restype = BOOL

        #  Profile Velocity Mode
        self.MoveWithVelocity = self.rotationMotor.VCS_MoveWithVelocity
        self.MoveWithVelocity.argtypes = [HANDLE, USHORT, LONG, POINTER(UINT)]
        self.MoveWithVelocity.restype = BOOL

        self.ActivateProfileVelocityMode = self.rotationMotor.VCS_ActivateProfileVelocityMode
        self.ActivateProfileVelocityMode.argtypes = [HANDLE, USHORT, POINTER(UINT)]
        self.ActivateProfileVelocityMode.restype = BOOL

        self.HaltVelocityMovement = self.rotationMotor.VCS_HaltVelocityMovement
        self.HaltVelocityMovement.argtypes = [HANDLE, USHORT, POINTER(UINT)]
        self.HaltVelocityMovement.restype = BOOL

        #Velocity Mode
        self.ActivateVelocityMode = self.rotationMotor.VCS_ActivateVelocityMode
        self.ActivateVelocityMode.argtypes = [HANDLE, USHORT, POINTER(UINT)]
        self.ActivateVelocityMode.restype = BOOL
        
        self.SetVelocityMust = self.rotationMotor.VCS_SetVelocityMust
        self.SetVelocityMust.argtypes = [HANDLE, USHORT, LONG,POINTER(UINT)]
        self.SetVelocityMust.restype = BOOL
        
        #  Position Mode
        self.ActivateProfilePositionMode = self.rotationMotor.VCS_ActivateProfilePositionMode
        self.ActivateProfilePositionMode.argtypes = [HANDLE, USHORT, POINTER(UINT)]
        self.ActivateProfilePositionMode.restype = BOOL

        self.SetPositionProfile = self.rotationMotor.VCS_SetPositionProfile
        self.SetPositionProfile.argtypes = [HANDLE, USHORT, UINT, UINT, UINT, POINTER(UINT)]
        self.SetPositionProfile.restype = BOOL

        self.MoveToPosition = self.rotationMotor.VCS_MoveToPosition
        self.MoveToPosition.argtypes = [HANDLE, USHORT, LONG, INT, INT, POINTER(UINT)]
        self.MoveToPosition.restype = BOOL

        self.HaltPositionMovement = self.rotationMotor.VCS_HaltPositionMovement
        self.HaltPositionMovement.argtypes = [HANDLE, USHORT, POINTER(UINT)]
        self.HaltPositionMovement.restype = BOOL
        
        #  Max Speed
        self.SetMaxProfileVelocity = self.rotationMotor.VCS_SetMaxProfileVelocity
        self.SetMaxProfileVelocity.argtypes = [HANDLE, USHORT, UINT, POINTER(UINT)]
        self.SetMaxProfileVelocity.restype = BOOL

        #  Motor Speed and Position Info
        self.GetPosition = self.rotationMotor.VCS_GetPositionIs
        self.GetPosition.argtypes = [HANDLE, USHORT, POINTER(INT), POINTER(UINT)]
        self.GetPosition.restype = BOOL

        self.GetVelocity = self.rotationMotor.VCS_GetVelocityIs
        self.GetVelocity.argtypes = [HANDLE, USHORT, POINTER(INT), POINTER(UINT)]
        self.GetVelocity.restype = BOOL

        #  Close Device
        self.CloseDevice = self.rotationMotor.VCS_CloseDevice
        self.CloseDevice.argtypes = [HANDLE, POINTER(UINT)]
        self.CloseDevice.restype = BOOL

        #  Close All Device
        self.CloseAllDevices = self.rotationMotor.VCS_CloseAllDevices
        self.CloseAllDevices.argtypes = [POINTER(UINT)]
        self.CloseAllDevices.restype = BOOL
        
        self.oIsFault = BOOL(0)
        self.oIsEnabled = BOOL(0)
        
        self.lead = 2
        self.resolution = 1000
        self.gear_ratio = 10
        self.direction = True

    def setDirection(self, direction):
        self.direction = direction

    def isSameDirection(self, value):
        if self.direction:
            return value
        else:
            return (-1 * value)

    def setParameter(self, resolution, gear, lead):
        self.lead = lead
        self.resolution = resolution
        self.gear = gear
    
    def transfromQcToPosition(self, qc):
        position = qc * self.lead / (self.resolution * self.gear_ratio);
        position = ((int)(position * pow(10, 3))) / pow(10, 3);
        return position;

    def transfromPositionToQc(self, position):
        qc = position * self.gear_ratio * self.resolution / self.lead;
        return qc;

    def transfromRPMToLineSp(self, rpm):
        lineSp = self.transfromQcToPosition(self.resolution * rpm / 60);
        return lineSp;

    def transfromLineSpToRPM(self, lineSp):
        rmp = self.transfromPositionToQc(lineSp) * 60/ self.resolution;
        return rmp;

    #  Open Device
    def open_device(self):
        Result = 0
        self.RMHandle = self.OpenDevice(self.pDeviceName, self.pProtocolStackName, self.pInterfaceName, self.pPortName, byref(self.errorCode))
        print("Open Device  ", self.RMHandle, self.errorCode.value)
        if self.ClearFault(self.RMHandle, self.RMNodeId, byref(self.errorCode)) != BOOL(0):
            Result = 1
        return Result

    #  Close Device
    def close_device(self):
        Result = 0
        if self.CloseDevice(self.RMHandle, byref(self.errorCode)) != BOOL(0):
            Result = 1
        return Result

    #  Max Speed
    def customMaxSpeed(self, setMaxSpeed):
        maxSpeed = self.isSameDirection(setMaxSpeed)
        Result = 0
        if self.SetMaxProfileVelocity(self.RMHandle, self.RMNodeId, UINT(maxSpeed), byref(self.errorCode)) != BOOL(0):
            Result = 1
        return Result

    def customEnable(self):
        lResult = 0
        oIsFault = BOOL(0)
        if lResult == 0:
            if self.ClearFault(self.RMHandle, self.RMNodeId, byref(self.errorCode)) != BOOL(0):
                lResult = 1
            if lResult == 1:
                if self.GetEnableState(self.RMHandle, self.RMNodeId, byref(self.oIsEnabled), byref(self.errorCode)) \
                        != BOOL(0):
                    lResult = 1
                if lResult == 1:
                    if self.SetEnableState(self.RMHandle, self.RMNodeId, byref(self.errorCode)) != BOOL(0):
                        lResult = 1
        return lResult

    def customDisable(self):
        lResult = 0
        if self.GetFaultState(self.RMHandle, self.RMNodeId, byref(self.oIsFault), byref(self.errorCode)) != BOOL(0):
            lResult = 1
        if lResult == 0:
            if oIsFault:
                if self.ClearFault(self.RMHandle, self.RMNodeId, byref(self.errorCode)) != BOOL(0):
                    lResult = 1
            if lResult == 0:
                if self.GetDisableState(self.RMHandle, self.RMNodeId, byref(self.oIsEnabled), byref(self.errorCode)) \
                        != BOOL(0):
                    lResult = 1

                if lResult == 0:
                    if self.SetDisableState(self.RMHandle, self.RMNodeId, byref(self.errorCode)) != BOOL(0):
                        lResult = 1
        self.SetDisableState(self.RMHandle, self.RMNodeId, byref(self.errorCode))
        return lResult

    def customActivateProfileVelocityMode(self):
        lResult = 0
        if self.ActivateProfileVelocityMode(self.RMHandle, self.RMNodeId, byref(self.errorCode)) != BOOL(0):
            lResult = 1
        return lResult

    def customProfileVelocityMoveWithVelocity(self, velocity):
        lResult = 0
        lineVelocity = self.isSameDirection(int(self.transfromLineSpToRPM(velocity)))
        if self.MoveWithVelocity(self.RMHandle, self.RMNodeId, LONG(lineVelocity), byref(self.errorCode)) != BOOL(0):
            lResult = 1
        return lResult

    def customProfileVelocityHalt(self):
        lResult = 0

        if self.HaltVelocityMovement(self.RMHandle, self.RMNodeId, byref(self.errorCode)) != BOOL(0):
            lResult = 1
        return lResult

    def customActivateProfilePositionMode(self):
        lResult = 0
        if self.ActivateProfilePositionMode(self.RMHandle, self.RMNodeId, byref(self.errorCode)) != BOOL(0):
            lResult = 1
        return lResult

    def customMoveToposition(self, targetPosition):
        lResult = 0
        position = self.isSameDirection(int(self.transfromPositionToQc(targetPosition)))
        if self.MoveToPosition(self.RMHandle, self.RMNodeId, LONG(position), 0, 1, byref(self.errorCode))\
                != BOOL(0):
            lResult = 1
        return lResult

    def customSetPositionProfile(self, velocity, acceleration, deceleration):
        lResult = 0
        lineVelocity = abs(int(self.transfromLineSpToRPM(velocity)))
        if self.SetPositionProfile(self.RMHandle, self.RMNodeId, UINT(lineVelocity), UINT(acceleration),
                                   UINT(deceleration), byref(self.errorCode)) != BOOL(0):
            lResult = 1
        return lResult

    def customProfilePositionHalt(self):
        lResult = 0
        if self.HaltPositionMovement(self.RMHandle, self.RMNodeId, byref(self.errorCode)) != BOOL(0):
            lResult = 1
        return lResult

    def customActivateVelocityMode(self):
        lResult = 0
        if self.ActivateVelocityMode(self.RMHandle, self.RMNodeId, byref(self.errorCode)) != BOOL(0):
            lResult = 1
        return lResult

    def customSetVelocityMust(self,VelocityMust):
        lResult = 0
        transformVelocityMust = self.isSameDirection(int(self.transfromLineSpToRPM(VelocityMust)))
        if self.SetVelocityMust(self.RMHandle, self.RMNodeId,LONG(transformVelocityMust), byref(self.errorCode)) != BOOL(0):
            lResult = 1
        return lResult


guidewireRotateMotor = MaxonMotor(1, "EPOS2", "MAXON SERIAL V2", "USB", "USB0", 1000000)
guidewireRotateMotor.open_device()
guidewireRotateMotor.setDirection(False)
guidewireRotateMotor.customEnable()
guidewireRotateMotor.setParameter(500*4, 1, 4)

guidewireRotateMotor.customActivateProfileVelocityMode()
guidewireRotateMotor.customProfileVelocityMoveWithVelocity(-1)

#guidewireRotateMotor.customActivateProfilePositionMode()
#guidewireRotateMotor.customSetPositionProfile(4,1000,1000)
#guidewireRotateMotor.customMoveToposition(200)
#guidewireRotateMotor.customActivateVelocityMode()
#guidewireRotateMotor.customSetVelocityMust(2)
time.sleep(2)
#guidewireRotateMotor.customProfilePositionHalt()
#guidewireRotateMotor.customSetVelocityMust(0)
guidewireRotateMotor.customProfileVelocityHalt()
guidewireRotateMotor.customDisable()
guidewireRotateMotor.close_device()

