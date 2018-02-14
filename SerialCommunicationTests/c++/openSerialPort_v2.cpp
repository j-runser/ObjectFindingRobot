#include <iostream>
#include <fcntl.h>  /* File Control Definitions          */
#include <termios.h>/* POSIX Terminal Control Definitions*/
#include <unistd.h> /* UNIX Standard Definitions         */
#include <errno.h>  /* ERROR Number Definitions          */

using namespace std;

int main() {

    struct termios portSettings; // allows the setting of things like buad, output mode. etc

    int fileDescriptor;
    
    char writeBuffer[] = "0";
    char readBuffer[32];
    int bytesWritten = 0;
    int bytesRead;

    // open the port
    fileDescriptor = open("/dev/ttyACM1",O_RDWR | O_NOCTTY);
    if (fileDescriptor == 1) { cout << "Error in Opening ttyACM0" << endl; }
    else { cout << "ttyACM0 Opened Successfully" << endl; }
    
    // wait for arduino restart
    usleep(3500000);
    
    // get and set the attribues for the port
    tcgetattr(fileDescriptor, &portSettings); // get the attributes
    cfsetispeed(&portSettings, B9600); // set in baud
    cfsetospeed(&portSettings, B9600); // set out baud
    portSettings.c_cflag &= ~PARENB; // no parity
    portSettings.c_cflag &= ~CSTOPB; // not stop bit
    portSettings.c_cflag &= ~CSIZE; // no word size tag
    portSettings.c_cflag |= CS8; // sets word size tag to 8-bit
    portSettings.c_lflag |= ICANON; // conical mode (read and write)
    tcsetattr(fileDescriptor, TCSANOW, &portSettings); // set the attributes
    
    // write to the port
    bytesWritten = write(fileDescriptor, writeBuffer, sizeof(writeBuffer));
    
    // read from the port
    bytesRead = read(fileDescriptor, readBuffer, 32);
    
    cout << "Bytes Written: " << bytesWritten << endl;
    cout << "Bytes Read: " << bytesRead << endl;
    cout << "Write Buffer: " << writeBuffer << endl;
    cout << "Read buffer: " << readBuffer;
    
    close(fileDescriptor);
    return 0;
}
