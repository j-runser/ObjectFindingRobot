/*
 * Purpose: This software will convert xyz coordinates into servo angles for 
 *          the Arduino Braccio Arm. It will also control the serial connetion
 *          to the arm.
 *
 * Author: Skye VanAtta - 8 February 2017
 *
 * Edited: Joe Runser - 24 March 2017
 * Notes: Converted the C program into a C++ header file.
 *
 * Edited: Skye VanAtta - 24 March 2017
 * Notes: Fixed isses with the actual conversion.
 *
 * Version: 2.1
*/

#include <stdlib.h>
#include <math.h>
#include <string>
#include <SerialStream.h>

#define epsilon 0.5
#define tr 0.01745

using namespace std;
using namespace LibSerial;

class ControlArm {

    public:
        ControlArm();
        ControlArm(string port);
        ControlArm(double x, double y, double z);
        ControlArm(string port, double x, double y, double z);
        
        bool sendServoToArm();
        
        // Setters
        void set_coordinates(double x, double y, double z);
        bool set_servoAngles();
        bool set_servoAngles(double x, double y, double z);

        // Getters
        int get_m1();
        int get_m2();
        int get_m3();
        int get_m4();
    
    private:
        int m1;
        int m2;
        int m3;
        int m4;

        double x;
        double y;
        double z;

        SerialStream arduino;
        string port;

        bool startConnection();
        bool isLegalCoordinates();
        bool yzEquation(double M2, double M3, double M4);
        
};

/*
 * Purpose: Initialize the class ControlArm.
 *
 * Constructor: ControlArm 
 *
 * Author: Joe Runser - 16 February 2017
 *
 * Edited: Joe Runser - 24 March 2017
 */
ControlArm::ControlArm() {

    port = "/dev/armArduino";

    startConnection();

}

/*
 * Purpose: Initialize the class ControlArm and set a different port name.
 *
 * Constructor: ControlArm 
 *
 * Author: Joe Runser - 13 February 2017
 *
 * Edited: Joe Runser - 24 March 2017
 */
ControlArm::ControlArm(string port) {
   
    this->port = port;

    startConnection();
    
}

/*
 * Purpose: Initialize the class ControlArm and set the cartesian coordinates.
 *
 * Constructor: ControlArm 
 *
 * Author: Joe Runser - 13 February 2017
 *
 * Edited: Joe Runser - 24 March 2017
 */
ControlArm::ControlArm(double x, double y, double z) {
   
    this->x = x;
    this->y = y;
    this->z = z;
    port = "/dev/armArduino";

    startConnection();
    
}

/*
 * Purpose: Initialize the class ControlArm, set the cartesian coordinates, and
 *          set a new port.
 *
 * Constructor: ControlArm 
 *
 * Author: Joe Runser - 13 February 2017
 *
 * Edited: Joe Runser - 24 March 2017
 */
ControlArm::ControlArm(string port, double x, double y, double z) {
   
    this->x = x;
    this->y = y;
    this->z = z;
    this->port = port;

    startConnection();
    
}

/*
 * Purpose: Do the convertion of Cartesian Cooridinates to Servo Angles.
 *          Specifically for the Arduino Braccio Robot Arm.
 * 
 * Function: set_servoAngles
 *
 * Author: Skye Vanatta
 *
 * Edited: Joe Runser - 23 March 2017
 * Edited: Skye VanAtta - 23 March 2017
 */
