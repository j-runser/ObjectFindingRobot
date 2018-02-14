/*
 * This software will connect to a serial device at /dev/ttyACM0 and
 * then allow the user to send either an 'on' (the number 1) or off 
 * (the number 0) command. This is test software thus there is no 
 * error detection or cleaning of the user input.
 * 
 * Special Insturcions: This document software uses 3rd party libraries
 * (listed below). It must be compiled using the special command
 * "$ g++ <thisFile.cpp> -L/usr/local/lib -lserial -pthread" unless the
 * libaries have been set up to compile automatically.
 *
 * LibSerial: https://sourceforge.net/projects/libserial/files/?source=navbar
 *
 * Based on: webtutsdepot.com/2010/05/01/arduino-visual-c-plus-plus-tutorial/
 *
 * Created: 25 January 2017
 * By: Joe Runser
 * 
 * Modified: 27 Janurary 2017
 * By: Joe Runser
*/

#include<SerialStream.h>
#include<iostream>
#include<string>

using namespace LibSerial;
using namespace std;

int main(void) {

    string command;
    
    SerialStream arduino;
    // remember to open the port before manipulating the settings.
    arduino.Open("/dev/ttyACM0");

    /*
     * For arduino the only prpperty that is required is the BAUD.
     * There are also many other properties to inlcud but not limitted to
     * Charecter size, Number of stop bits, Parity size, and Flow-Control
     * type.
    */
    // set some specific properties for the serial connection
    arduino.SetBaudRate(SerialStreamBuf::BAUD_9600);

    do {
        if(arduino.IsOpen()) {
            cout << "Good:\nWould you like the light 'on' or 'off'? ";
            cin >> command;

            if(command == "on") { arduino << 1 << endl; }
            else if(command == "off") { arduino << 0 << endl; }
	        else break;

        } else { cout << "Not good.\n"; break; }

    } while(true);

    arduino.Close();

    return 0;
}
