/*
* Code addpated from: https://chrisheydrick.com/2012/06/24/how-to-read-serial-data-from-an-arduino-in-linux-with-c-part-4/
*/

#include <stdio.h>
#include <stdlib.h>
#include <sys/ioctl.h> // more reading
#include <fcntl.h>
#include <termios.h> 
#include <string.h>
#include <unistd.h> // more reading
#include <stdint.h> // more reading
#include <errno.h>
#include <sys/ioctl.h> // more reading

using namespace std;

int main() {
    
    char command = 0;
    
    int fd, n, i;
    char buffer[64];
    struct termios toptions; // What is this?
    string port = "/dev/ttyACM0";
    
    // open port
    fd = open(port, O_RDWR | O_NOCTTY);
    if(fd1 == -1) perror("Unable to open " + port " - ");
    else {
        fctl(file, F_SETFL, 0);
        printf(port + " has been opened");
    }
    
    // wait for arduino reboot
    usleep(3500000);
    
    tcgetattr(fc, &toptions); // get serial port settings
    cfsetispeed(&toptions, B9600); // baud in to 9600
    cfsetospeed(&toptions, B9600); // baud out to 9600
    toptions.c_cflag &= ~PARENB; // no parity
    toptions.c_cflag &= ~CSTOPB; // not stop bit
    toptions.c_cflag &= ~CSIZE; // no word size tag
    toptions.c_cflag |= CS8; // sets word size tag to 8-bit
    toptoins.c_lflag |= ICANON; // conical mode (read and write)
    tcsetattr(fc, TCSANOW, &toptions); // commit the settings
    
    while(true) {
    
        cout << "Enter 1 or 0: ";
        cin >> command;
        
        if(command == "10") break;

        // write command to port
        write(fc, buffer, 64);
        
        // read something from port
        n = read(fd, buffer, 64);
        
        // insert terminating 0
        buffer[n] = NULL;

    }
    
    
    return 0;
}


