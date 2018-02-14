/*
 * Gets the input from xyzToArduino.cpp and sends it to the servos on the Braccio Arm
 * 
 * Author: Joe Runser 12 February 2017
 * 
 * Edited: Joe Runser 15 February 2017
 */

int m1, m2, m3, m4;

bool isDone = false;

void setup() {
  Serial.begin(9600);
}

void loop() {
  if(Serial.available() > 0) {
    
    // reads the servo angles
    m1 = Serial.parseInt();
    m2 = Serial.parseInt();
    m3 = Serial.parseInt();
    m4 = Serial.parseInt();

    isDone = true;

    Serial.println(isDone);
    Serial.println(m1);
    Serial.println(m2);
    Serial.println(m3);
    Serial.println(m4);
  }
}
