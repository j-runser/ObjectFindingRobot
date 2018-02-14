/* 
 *  Works together with an external input to add two integers and 
 *  return their sum.
 *    
 *  Created: 27 January 2017
 *  By: Joe Runser
 *   
 *  Modified: 27 January 2017
 *  By: Joe Runser
 */

int numOne;
int numTwo;
int sum;

void setup() {
  Serial.begin(9600);
}

void loop() {
  if(Serial.available() > 0) {
    
    // reads the incoming data
    numOne = Serial.parseInt();
    numTwo = Serial.parseInt();

    sum = numOne + numTwo;

    Serial.println(sum);
  }
}
