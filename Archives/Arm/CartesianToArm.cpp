/*
 * Purpose: This software will convert Cartesian Coordinates into servo angles for
 * the Braccio Arm.
 *
 * Author: Skye VanAtta 8 February 2017
 *
 * Edited: Joe Runser 16 February 2017
 *
 * Version: 1.1
 */

#include <iostream>
#include <string>
#include "CartesianToServo_v2.h"
#include <SerialStream.h>

using namespace std;
using namespace LibSerial;

// main function for sending the servo angles to the arduino
int main(int argc, char* argv[]) {

    // The Cartesian Coordinates
    double x;
    double y;
    double z;

    // Port the Arduino is attached to
    string port = "/dev/ttyACM0";

    // Will be used to make sure the data transfer is complete
    bool doneStatus = false;

    // Gets the required arguments from the command line and puts them into the
    // Cartesian Coordinates.
    if(argc == 4) {
	    x = atoi(argv[1]);
	    y = atoi(argv[2]);
	    z = atoi(argv[3]);
    } else {
        cout << "Please give an x, y and z." << endl;
        return 0;
    }

    // Set up the connection with the arduino
    SerialStream armArduino;
    armArduino.Open(port);
    armArduino.SetBaudRate(SerialStreamBuf::BAUD_9600);

    // Convert the servo angles
    CartesianToServo converter;
    converter.set_coordinates(x, y, z);
    
    if(converter.convertToServo()) {
        cout << "Conversion complete." << endl;
    
        do {
            if(armArduino.IsOpen()) {
                cout << "Sending data." << endl;
                // Send the arduino angle for each servo.
	            armArduino << converter.get_m1() << endl;
	            armArduino << converter.get_m2() << endl;
	            armArduino << converter.get_m3() << endl;
	            armArduino << converter.get_m4() << endl;

                armArduino >> doneStatus;

                cout << "Done: " << doneStatus << endl;

                // Terminate the connection.
    	        break;
    	    } else {
                // No connetion.
                cout << "The connection failed." << endl;

                cout << "Servo 1: " << converter.get_m1() << endl;
                cout << "Servo 2: " << converter.get_m2() << endl;
                cout << "Servo 3: " << converter.get_m3() << endl;
                cout << "servo 4: " << converter.get_m4() << endl;
            
                doneStatus = true;
                break;
            }
        } while(doneStatus);
    } else {
        cout << "Illegal coordinates." << endl;
    }

}
