#!/usr/bin/python
# Filename: initialiseTest.py

# Testing openEMS modules ability to connect to control board and communicate over serial

import openEMS

brd = openEMS.openEMS("COM4", 19200)

ch1 = brd.makeCommand(1, 100, 5000)
ch2 = brd.makeCommand(2, 100, 5000)

brd.send(ch1[1])
brd.send(ch2[1])

brd.shutdown()
