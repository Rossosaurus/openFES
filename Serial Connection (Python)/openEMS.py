#!/usr/bin/python
# Filename: openEMS.py

# openEMS python module handling communication to openEMSStim device using serial over USB

import serial
from time import sleep

class openEMS():
    def __init__(self, serialPort, baudrate=19200):
        self.serialPort = serialPort
        self.baudrate = baudrate
        self.sleepTime = 10
        self.ems = None
        
        try:
            self.ems = serial.Serial(self.serialPort, self.baudrate)
        except serial.SerialException:
            print("Exception Raised: serial device: " + str(self.serialPort) + " was not found")
            return None
        
        print ("Waiting " + str(self.sleepTime) + "seconds for board to initialise...\n")
        sleep(self.sleepTime)
        print("Ready...\n")
    
    def shutdown(self):
        self.ems.close()    
            
    def makeCommand(self, C, I, D):
        txt = ""
        channel = int(C)
        intensity = int(I)
        duration = int(D)
        
        if channel == 1 or channel == 2:
            txt += str("C" + str(channel - 1))
        else:
            print("Incorrect channel number input: " + str(channel) + " is not an available channel to activate. Channel must be set to '1' or '2'")
        if intensity >= 0 and intensity <= 100:
            txt += str("I" + str(intensity))
        else:
            print("Incorrect intensity input: " + str(intensity) + "\nIntensity must be an integer between '0' and '100'")
            
        if duration >= 0:
            txt += str("T" + str(duration) + "G")
        else:
            print("Incorrect duration input: " + str(duration) + "\nDuration must be an integer of signal duration in ms")
    
    def send(self, cmd):
        cmd = bytes(cmd)
        print("Successfully converted command string to: " + str(type(cmd)))
        if self.ems and cmd:
            self.ems.write(cmd)
        else:
            print("Problem creating EMS device on: " + str(self.serialPort) + ", command: " + str(cmd) + "could not be sent")
    

    
    