# -*- coding: utf-8 -*-
import threading
import time
from LiENaBasic.lienaDefinition import *
from LiENaStructure.LiENaDatagram.LienaDatagram import LienaDatagram
from LiENaStructure.LiENaMessage.LienaHeartbeatMessage import LienaHeartbeatMessage


class LienaHearBeatTask:
    def __init__(self, output_queue, global_parameter):
        self.output_queue = output_queue
        self.global_parameter = global_parameter

        self.outputMessageCache = None
        self.rtPeriod = 1
        self.stand_by = False

        self.flag = None
        self.heartbeat_thread = threading.Thread(None, self.heartbeat)

    def heartbeat(self):
        while self.flag:

            if self.stand_by:
                time.sleep(1)
                continue

            self.send_heartbeat_message()
            time.sleep(self.rtPeriod)

    def encode_heartbeat_message(self, message):
        bytes_to_send = bytearray(self.global_parameter.get_global_datagram_size())

        data_type_msb = message.get_message_id() // (2 ** 32)
        data_type_lsb = message.get_message_id() % (2 ** 32)

        bytes_to_send[0] = (data_type_msb & 0xff000000) >> 24
        bytes_to_send[1] = (data_type_msb & 0x00ff0000) >> 16
        bytes_to_send[2] = (data_type_msb & 0x0000ff00) >> 8
        bytes_to_send[3] = (data_type_msb & 0x000000ff)
        bytes_to_send[4] = (data_type_lsb & 0xff000000) >> 24
        bytes_to_send[5] = (data_type_lsb & 0x00ff0000) >> 16
        bytes_to_send[6] = (data_type_lsb & 0x0000ff00) >> 8
        bytes_to_send[7] = (data_type_lsb & 0x000000ff)

        bytes_to_send[8] = (message.get_target_id() & 0xff000000) >> 24
        bytes_to_send[9] = (message.get_target_id() & 0x00ff0000) >> 16
        bytes_to_send[10] = (message.get_target_id() & 0x0000ff00) >> 8
        bytes_to_send[11] = (message.get_target_id() & 0x000000ff)

        bytes_to_send[12] = (message.get_origin_id() & 0xff000000) >> 24
        bytes_to_send[13] = (message.get_origin_id() & 0x00ff0000) >> 16
        bytes_to_send[14] = (message.get_origin_id() & 0x0000ff00) >> 8
        bytes_to_send[15] = (message.get_origin_id() & 0x000000ff)

        bytes_to_send[16] = (message.get_timestamps() & 0xff00000000000000) >> 56
        bytes_to_send[17] = (message.get_timestamps() & 0x00ff000000000000) >> 48
        bytes_to_send[18] = (message.get_timestamps() & 0x0000ff0000000000) >> 40
        bytes_to_send[19] = (message.get_timestamps() & 0x000000ff00000000) >> 32
        bytes_to_send[20] = (message.get_timestamps() & 0x00000000ff000000) >> 24
        bytes_to_send[21] = (message.get_timestamps() & 0x0000000000ff0000) >> 16
        bytes_to_send[22] = (message.get_timestamps() & 0x000000000000ff00) >> 8
        bytes_to_send[23] = (message.get_timestamps() & 0x00000000000000ff)

        bytes_to_send[24] = (message.get_dlc() & 0xff000000) >> 24
        bytes_to_send[25] = (message.get_dlc() & 0x00ff0000) >> 16
        bytes_to_send[26] = (message.get_dlc() & 0x0000ff00) >> 8
        bytes_to_send[27] = (message.get_dlc() & 0x000000ff)

        for x in range(28, self.global_parameter.get_global_datagram_size()):
            bytes_to_send[x] = 0

        return bytes_to_send

    def send_heartbeat_message(self):
        if DEBUG:
            print("send_heartbeat_message")
        heartbeat_message = LienaHeartbeatMessage(LIENA_SESSION_MANAGEMENT_HEARTBEAT_MESSAGE,
                                                  0,
                                                  0,
                                                  round((time.time() % 86400) * 1000000),
                                                  0)
        datagram = LienaDatagram(self.global_parameter.get_global_datagram_size(), self.encode_heartbeat_message(heartbeat_message))
        self.output_queue.append(datagram)

    def launch(self):
        self.flag = True
        self.heartbeat_thread.start()

    def set_output_cache(self, output_message_cache):
        self.outputMessageCache = output_message_cache

    def terminate(self):
        self.output_queue.clear()
        self.flag = False

    def freeze(self):
        self.stand_by = True

    def enable(self):
        self.stand_by = False