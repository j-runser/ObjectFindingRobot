#ifndef CartesianToServo_h
#define CartesianToServo_h

/*
 * Purpose: Creates the Cartesian coordinates for the Arduino Braccio Arm.
 */
void ConvertXYZToServo(int* servoM1, int* servoM2, int* servoM3, int* servoM4);

/*
 * Purpose: Set the xyz coordinates for the converstion. This must be called before
 * the ConvertXYZToServo is called.
 */
void  SetXYZ(int local_x, int local_y, int local_z);

#endif
