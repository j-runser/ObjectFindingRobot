/*
 * Purpose: This software will convert xyz coordinates into servo angles for 
 *          the Arduino Braccio Arm.
 *
 * Author: Skye VanAtta - 8 February 2017
 *
 * Edited: Joe Runser - 16 February 2017
 * Edited: Skye VanAtta - 13 February 2017
 *
 * Version: 1.3
*/

#include <stdlib.h>
#include <math.h>

using namespace std;

class CartesianToServo {

    public:
        CartesianToServo();
        CartesianToServo(int x, int y, int z);

        void set_coordinates(int x, int y, int z);

        int get_m1();
        int get_m2();
        int get_m3();
        int get_m4();
    
    private:
        int m1;
        int m2;
        int m3;
        int m4;

        int x;
        int y;
        int z;

        void cartesianToServo();
        bool isLegalCoordinates();
        bool yzEquation();
};

/*
 * Purpose: Initialize the class CartesianToServo.
 *
 * Constructor: CartesianToServo 
 *
 * Author: Joe Runser - 16 February 2017
 *
 * Edited: Joe Runser - 16 February 2017
 */
CartesianToServo::CartesianToServo() {

}

/*
 * Purpose: Initialize the class CartesianToServo and using the input convert the
 *          Cartesian Coordinates to Servo angles. Those Servo Angles can then be
 *          called using thier respective get methods.
 *
 * Constructor: CartesianToServo 
 *
 * Author: Joe Runser - 13 February 2017
 *
 * Edited: Joe Runser - 13 February 2017
 */
CartesianToServo::CartesianToServo(int x, int y, int z) {
   
    this->x = x;
    this->y = y;
    this->z = z;

    cartesianToServo();

}

/*
 * Purpose: Do the convertion of Cartesian Cooridinates to Servo Angles.
 *          Specifically for the Arduino Braccio Robot Arm.
 * 
 * Function: cartesianToServo
 *
 * Author: Skye Vanatta
 *
 * Edited: Joe Runser - 13 February 2017
 */
void CartesianToServo::cartesianToServo() {

    int final_m2;
    int final_m3;
    int final_m4;

    if(isLegalCoordinates()) {
       
        // Calculate m1 from x and y
        double tmpVar = (double) x / y;
        m1 = floor(atan(tmpVar) * 180 / M_PI + 90 + 0.5);

        m2 = 15; //min is 15
        m3 = 0; //min is 0
        m4 = 0; //min is 0
        
        // Measues when the below loop is complete.
        bool notComplete = true;

        //iteratively calculating m2, m3, and M4 from y and z coordinates
        while(notComplete) {
            
            // If this is the case then m2, m3, and m4 are good.
            if(yzEquation()) { notComplete = false; break; }

            for(int i = 0; i <= 180; i++) { // for loop i
                m4 = i;
                if(yzEquation()) { notComplete = false; break; }

                for(int j = 0; j <= 180; j++) { // for loop j
                    m3 = j;
                    if(yzEquation()) { notComplete = false; break; }
                    
                    for(int k = 15; k <= 165; k++) { // for look k
                        m2 = k;
                        if(yzEquation()) { 
                            final_m2 = m2;
                            final_m3 = m3;
                            final_m4 = m4;

                            notComplete = false;
                            break; 
                        }
                    } // end  k
                } // end j
            } // end i
        } // end while
    } // if

    m2 = final_m2;
    m3 = final_m3;
    m4 = final_m4;

}

/*
 * Purpose: Make sure the x, y, and z coordinates can be used on with the Braccio 
 *          Arm.
 *
 * Function: isLeagalCoordinates
 *
 * Author: Skye Vanatta
 *
 * Edited: Joe Runser - 13 February 2017 
*/
bool CartesianToServo::isLegalCoordinates() {
    
    if((y > 60) && (abs(x) > 60) && (z >= 0) 
		    && ((y * y / 198025 + z * z / 265225) <= 1.0) 
		    && (abs(x) <= 445)) { return true; }
    else { return false; }

}

/*
 * Purpose: Check the servo angles to see if the location is found.
 *
 * Function: yzEquation
 *
 * Author: Skye Vanatta
 *
 * Edited: Joe Runser - 13 February 2017 
 * Edited: Skye Vanatta - 13 February 2017
*/
bool CartesianToServo::yzEquation() {

    static double TR = M_PI / 180; //degrees to radians

    // Checks for y being solved
    double solvedY = -125 * cos(m2 * TR)
            + 125 * fabs(cos((m3 + fabs(90 - m2)) * TR)) 
			* sin((m2 + m3 + 180) * TR) / fabs(sin((m2 + m3 + 180) * TR))
            + 185 * fabs(cos((m4 + fabs(90 - m3) + fabs(90 - m2)) * TR)) 
			* cos((m2 + m3 + m4) * TR) / fabs(cos((m2 + m3 + m4) * TR));
    
    // Checks for z being sovled
    double solvedZ = 125 * sin(m2 * TR)
            + 125 * fabs((sin((m3 + fabs(90 - m2)) * TR))) 
			* sin((m2 + m3 - 90) * TR) / fabs(sin((m2 + m3 - 90) * TR))
            + 185 * fabs(sin((m4 + fabs(90 - m3) + fabs(90 - m2)) * TR)) 
			* sin((m2 + m3 + m4 + 180) * TR) / fabs(sin((m2 + m3 + m4 + 180) * TR));

    //cout << "SolvedY = " << floor(solvedY) << " and y = " << y << endl;
    //cout << "SolvedZ = " << floor(solvedZ) << " and z = " << z << endl;

    if((floor(solvedY) == y) && (floor(solvedZ) == z)) { return true; }
    else { return false; }
}

/*
 * Purpose: Get m1.
 *
 * Function: get_m1
 *
 * Author: Joe Runser - 13 February 2017
 */
int CartesianToServo::get_m1() {

    return m1;

}

/*
 * Purpose: Get m2.
 *
 * Function: get_m2
 *
 * Author: Joe Runser - 13 February 2017
 */

int CartesianToServo::get_m2() {

    return m2;

}

/*
 * Purpose: Get m3.
 *
 * Function: get_m3
 *
 * Author: Joe Runser - 13 February 2017
 */
int CartesianToServo::get_m3() {

    return m3;

}

/*
 * Purpose: Get m4.
 *
 * Function: get_m4
 *
 * Author: Joe Runser - 13 February 2017
 */
int CartesianToServo::get_m4() {

    return m4;

}

/*
 * Purpose: Set the cartesian coordinates.
 *
 * Function: set_coordinates
 *
 * Author: Joe Runser - 16 February 2017
 */
void CartesianToServo::set_coordinates(int x, int y, int z) {

    this->x = x;
    this->y = y;
    this->z = z;

    cartesianToServo();
}
