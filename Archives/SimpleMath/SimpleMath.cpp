/*
 * Connect to an ardino and send the arduino two integer numbers which will
 * then be added on the arduino and the sum of which will be sent back.
 * 
 * Special Insturcions: This document software uses 3rd party libraries
 * (listed below). It must be compiled using the special command
 * "$ g++ <thisFile.cpp> -L/usr/local/lib -lserial -pthread" unless the
 * libaries have been set up to compile automatically.
 *
 * LibSerial: https://sourceforge.net/projects/libserial/files/?source=navbar
 *
 * Created: 27 January 2017
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

    int numOne;
    int numTwo;
    int sum;
    string port = "/dev/ttyACM0";

//    cout << "What port would you like to use: ";
//    cin >> port;

    // Open a given port
    SerialStream arduino;
    arduino.Open(port);
    arduino.SetBaudRate(SerialStreamBuf::BAUD_9600);

    do {
        if(arduino.IsOpen()) {
            cout << "What two numbers would you like to add: ";
            cin >> numOne >> numTwo;

            arduino << numOne << endl;
            arduino << numTwo << endl;

            arduino >> sum;

            cout << "This is the sum: " << sum << endl;

            break;

        } else { cout << port << " is a bad connection.\n"; break; }

    } while(true);

    arduino.Close();

    return 0;
}