bool ControlArm::set_servoAngles() {

    double tmp_m1;
    double tmp_m2;
    double tmp_m3;
    double tmp_m4;
    
    if(isLegalCoordinates()) {
            
    //calculating M1 from x and y coordinate
    double xDivByy = x / y;
    int n = floor(atan(xDivByy) * 180 / 3.14159 + 90 + 0.5);
    
    if(n >= 0 && n <= 180) {
        tmp_m1 = n;
        m1 = tmp_m1;
        } else {
            m1 = 90;
        }
            
        x = fabs(x);
            
            tmp_m2 = 165.00; //min is 15
            tmp_m3 = 180.00; //min is 0
            tmp_m4 = 180.00; //min is 0
            
            //iteratively calculating M2, M3, and M4 from y and z coordinates
            bool incomplete = true;
            
            while(incomplete){
                if(yzEquation(tmp_m2, tmp_m3, tmp_m4)) {
                    incomplete = false;
                    break;
                }
                //every 22.5 degrees for M4 is a minute or so
                for(double i = 180.0; i >= 0.0; i = i - 0.2) { 
                tmp_m4 = i;
                if(tmp_m4 <= 0.2) { incomplete = false; break;}
                if(!incomplete) { break; }
                if(yzEquation(tmp_m2, tmp_m3, tmp_m4)) {
                    incomplete = false;
                    break;
                }
                for(double j = 180.0; j >= 0.0; j = j - 0.2){
                    tmp_m3 = j;
                    if(!incomplete) { break; }
                    if(yzEquation(tmp_m2, tmp_m3, tmp_m4)) {
                        incomplete = false;
                        break;
                    }
                    for(double k = 15.0; k <= 165.0; k = k + 0.2) {
                        tmp_m2 = k;
                        if(!incomplete) { break; }
                        if(yzEquation(tmp_m2, tmp_m3, tmp_m4)) {
                            m2 = (int)floor(tmp_m2 + 0.5);
                            m3 = (int)floor(tmp_m3 + 0.5);
                            m4 = (int)floor(tmp_m4 + 0.5);
                            incomplete = false;
                            break;
                        } // end if
                    } //  end for k
                } // end for j
            } // end for i
        } // end while
        
        return true;
        
    } // end if
    
    return false;
}

/*
 * Purpose: Do the convertion of Cartesian Cooridinates to Servo Angles.
 *          Specifically for the Arduino Braccio Robot Arm. This will also set
 *          the cartisian coordinates to whatever is inputed.
 * 
 * Function: set_servoAngles
 *
 * Author: Skye Vanatta
 *
 * Edited: Joe Runser - 24 March 2017
 * Edited: Skye VanAtta - 23 March 2017
 */
bool ControlArm::set_servoAngles(double x, double y, double z) {

    this->x = x;
    this->y = y;
    this->z = z;

    double tmp_m1;
    double tmp_m2;
    double tmp_m3;
    double tmp_m4;
    
    if(isLegalCoordinates()) {
            
    //calculating M1 from x and y coordinate
    double xDivByy = x / y;
    int n = floor(atan(xDivByy) * 180 / 3.14159 + 90 + 0.5);
    
    if(n >= 0 && n <= 180) {
        tmp_m1 = n;
        m1 = tmp_m1;
        } else {
            m1 = 90;
        }
            
        x = fabs(x);
            
            tmp_m2 = 165.00; //min is 15
            tmp_m3 = 180.00; //min is 0
            tmp_m4 = 180.00; //min is 0
            
            //iteratively calculating M2, M3, and M4 from y and z coordinates
            bool incomplete = true;
            
            while(incomplete){
                if(yzEquation(tmp_m2, tmp_m3, tmp_m4)) {
                    incomplete = false;
                    break;
                }
                //every 22.5 degrees for M4 is a minute or so
                for(double i = 180.0; i >= 0.0; i = i - 0.2) { 
                tmp_m4 = i;
                if(tmp_m4 <= 0.2) { incomplete = false; break;}
                if(!incomplete) { break; }
                if(yzEquation(tmp_m2, tmp_m3, tmp_m4)) {
                    incomplete = false;
                    break;
                }
                for(double j = 180.0; j >= 0.0; j = j - 0.2){
                    tmp_m3 = j;
                    if(!incomplete) { break; }
                    if(yzEquation(tmp_m2, tmp_m3, tmp_m4)) {
                        incomplete = false;
                        break;
                    }
                    for(double k = 15.0; k <= 165.0; k = k + 0.2) {
                        tmp_m2 = k;
                        if(!incomplete) { break; }
                        if(yzEquation(tmp_m2, tmp_m3, tmp_m4)) {
                            m2 = (int)floor(tmp_m2 + 0.5);
                            m3 = (int)floor(tmp_m3 + 0.5);
                            m4 = (int)floor(tmp_m4 + 0.5);
                            incomplete = false;
                            break;
                        } // end if
                    } //  end for k
                } // end for j
            } // end for i
        } // end while
        
        return true;
        
    } // end if
    
    return false;

}

