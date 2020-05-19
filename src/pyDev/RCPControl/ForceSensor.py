import struct
import time
import modbus_tk
import modbus_tk.defines as cst
import serial
from modbus_tk import modbus_rtu


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


#self.translationalForceSensor = ForceSensor("/dev/ttyusb_force", 9600, 8, 'N', 1)
#self.rotationalForceSensor = ForceSensor("/dev/ttyusb_torque", 9600, 8, 'N', 1)
force_port1 = "/dev/ttyusb_force"
force_port2 = "/dev/ttyusb_force"
baud_rate = 9600
byte_size = 8
parity = 'N'
stop_bits = 1
force_feedback1 = ForceSensor(force_port1, baud_rate, byte_size, parity, stop_bits)
force_feedback2 = ForceSensor(force_port2, baud_rate, byte_size, parity, stop_bits)
while True:
    force_value = force_feedback1.get_value()
    torque_value = force_feedback2.get_value()
    time.sleep(0.01)
    print(force_value, torque_value)
