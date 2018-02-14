/*
 * Purpose: Use the cartesian coordinates of the strawberry as determined by 
 *          the object finding equiptment. It will take these coordinates and
 *          convert them to servo angles and return them in the format:
 *          "m1 m2 m3 m4".
 *
 * Errors: Too many arguments:     -1
 *         Too few arguments:      -2
 *         Unsucessful conversion: -3
 *
 * Author: Joe Runser - 4 April 2017
 *
 * Version: 2.0
 */

#include <iostream>
#include "CartesianToServo_v2.h"

using namespace std;

// main function for sending the servo angles to the arduino
int main(int argc, char* argv[]) {

    // The Cartesian Coordinates
    double x;
    double y;
    double z;
    
    int error;

    // Gets the required arguments from the command line and puts them into the
    // Cartesian Coordinates.
    if(argc == 4) {
	    x = atoi(argv[1]);
	    y = atoi(argv[2]);
	    z = atoi(argv[3]);
    } 
    else if(argc >= 4) {
        error = -1;
        cout << error << endl;
        return 0;
    }
    else if(argc <= 4) {
        error = -2;
        cout << error << endl;
        return 0;
    }
    
    CartesianToServo converter;
    converter.set_coordinates(x, y, z);
    
    if(converter.convertToServo()) {
        int m1 = converter.get_m1();
        int m2 = converter.get_m2();
        int m3 = converter.get_m3();
        int m4 = converter.get_m4();
        
        cout << m1 << " " << m2 << " " << m3 << " " << m4;
        return 0;
    }
    else {
        error = -3;
    }
    

}
