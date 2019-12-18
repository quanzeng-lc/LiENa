#!/usr/bin/python3
# -*- coding: utf-8 -*-

"""
Scripts to test LiENa middleware

author: Cheng WANG

last edited: December 2018
"""

import sys
from PyQt5.QtCore import QCoreApplication
from liena import Liena
from LiENaBasic.lienaDefinition import *


def main():
    app = QCoreApplication(sys.argv)

    communication_stack = Liena()
    communication_stack.set_local_device_id(NORMAN_ENDOVASCULAR_ROBOTIC_VERSION_1)
    communication_stack.launch()
    communication_stack.open_session_request(SIAT_COCKPIT_VERSION_1, "192.168.1.142", 10704)

    sys.exit(app.exec_())


if __name__ == '__main__':
    main()
