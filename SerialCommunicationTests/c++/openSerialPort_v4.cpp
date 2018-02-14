#include <sstream>
#include <stdlib.h>
#include <iostream>
#include <fcntl.h>  /* File Control Definitions          */
#include <termios.h>/* POSIX Terminal Control Definitions*/
#include <unistd.h> /* UNIX Standard Definitions         */
#include <string>

using namespace std;

int main() {

    struct termios portSettings; // allows the setting of things like buad, output mode. etc

    int fileDescriptor;
    
    string writeBuffer;
    char readBuffer[64];
    int bytesWritten;
    int bytesRead;
    
    char *port = (char*)"/dev/ttyACM1";

    // open the port
    fileDescriptor = open(port,O_RDWR | O_NOCTTY);
    if (fileDescriptor == 1) { cout << "Error in Opening ttyACM0" << endl; }
    else { cout << "ttyACM0 Opened Successfully" << endl; }
    
    // wait for arduino restart
    usleep(3500000);
    
    // get and set the attribues for the port, make into function, maybe broke
    tcgetattr(fileDescriptor, &portSettings); // get the attributes
    portSettings.c_iflag = IGNBRK | IGNPAR; // ignore break and parity errors
    portSettings.c_oflag = 0; // no flow control
    portSettings.c_lflag = 0;
    cfsetspeed(&portSettings, B9600); // set baud
    tcsetattr(fileDescriptor, TCSANOW, &portSettings); // set the attributes
    
    // make sure the settings are in place
    usleep(1000);
    
    while(true) {
        
        // variables should be reset every itteration
        int command;
        int i;
        stringstream strStream; 

        cout << "Input 1 or 0 (10 to terminate): ";
        cin >> command;
        
        if(command == 10) { break; }

        // convert from int to string, make into function
        strStream << command;
        writeBuffer = strStream.str();
        
        // Flush anything already in the serial buffer
        // tcflush(fileDescriptor, TCIFLUSH);
        
        // write to the port
        bytesWritten = write(fileDescriptor, writeBuffer.c_str(), writeBuffer.length());
    
        // read from the port, is broke
        bytesRead = read(fileDescriptor, readBuffer, 64);
        
        // output test data
        cout << "Bytes Written: " << bytesWritten << endl;
        cout << "Bytes Read: " << bytesRead << endl;
        cout << "Write Buffer: " << writeBuffer << endl;
        cout << "Read buffer: " << readBuffer;

    }
    
    close(fileDescriptor);
    return 0;
}
