/* 
 * Purpose: This will hold all of the required statup functoins for the
 *          object finding robot.
 * 
 * Author: Joe Runser - 16 February 2017
 * 
 * Edited: Joe Runser - 16 March 2017
 *
 * Version: 0.2 - untested
 */

#include <stdlib.h>
#include <SerialStream.h>
// #include <CartesianToServo.h>

class SubsystemControls {

    public:
        SubsystemControls();

        bool InitializeBraccio();
	    bool InitializeWheels();
	    bool InitializeAlgorithm();

    private:
        SerialStream arm;
	    SerialStream wheels;
        CartesianToServo converter();

};

/*
 * Purpose: Starts all of the subsystems in the object finding robot.
 * 
 * Author: Joe Runser - 16 February 2017
 *
 * Edited: Joe Runser - 16 March 2017
 */
SubsystemControls::SubsystemControls() {
    
    InitializeBraccio();
    InitializeWheels();
    InitializeAlgorithm();
}

/*
 * Puropose: Start the Braccio robotic arm.
 *
 * Return: Will return true if the arm is ready to go.
 * 
 * Author: Joe Runser - 16 February 2017
 *
 * Edited: Joe Rusner - 16 March 2017
 */
bool SubsystemControls::InitializeBraccio() {

    bool isReady = false;

    // Set up serial connection.
    arm.Open("/dev/arduinoBraccio");
    arm.SetBaudRate(SerialStreamBuf::BAUD_9600);
    
    // Convert the coordinates to servo angles.
    converter.set_coordinates(100, 300, 170);

    // Send the servo angles to the arm and get back a confirmation.
    if(arm.IsOpen()) {
        arm << converter.get_m1() << endl;
        arm << converter.get_m2() << endl;
        arm << converter.get_m3() << endl;
        arm << converter.get_m4() << endl;

        arm >> isReady;
    } else { isReady = false; }

    return isReady;
}

/*
 * Puropose: Initialize the wheels arduino.
 *
 * Return: Will return true if the wheels initilize.
 * 
 * Author: Joe Runser - 16 February 2017
 *
 * Edited: Joe Rusner - 16 March 2017
 */
SubsystemControls::InitializeWheels() {

    bool isReady = false;

    wheels.Open("/dev/arduinoWheels");
    wheels.SetBaudRate(SerialStreamBuf::BAUD_9600);

    if(wheels.IsOpen()) { isReady = true; }
    else { isReady = false; }

    return isReady;

}

/*
 * Puropose: Start the deep learning search algorithm.
 *
 * Return: Will return true if the algorithm starts and false otherwise.
 * 
 * Author: Joe Runser - 16 February 2017
 *
 * Edited: Joe Rusner - 16 March 2017
 */
SybsystemControls::InitializeAlgorithm() {

    // start the searching algorithm

}

/*
 * Puropose: Shutdown the Braccio arm.
 *
 * Return: Will return true if the arm is shut down.
 * 
 * Author: Joe Runser - 16 February 2017
 *
 * Edited: Joe Rusner - 16 February 2017
 */
bool SubsystemControls::ShutdownBraccio() {

    bool isShutdown = false;

    

}