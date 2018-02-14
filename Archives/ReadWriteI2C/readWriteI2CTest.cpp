/*
 * Purpose: Test readWriteI2C.h 
 *
 * Author: Joe Runser - 8 March 2017
 *
 * Edited: Joe Runser - 8 March  2017
 *
 * Version: 1.0
*/

#include "readWriteI2C.h"
#include <iostream>

using namespace std;

int main() {

    unsigned char deviceAddress = 0x04;
    unsigned char regAddress = 4;
    char* deviceFilename = (char*)"/dev/i2c-1";

    int one = 5;
    int two = 7;
    unsigned char three = 0;

    ReadWriteI2C testArduino(deviceAddress, deviceFilename);

    testArduino.writeReg(regAddress, one);
    testArduino.writeReg(regAddress, two);

    testArduino.readReg(regAddress, three);

    cout << "This is the sum of your numbers: " << three << endl;

    return 0;

}
