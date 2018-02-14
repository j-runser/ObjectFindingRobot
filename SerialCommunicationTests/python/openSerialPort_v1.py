# Purpose: Use serial protocol to connect to an Arduino Uno and send one input
#          and recive one output.
#
# Author: Joe Rusner - 3 April 2017
#
# Edited: Joe Rusner - 4 May 2017
#
# Note: Make sure the port is correct, the port can also be a persistant name.

# import libraries
import serial
import time

# get the python verion info
from sys import version_info
py3 = version_info[0] > 2

# name for port
port = '/dev/ttyACM0'

# this creates the connection and opens the port
arduino = serial.Serial(port, 9600)

# main loop
while True:

    # get user input
    if py3: # for python version 3.x
        outData = input('Enter 1 or 0 (exit to quit): ')
    else: # older versions of python
        outData = raw_input('Enter 1 or 0 (exit to quit): ')
        
    if outData == 'exit':
        quit()
        
    # write to the arduino
    arduino.write(outData)
    
    # read from arduion
    inData = arduino.readline()
    
    # print results
    print 'Output: ' + outData
    print 'Input: ' + inData