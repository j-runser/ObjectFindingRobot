/*
 *  Will allow the sensors to interupt movement and will cause the
 *  robot to move as nessasary.
 *  
 */

#include <SharpIR.h>
#include <math.h>


// Define the name and port of the sensor.
#define sensor_front A0
#define sensor_left  A1
#define sensor_right A2
#define sensor_back  A3
// The model of distance sensor that we are using.
#define model 20150

static int NUM_AVERAGE = 25;
static int VALID_READ  = 55;
static int MODEL_NUM   = 20150;

// Declare each instance of SharpIR corresponding with each sensor
SharpIR sharp_front(sensor_front, NUM_AVERAGE, VALID_READ, MODEL_NUM);
SharpIR sharp_left( sensor_left,  NUM_AVERAGE, VALID_READ, MODEL_NUM);
SharpIR sharp_right(sensor_right, NUM_AVERAGE, VALID_READ, MODEL_NUM);
SharpIR sharp_back( sensor_back,  NUM_AVERAGE, VALID_READ, MODEL_NUM);

void setup() {

  Serial.begin(9600);
  pinMode(sensor_front, INPUT);

}

void loop() {
  
  int dis_front = sharp_front.distance();
  int dis_left  = sharp_left.distance();
  int dis_right = sharp_right.distance();
  int dis_back  = sharp_back.distance();

  constrain(dis_front,20,150);
  constrain(dis_right,20,150);

  
  int exR = dis_right - 20;
  int exF = dis_front - 20;
  int rH = sqrt(exR*exR + 961);
  int trH = sqrt(256 + exF*exF);
  int wH = sqrt((dis_front + 11)*(dis_front + 11) + (dis_right - 4)*(dis_right - 4));
  
  if(wH <= (rH + trH)) {
    // immidiatly stop and figure out the next move.
    Serial.println("stop");
  }
  
  Serial.print("front sensor reading: ");
  Serial.println(dis_front);

  Serial.print("right sensor reading: ");
  Serial.println(dis_right);

  delay(1500);

}