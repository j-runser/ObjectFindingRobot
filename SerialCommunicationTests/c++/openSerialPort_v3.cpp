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
    cfsetispeed(&portSettings, B9600); // set in baud
    cfsetospeed(&portSettings, B9600); // set out baud
    portSettings.c_cflag &= ~PARENB; // no parity
    portSettings.c_cflag &= ~CSTOPB; // not stop bit
    portSettings.c_cflag &= ~CSIZE; // no word size tag
    portSettings.c_cflag &= ~CRTSCTS; // no hardware flow control
    portSettings.c_cflag |= CS8; // sets word size tag to 8-bit
    portSettings.c_lflag |= ICANON; // conical mode (read and write)
    portSettings.c_cflag |= CREAD | CLOCAL; // enable reciver, ignore modem control
    tcsetattr(fileDescriptor, TCSANOW, &portSettings); // set the attributes
    
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
