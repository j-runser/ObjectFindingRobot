#include <Braccio.h>
#include <Servo.h>

Servo base;
Servo shoulder;
Servo elbow;
Servo wrist_rot;
Servo wrist_ver;
Servo gripper;

//do not adjust these
int basketM1 = 90;
int basketM2 = 120;
int basketM3 = 35;
int basketM4 = 20;
int permanentM5 = 90;
int closedHand = 50;
int openHand = 10;
int stepdelay = 30;

int M1, M2, M3, M4 = 0;
bool isDone = false;

void setup() {
  Serial.begin(9600);
  Braccio.begin();

  // hanging over strawberry basket 
  Braccio.ServoMovement(stepdelay, basketM1, basketM2, basketM3, basketM4,
      permanentM5, openHand);
}


void loop() {

  if(Serial.available() > 0) {
    Braccio.ServoMovement(stepdelay, basketM1, 130, 15, 45,permanentM5, 30);

    M1 = Serial.parseInt();
    M2 = Serial.parseInt();
    M3 = Serial.parseInt();
    M4 = Serial.parseInt();

    if((M2 + M3 + M4) > 0) {
  
      //if(M3 <= 45){ M3 = M3 + 17; /* 17 */ } 
      //else if(M3 > 45 && M3 <= 135) { M3 = M3 + 12; /* 12 */ } 
      //else if(M3 > 135) { M2 = M2 + 15; /* 15 */ }
    
      //if(M4 <= 135) { M4 = M4 + 10; /* 10 */ }
      //else if(M4 > 135) { M3 = M3 + 5; }

      constrain(M1, 0, 180);
      constrain(M2, 15, 165);
      constrain(M3, 0, 180);
      constrain(M4, 0, 180);
      
      // reaching for object with open hand
      Braccio.ServoMovement(stepdelay, M1, M2, M3, M4, permanentM5, openHand); 
      delay(3000);
      
      // grabbing object
      Braccio.ServoMovement(stepdelay, M1, M2, M3, M4, permanentM5, closedHand); 
      delay(3000);
      
      // returning to strawberry basket with closed hand  
      Braccio.ServoMovement(stepdelay, basketM1, basketM2, basketM3, basketM4,
          permanentM5, closedHand);  
      delay(1000);
      
      // dropping in strawberry 
      Braccio.ServoMovement(stepdelay, basketM1, basketM2, basketM3, basketM4, 
          permanentM5, openHand);  
      delay(1000);

      M1, M2, M3, M4 = 0;

      Braccio.ServoMovement(stepdelay, basketM1, basketM2, basketM3, basketM4,
      permanentM5, openHand);
      delay(500);

      Serial.println(true);
    } else {
      Serial.println(false);
    }

  }

  /*
   Step Delay: a milliseconds delay between the movement of each servo.  Allowed values from 10 to 30 msec.
   M1=base degrees. Allowed values from 0 to 180 degrees
   M2=shoulder degrees. Allowed values from 15 to 165 degrees
   M3=elbow degrees. Allowed values from 0 to 180 degrees
   M4=wrist vertical degrees. Allowed values from 0 to 180 degrees
   M5=wrist rotation degrees. Allowed values from 0 to 180 degrees
   M6=gripper degrees. Allowed values from 10 to 73 degrees. 10: the toungue is open, 73: the gripper is closed.
  */
   
  // exit(0);
}



