/*
* Code addpated from: https://chrisheydrick.com/2012/06/24/how-to-read-serial-data-from-an-arduino-in-linux-with-c-part-4/
*/

#include <iostream>
//#include <stdio.h>
//#include <stdlib.h>
//#include <sys/ioctl.h> // more reading
#include <fcntl.h>
#include <termios.h> 
//#include <string.h>
//#include <unistd.h> // more reading
//#include <stdint.h> // more reading
//#include <errno.h>
//#include <sys/ioctl.h> // more reading
//#include <string>

#define PORT "/dev/ttyACM0"

using namespace std;

int main() {
    
    string command = 0;
    
    int port_fd;
    char *port;
    struct termios toptions; // connection options
    
    // open port
    fd = open("/dev/ttyACM0", O_RDWR | O_NOCTTY);
    if(fd == -1) perror("Error opening file");
    else {
        fcntl(fd, F_SETFL, 0);
        cout << "Connected to port.";
    }
    
    // wait for arduino reboot
    usleep(3500000);
    
    tcgetattr(fd, &toptions); // get serial port settings
    cfsetispeed(&toptions, B9600); // baud in to 9600
    cfsetospeed(&toptions, B9600); // baud out to 9600
    toptions.c_cflag &= ~PARENB; // no parity
    toptions.c_cflag &= ~CSTOPB; // not stop bit
    toptions.c_cflag &= ~CSIZE; // no word size tag
    toptions.c_cflag |= CS8; // sets word size tag to 8-bit
    toptions.c_lflag |= ICANON; // conical mode (read and write)
    tcsetattr(fd, TCSANOW, &toptions); // commit the settings
    
    while(true) {
    
        cout << "Enter 1 or 0 (10 to quit): ";
        getline(cin, command);
        
        if(command == "10") break;

        // write command to port
        write(fd, buffer, 64);
        
        // read something from port
        n = read(fd, buffer, 64);
        
        // insert terminating 0
        buffer[n] = 0;

    }
    
    
    return 0;
}


