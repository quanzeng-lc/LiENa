import struct
import time
import modbus_tk
import modbus_tk.defines as cst
import serial
from modbus_tk import modbus_rtu
import numpy as np
from scipy import signal


class ForceSensor(object):
    
    def __init__(self, port, baudrate, bytesize, parity, stopbits):
        #serial parameter set
        self.PORT = port
        self.baudrate = baudrate
        self.bytesize = bytesize
        self.parity = parity
        self.stopbits = stopbits
        self.forceFeedback  = 0
        self.hapticFeedbackID = 0
        self.isAveragy = False;
        fs = 26
        fc = 3
        w = 2*fc/fs
        self.shift = 0
        self.b, self.a = signal.butter(2, w, 'low')
        try:
            self.master = modbus_rtu.RtuMaster(serial.Serial(port=self.PORT, baudrate=self.baudrate, bytesize=self.bytesize, parity=self.parity, stopbits=self.stopbits, xonxoff=0))
            self.master.set_timeout(4)
            self.master.set_verbose(True)
            # logger.info("connected")
        except modbus_tk.modbus.ModbusError as exc:
            # logger.error("%s- Code=%d", exc, exc.get_exception_code())
            print("error")
        # self.feedbackTask = threading.Thread(None, self.aquireForce)

    def get_value(self):
        ret = 65530
        try:
            output = self.master.execute(1, cst.READ_HOLDING_REGISTERS, 30, 2)
            bb = struct.unpack('>i', struct.pack('>HH', output[0], output[1]))
            ret = bb[0]
        except Exception as e:
            print("serial abnormal:", e)
        return ret

    def get_value_filter(self):
        if not self.isAveragy:
            sumValue = 0;
            for i in range(100):
                sumValue = sumValue + self.get_value()
            self.shift = sumValue / 100
            self.isAveragy = True
        #print("shift: ", self.shift)
        data = self.get_value() - self.shift
        output = signal.filtfilt(self.b, self.a, np.array([data]), padlen=0)
        return int(output[0])



"""
#self.translationalForceSensor = ForceSensor("/dev/ttyusb_force", 9600, 8, 'N', 1)
#self.rotationalForceSensor = ForceSensor("/dev/ttyusb_torque", 9600, 8, 'N', 1)
force_port1 = "/dev/ttyUSB0"
#force_port2 = "/dev/ttyusb_torque"
baud_rate = 9600
byte_size = 8
parity = 'N'
stop_bits = 1
force_feedback1 = ForceSensor(force_port1, baud_rate, byte_size, parity, stop_bits)
#force_feedback2 = ForceSensor(force_port2, baud_rate, byte_size, parity, stop_bits)

while True:
    force_value = force_feedback1.get_value_filter()
    #torque_value = force_feedback2.get_value()
    time.sleep(0.1)
    #print(force_value, torque_value)
    print(force_value)
"""
