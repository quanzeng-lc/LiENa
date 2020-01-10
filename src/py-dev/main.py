#!/usr/bin/python2
# -*- coding: utf-8 -*-

"""
procedure to manage session

author: Cheng WANG

last edited: January 2015
"""

"""
import sys
import os
import time
from RCPCom.RCPComStack import RCPComStack
from RCPContext.RCPContext import RCPContext
from RCPControl.Dispatcher import Dispatcher


def main():
    context = RCPContext()
    
    com_stack = RCPComStack(context)
    
    instruments = Dispatcher(context)

    com_stack.connectera("192.168.1.172", 10704)


if __name__ == '__main__':
    main()
"""

"""
Scripts to test LiENa middleware

author: Cheng WANG

last edited: December 2018
"""

import sys
from PyQt5.QtCore import QCoreApplication
from LiENa.liena import Liena
from LiENa.LiENaBasic.lienaDefinition import *
from RCPContext.RCPContext import RCPContext
from RCPControl.Dispatcher import Dispatcher
#from RCPControl.NewDispatcher import NewDispatcher


def main():
    app = QCoreApplication(sys.argv)

    communication_stack = Liena()
    communication_stack.set_local_device_id(NORMAN_ENDOVASCULAR_ROBOTIC_VERSION_1)
    communication_stack.launch()
    communication_stack.open_session_request(SIAT_COCKPIT_VERSION_1, "192.168.1.142", 10704)

    context = RCPContext(communication_stack.get_input_cache(), communication_stack.get_output_cache())

    instruments = Dispatcher(context)
    # instruments = NewDispatcher(context)
    sys.exit(app.exec_())


if __name__ == '__main__':
    main()