/*
 * Purpose: Make sure the x, y, and z coordinates can be used on with the Braccio 
 *          Arm.
 *
 * Function: isLeagalCoordinates
 *
 * Author: Skye Vanatta
 *
 * Edited: Joe Runser - 23 March 2017 
 */
bool ControlArm::isLegalCoordinates() {
    
    double XY = sqrt(x * x + y * y);
    if((z >= 0.0) && (XY * XY / 198025 + z * z / 265225) <= 1 
                  && (XY * XY / 15625 + z * z / 52900) >= 1) {
        return true;
    }
    
    return false;

}

/*
 * Purpose: Check the servo angles to see if the location is found.
 *
 * Function: yzEquation
 *
 * Author: Skye Vanatta
 *
 * Edited: Joe Runser - 23 March 2017 
 * Edited: Skye VanAtta - 24 March 2017
*/
bool ControlArm::yzEquation(double M2, double M3, double M4) {

    double XY = sqrt(x * x + y * y);
        
    double solvedXY;
    solvedXY = -125.0 * cos(M2 * tr) + 125.0 
               * fabs(cos((M3 - (90.0 - M2)) * tr)) 
               * sin((M2 + M3 + 180.0) * tr) / fabs(sin((M2 + M3 + 180.0) * tr))
               + 185.0 * fabs(cos((M4 - (90.0 - M3) - (90.0 - M2)) * tr))
               * cos((M2 + M3 + M4) * tr) / fabs(cos((M2 + M3 + M4) * tr));
        
        
    double solvedZ;
    solvedZ = 125.0 * sin(M2 * tr) + 125.0 
              * fabs((sin((M3 - (90.0 - M2)) * tr))) 
              * sin((M2 + M3 - 90.0) * tr) / fabs(sin((M2 + M3 - 90.0) * tr))
              + 185.0 * fabs(sin((M4 - (90.0 - M3) - (90.0 - M2)) * tr)) 
              * sin((M2 + M3 + M4 + 180.0) * tr) / fabs(sin((M2 + M3 + M4 + 180.0) * tr)) 
              + 70.0;

    if(((fabs(solvedXY - XY)) < epsilon) && ((fabs(solvedZ - z)) < epsilon)){
        return true;
    }
    
    return false;
}

/*
 * Purpose: Get m1.
 *
 * Function: get_m1
 *
 * Author: Joe Runser - 13 February 2017
 */
int ControlArm::get_m1() {

    return m1;

}

/*
 * Purpose: Get m2.
 *
 * Function: get_m2
 *
 * Author: Joe Runser - 13 February 2017
 */

int ControlArm::get_m2() {

    return m2;

}

/*
 * Purpose: Get m3.
 *
 * Function: get_m3
 *
 * Author: Joe Runser - 13 February 2017
 */
int ControlArm::get_m3() {

    return m3;

}

/*
 * Purpose: Get m4.
 *
 * Function: get_m4
 *
 * Author: Joe Runser - 13 February 2017
 */
int ControlArm::get_m4() {

    return m4;

}

/*
 * Purpose: Set the cartesian coordinates.
 *
 * Function: set_coordinates
 *
 * Author: Joe Runser - 16 February 2017
 */
void ControlArm::set_coordinates(double x, double y, double z) {

    this->x = x;
    this->y = y;
    this->z = z;
    
}

/*
 * Purpose: Send the servo angles to the arm. Caution, do not run this without 
 *          setting the servo angles first.
 *
 * Constructor: sendServoToArm 
 *
 * Author: Joe Runser - 24 March 2017
 *
 * Edited: Joe Runser - 24 March 2017
 */
bool ControlArm::sendServoToArm() {

    bool doneStatus = false;

    if(arduino.IsOpen()) {
        arduino << m1 << endl;
        arduino << m2 << endl;
        arduino << m3 << endl;
        arduino << m4 << endl;

        arduino >> doneStatus;

        return doneStatus;
    }

    return doneStatus;
    
}

/*
 * Purpose: Start the serial connection to the arduino. Caution, do not run this
 *          without setting the servo angles first.
 *
 * Constructor: sendServoToArm 
 *
 * Author: Joe Runser - 24 March 2017
 *
 * Edited: Joe Runser - 24 March 2017
 */
bool ControlArm::startConnection() {

    arduino.Open(port);
    arduino.SetBaudRate(SerialStreamBuf::BAUD_9600);

}