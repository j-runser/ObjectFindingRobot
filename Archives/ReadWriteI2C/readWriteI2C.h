/*
 * Purpose: This software will build the I2C connections to the arduino devices
 *          if the proper instructions are given.
 *
 * Author: Joe Runser - 8 March 2017
 *
 * Edited: Joe Runser - 8 March 2017
 *
 * Version: 1.0
*/

#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/i2c.h>
#include <linux/i2c-dev.h>

using namespace std;

class ReadWriteI2C {

    public:
        ReadWriteI2C();
        ReadWriteI2C(unsigned char deviceAddress, char *filename);
        ~ReadWriteI2C();
        
        void writeReg(unsigned char regAddress, unsigned char data);
        void readReg(unsigned char regAddress, unsigned char &data);

    private:
        unsigned char deviceAddress;
        char *filename;
        int descriptor;

        void openI2C();
        void closeI2C();

};


/*
 * Purpose: Instantiate ReadWriteI2c.
 *
 * Constructor: ReadWriteI2C
 *
 * Author: Joe Runser - 8 March 2017
 *
 * Edited: Joe Runser - 8 March 2017
 */
ReadWriteI2C::ReadWriteI2C() {

    filename = (char*)"/dev/i2c-0";
    deviceAddress = 0;
    descriptor = -1;

    openI2C();

}

/*
 * Purpose: Instantiate ReadWriteI2c.
 *
 * Constructor: ReadWriteI2C
 *
 * Author: Joe Runser - 8 March 2017
 *
 * Edited: Joe Runser - 8 March 2017
 */
ReadWriteI2C::ReadWriteI2C(unsigned char deviceAddress, char* filename) {

    this->filename = filename;
    this->deviceAddress = deviceAddress;
    descriptor = -1;

    openI2C();

}

/*
 * Purpose: Destructor for ReadWriteI2C.
 *
 * Destructor: ~ReadWriteI2C 
 *
 * Author: Joe Runser - 8 March 2017
 *
 * Edited: Joe Runser - 8 March 2017
*/
ReadWriteI2C::~ReadWriteI2C() {

    closeI2C();

}

/*
 * Purpose: Open the filestream for an I2C connection.
 *
 * Method: openI2C
 *
 * Author: Joe Runser - 8 March 2017
 *
 * Edited: Joe Runser - 8 March 2017
*/
void ReadWriteI2C::openI2C() {

    descriptor = open(filename, O_RDWR);

    if(descriptor < 0) {
        perror("Could not open file (1)");
        exit(1);
    }

}

/*
 * Purpose: Close the filestream for an I2C connection.
 *
 * Method: closeI2C
 *
 * Author: Joe Runser - 8 March 2017
 *
 * Edited: Joe Runser - 8 March 2017
*/
void ReadWriteI2C::closeI2C() {

    int retVal = -1;
    retVal = close(descriptor);

    if(retVal < 0) {
        perror("Could not close file (1)");
        exit(1);
    }

}

/*
 * Purpose: Write bytes of data to an I2C device. 
 *
 * Method: writeReg
 *
 * Author: Joe Runser - 8 March 2017
 *
 * Edited: Joe Runser - 8 March 2017
*/
void ReadWriteI2C::writeReg(unsigned char regAddress, unsigned char data) {

    unsigned char buff[2];
    int retVal = -1;
    struct i2c_rdwr_ioctl_data packets;
    struct i2c_msg messages[1];

    buff[0] = regAddress;
    buff[1] = data;
    
    messages[0].addr = deviceAddress;
    messages[0].flags = 0;
    messages[0].len = sizeof(buff);
    messages[0].buf = buff;

    packets.msgs = messages;
    packets.nmsgs = 1;
    
    retVal = ioctl(descriptor, I2C_RDWR, &packets);
    if(retVal < 0) { perror("Write to I2C Device failed."); }

}

/*
 * Purpose: Read bytes of data from an I2C device. 
 *
 * Method: readReg
 *
 * Author: Joe Runser - 8 March 2017
 *
 * Edited: Joe Runser - 8 March 2017
*/
void ReadWriteI2C::readReg(unsigned char regAddress, unsigned char &data) {

    unsigned char *inbuff;
    unsigned char outbuff;
    int retVal = -1;
    struct i2c_rdwr_ioctl_data packets;
    struct i2c_msg messages[2];

    outbuff = regAddress;
    messages[0].addr = deviceAddress;
    messages[0].flags = 0;
    messages[0].len = sizeof(outbuff);
    messages[0].buf = &outbuff;

    inbuff = &data;
    messages[1].addr = deviceAddress;
    messages[1].flags = I2C_M_RD;
    messages[1].len = sizeof(*inbuff);
    messages[1].buf = inbuff;

    packets.msgs = messages;
    packets.nmsgs = 2;

    retVal = ioctl(descriptor, I2C_RDWR, &packets);
    if(retVal < 0) { perror("Read from I2C Device failed."); }

}
