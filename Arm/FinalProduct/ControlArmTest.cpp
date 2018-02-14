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
#include "ControlArm.h"

using namespace std;

// main function for sending the servo angles to the arduino
int main(int argc, char* argv[]) {

    // The Cartesian Coordinates
    double x;
    double y;
    double z;

    ControlArm arm("/dev/ttyACM0");

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
    
    if(arm.set_servoAngles(x, y, z)) {
        cout << "Conversion complete." << endl;
    
        do {
            if(arm.sendServoToArm()) { break; } 
            else { 
                // No connetion.
                cout << "The connection failed." << endl;

                cout << "Servo 1: " << arm.get_m1() << endl;
                cout << "Servo 2: " << arm.get_m2() << endl;
                cout << "Servo 3: " << arm.get_m3() << endl;
                cout << "servo 4: " << arm.get_m4() << endl;

                break;
            }
        } while(true);

    } else { cout << "Illegal coordinates." << endl; }

}
