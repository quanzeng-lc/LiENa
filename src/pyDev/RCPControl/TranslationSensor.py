import struct

import modbus_tk
import modbus_tk.defines as cst
import serial
from modbus_tk import modbus_rtu


class TransLationSensor(object):
    
    def __init__(self, port, baudrate, bytesize, parity, stopbits):
        #serial parameter set
        self.PORT = port
        self.baudrate = baudrate
        self.bytesize = bytesize
        self.parity = parity
        self.stopbits = stopbits
        self.forceFeedback  = 0
        self.hapticFeedbackID = 0
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
            output = self.master.reaction(1, cst.READ_HOLDING_REGISTERS, 30, 2)
            bb = struct.unpack('>i', struct.pack('>HH', output[0], output[1]))
            ret = bb[0]
        except Exception as e:
            print("serial abnormal:", e)
        return ret

"""
forcePORT = "/dev/ttyUSB0"
baudrate = 9600
bytesize = 8
parity = 'N'
stopbits = 1
transLation = TransLationSensor(forcePORT, baudrate, bytesize, parity, stopbits)
while True:
    transLation_value = transLation.aquireForce()
    time.sleep(0.01)
    print("force", transLation_value)
"""
