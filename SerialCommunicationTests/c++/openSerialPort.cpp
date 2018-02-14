#include <iostream>
#include <fcntl.h>  /* File Control Definitions          */
#include <termios.h>/* POSIX Terminal Control Definitions*/
#include <unistd.h> /* UNIX Standard Definitions         */
#include <errno.h>  /* ERROR Number Definitions          */

using namespace std;

int main() {

    int fd;

    fd = open("/dev/ttyACM0",O_RDWR | O_NOCTTY);

    if (fd == 1) { cout << "Error in Opening ttyACM0" << endl; }
    else { cout << "ttyACM0 Opened Successfully" << endl; }

    close(fd);
    return 0;
}
