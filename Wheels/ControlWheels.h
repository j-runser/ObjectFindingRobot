/*
 * Purpose: Connect to the wheelsArduino, then send it specific user commands as
 *          to control the movement of the robot.
 *
 * Author: Joe Runser - 1 March 2017
 *
 * Edited: Joe Runser - 24 March 2017
 *
 * Version: 1.0
*/

#include <SerialStream.h>
#include <string>

#define W_STOP 0
#define W_FORWARD 1
#define W_BACKWARD 2
#define W_RIGHT 3
#define W_LEFT 4
#define W_START 5

using namespace std;
using namespace LibSerial;

class ControlWheels {

    public:
        ControlWheels();
        ControlWheels(string port);

        bool sendCommand(int command);

    private:
        SerialStream arduino;

        string port;

        void startConnection();
        void systemCheck();

};

/*
 * Purpose: Initialize the class ControlWheels.
 *
 * Constructor: ControlWheels, it will perform a systems check then it will be 
 *              put in a wait condition for further commands.
 *
 * Author: Joe Runser - 1 March 2017
 *
 * Edited: Joe Runser - 24 March 2017
*/
ControlWheels::ControlWheels() {

    port = "/dev/wheelsArduino";

    startConnection();

}

/*
 * Purpose: Initialize the class ControlWheels.
 *
 * Constructor: ControlWheels, it will perform a systems check then it will be 
 *              put in a wait condition for further commands.
 *
 * Author: Joe Runser - 1 March 2017
 *
 * Edited: Joe Runser - 24 March 2017
*/
ControlWheels::ControlWheels(string port) {

    this->port = port;

    startConnection();

}

/*
 * Purpose: Start the connection stream with the wheelsArduino.
 *
 * Function: startConnection
 *
 * Author: Joe Runser - 1 March 2017
 *
 * Edited: Joe Runser - 22 March 2017
*/
void ControlWheels::startConnection() {

    arduino.Open(port);
    arduino.SetBaudRate(SerialStreamBuf::BAUD_9600);

}

/*
 * Purpose: Do a system check on the wheelsArduino as to make sure the system is
 *          working properly.
 *
 * Note: The arduino will have the sister class in the initialization.
 *
 * Function: systemCheck
 *
 * Author: Joe Runser - 1 March 2017
 *
 * Edited: Joe Runser - 20 March 2017
*/
void ControlWheels::systemCheck() {

}

/*
 * Purpose: Send the wheels a command.
 *
 * Function: sendCommand
 *
 * Author: Joe Runser - 20 March 2017
 *
 * Edited: Joe Runser - 24 March 2017
*/
bool ControlWheels::sendCommand(int command) {

    if(arduino.IsOpen()) {
        arduino << command;
        return true;
    }
    
    return false;

}
