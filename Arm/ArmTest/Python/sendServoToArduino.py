# import libraries
import serial
import subprocess
import time

# get the python verion info
from sys import version_info
py3 = version_info[0] > 2

port = '/dev/ttyACM0'
# this creates the connection and opens the port
arduino = serial.Serial(port, 9600)

# main loop
while True:

    # get user input
    if py3: # for python version 3.x
        coordinates = input('Enter the coordinates (exit to quit): ')
    else: # older versions of python
        coordinates = raw_input('Enter the coordinates (exit to quit): ')
    
    # for quitting
    if coordinates == 'exit':
        quit()
    
    command = './a.out ' + coordinates # create the command
    process = subprocess.Popen(command.split(), stdout=subprocess.PIPE) # run a.out
    output, error = process.communicate() # get the output from the a.out
    splitOutput = output.split() # tokenize the output
    
    # check for errors from the output
    if len(splitOutput) == 4:
        # write and read with arduino
        for variable in splitOutput:
            arduino.write(variable)
            print 'To Arduino: ' + variable
        fromArduino = arduino.readline()
        print 'From Arduino: ' + fromArduino
    elif len(splitOutput) == 1:
        if splitOutput[0] == '-1\n': # too many arguments
            print 'Too many arguments to converter.'
        elif splitOutput == '-2\n': # too few arguments
            print 'Too few arguments to converter.'
        elif splitOutput == '-3\n': # cannot convert for arm
            print 'Illegal coordinates, this may damage the arm.'
    else:
        print 'Unknown error'
            
