//pins for right motor
const int ledPin13 = 13;
const int ledPin12 = 12;
const int ledPin11 = 11;
const int ledPin10 = 10;

//pins for left motor
const int ledPin9  = 9;
const int ledPin8  = 8;
const int ledPin7  = 7;
const int ledPin6  = 6;

//tells which sensor is in which port on arduino
#define sensor_front A0
#define sensor_left  A1
#define sensor_back  A2
#define sensor_right A3

// The current command for the wheel motors.
int currentCommand;

//distance to walls on each side of robot
float dis_front;
float dis_left;
float dis_right;
float dis_back;

//corner distance variables
bool upRight;
bool upLeft;
bool backLeft;
bool backRight;

//direction codes to compare with directions from jetson; used stopp because stop is a used variable
const int forward = 0;
const int stopp   = 1;
const int left    = 2;
const int right   = 3;
const int reverse = 4;
const int search  = 5;

//Minimum distance bot can be from a wall or object
const int min_dis = 25;

//speeds the motor can go (255=max_speed and 0=Off)
const int maxSpeed = 255;
const int minSpeed = 0;

//used to measure time of something working
//Ex: finding amount of time the robot has turned left and then if it needs to return back to starting point it can use elapsed time to turn right
unsigned long start,finished,elapsed;

int jetDirect = 2;

void setup() {
  Serial.begin(9600);

  pinMode(LED_BUILTIN, OUTPUT);

  pinMode(ledPin13, OUTPUT);
  pinMode(ledPin12, OUTPUT);
  pinMode(ledPin11, OUTPUT);
  pinMode(ledPin10, OUTPUT);
  pinMode(ledPin9 , OUTPUT);
  pinMode(ledPin8 , OUTPUT);
  pinMode(ledPin7 , OUTPUT);
  pinMode(ledPin6 , OUTPUT);
}

void rightMotorForward(){
  digitalWrite(ledPin12, HIGH);
  digitalWrite(ledPin13, LOW);
  analogWrite(ledPin10 , minSpeed);
  analogWrite(ledPin11 , 125);
}

void leftMotorForward(){
  digitalWrite(ledPin7, HIGH);
  digitalWrite(ledPin8, LOW);
  analogWrite(ledPin9 , minSpeed);
  analogWrite(ledPin6 , 125);
}

void rightMotorReverse(){
  digitalWrite(ledPin12, LOW);
  analogWrite(ledPin11 , minSpeed);
  digitalWrite(ledPin13,HIGH);
  analogWrite(ledPin10 , 125);
}

void leftMotorReverse() {
  digitalWrite(ledPin7, LOW);
  analogWrite(ledPin6 , minSpeed);
  digitalWrite(ledPin8, HIGH);
  analogWrite(ledPin9 , 125);
}

//robot will go forward
void Forward() {
  rightMotorForward();
  leftMotorForward();
}

//robot will go in reverse
void Reverse(){
  rightMotorReverse();
  leftMotorReverse();
}

//robot will turn left
void Left(){
  leftMotorReverse();
  rightMotorForward();
}

//robot will turn right
void Right(){
  rightMotorReverse();
  leftMotorForward();
}

//robot will stop both motors
void Stop(){
  digitalWrite(ledPin12, LOW);
  analogWrite(ledPin11 , minSpeed);
  digitalWrite(ledPin13,LOW);
  analogWrite(ledPin10 , minSpeed);

  digitalWrite(ledPin7, LOW);
  analogWrite(ledPin6 , minSpeed);
  digitalWrite(ledPin8, LOW);
  analogWrite(ledPin9 , minSpeed);

  delay(1000);
}

//finds the distance in centimeters from sensors
float sensorF(){
  float cm;
  float sensorValue;
  
  sensorValue = analogRead(sensor_front);
  cm = 10650.08 * pow(sensorValue,-0.935) - 10;
  return cm;
}

float sensorB(){
  float cm;
  float sensorValue;
  
  sensorValue = analogRead(sensor_back);
  cm = 10650.08 * pow(sensorValue,-0.935) - 10;
  return cm;
}

float sensorR(){
  float cm;
  float sensorValue;
  
  sensorValue = analogRead(sensor_right);
  cm = 10650.08 * pow(sensorValue,-0.935) - 10;
  return cm;
}

float sensorL(){
  float cm;
  float sensorValue;
  
  sensorValue = analogRead(sensor_left);
  cm = 10650.08 * pow(sensorValue,-0.935) - 10;
  return cm;
}

//finds how far corner of box is to hitting something
bool cornerSense(float side, float adjacent){
  int exR = adjacent - 20;
  int exF = side - 20;
  int rH  = sqrt(exR*exR + 961);
  int trH = sqrt(256 + exF*exF);
  int wH  = sqrt((side + 11)*(side + 11) + (adjacent - 4)*(adjacent - 4));
  
  if(wH <= (rH + trH)) {
    return true;
  }
  else{
    return false;
  }
}

//reads in directions from jetson board
int directions(){
  jetDirect = Serial.parseInt();
  return jetDirect;
}

//updates all sensors
void updateSensors(){
  dis_front = sensorF();
  dis_left  = sensorL();
  dis_right = sensorR();
  //dis_back  = sensorB();
  dis_back  = 200;
}

//updates all corner distances
void updateCorners(){
  //upRight   = cornerSense(dis_front, dis_right);
  //upLeft    = cornerSense(dis_front, dis_left);
  //backLeft  = cornerSense(dis_back , dis_left);
  //backRight = cornerSense(dis_back , dis_right);

  upRight   =false;
  upLeft    = false;
  backLeft  = false;
  backRight = false;
}

void frontAvoid(){
updateSensors();
updateCorners();

	if(upLeft==false && upRight==false && dis_front>=min_dis){
		//nothing in way
		while(upLeft==false && upRight==false && dis_front>=min_dis){
			Forward();

			updateCorners();
			updateSensors();

			if(currentCommand == stopp){
				Stop();
				break;
			}
		}

		//something at upper left corner
		if(upLeft==true && upRight==false){
			Stop();
			start=millis();
				  
			while(upLeft==true){
				Right();
					
				updateCorners();
				updateSensors();

				if(currentCommand==stopp){
					Stop();
					break;
				}
			}
				  
			finished=millis();
			Stop();
			elapsed=finished-start;
			updateSensors();

			while(dis_left<35){
			Forward();

			updateSensors();

				if(currentCommand==stopp){
				  Stop();
				  break;
				}
			}

			Stop();
			updateSensors();
			updateCorners();

			if(dis_left>=35){
				Left();
				delay(elapsed);
			}
			//object (hopefully) avoided      
			}
		//something at upper right corner
		else if(upLeft==false && upRight==true){
				Stop();
				start=millis();

			while(upRight==true){
				Left();

				updateCorners();
				updateSensors();

				if(currentCommand==stopp){
				  Stop();
				  break;
				}
			}

			finished=millis();
			Stop();
			elapsed=finished-start;
			updateSensors();

			while(dis_right<35){
				Forward();

				updateSensors();

				if(currentCommand==stopp){
				  Stop();
				  break;
				}
			}

			Stop();
			updateSensors();
			updateCorners();

			if(dis_right>=35){
				Right();
				delay(elapsed);
			}
			//object (hopefully) avoided 
		}
	}
}

void backAvoid(){
updateSensors();
updateCorners();

	if(backLeft==false && backRight==false && dis_back>=min_dis){

		//nothing in way
		while(backLeft==false && backRight==false && dis_back>=min_dis){
			Reverse();

			updateCorners();
			updateSensors();

			if(currentCommand==stopp){
				Stop();
				break;
			}
		}

		//something at back left corner
		if(backLeft==true && backRight==false){
			Stop();
			start=millis();
				  
			while(backLeft==true){
				Right();
					
				updateCorners();
				updateSensors();

				if(currentCommand==stopp){
					Stop();
					break;
				}
			}
				  
			finished=millis();
			Stop();
			elapsed=finished-start;
			updateSensors();

			while(dis_left<35){
				Reverse();

				updateSensors();

				if(currentCommand==stopp){
				  Stop();
				  break;
				}
			}

			Stop();
			updateSensors();
			updateCorners();

			if(dis_left>=35){
				Left();
				delay(elapsed);
			}
			//object (hopefully) avoided      
			}
		//something at back right corner
		else if(backLeft==false && backRight==true){
				Stop();
				start=millis();

			while(backRight==true){
				Left();

				updateCorners();
				updateSensors();

				if(currentCommand==stopp){
				  Stop();
				  break;
				}
			}

			finished=millis();
			Stop();
			elapsed=finished-start;
			updateSensors();

			while(dis_right<35){
				Reverse();

				updateSensors();

				if(currentCommand==stopp){
				  Stop();
				  break;
				}
			}

			Stop();
			updateSensors();
			updateCorners();

			if(dis_right>=35){
				Right();
				delay(elapsed);
			}
			//object (hopefully) avoided 
		}
	}
}

void loop() {

  if(Serial.available() > 0) {
    digitalWrite(LED_BUILTIN, HIGH);
    currentCommand = Serial.parseInt();
    Serial.println("Connected");
    delay(3000);
  }

  digitalWrite(LED_BUILTIN, LOW);
  
	//keeps track of if the arduino was able to enter left turn search and right turn search
	int leftSuccess  = 2;
	int rightSuccess = 3;
	
  //finding distance for each sensor
  updateSensors();
  updateCorners();
  
  //keeps the sensor data from going above or below 20cm-150cm range
  constrain(dis_front, 20, 150);
  constrain(dis_right, 20, 150);
  constrain(dis_left,  20, 150);
  constrain(dis_back,  20, 150);
  
  //searches for object by turning left
  if(currentCommand == left && leftSuccess == left && backLeft == false && upLeft == false && dis_left >= min_dis) {
    while(leftSuccess==left && backLeft==false && upLeft==false && dis_left>=min_dis){
	  start=millis();
      Left();

      //constantly updating while spinning left
      updateCorners();
      updateSensors();
      
      if(currentCommand!=left || backLeft==true || upLeft==true || dis_left<min_dis){
        Stop();
		updateCorners();
        updateSensors();
		
        //if something of interest in camera it moves forward with course alterations if needed
        if(currentCommand==forward){
		      Stop();
          frontAvoid();
        }
		else if(backLeft==true && upLeft==true){
			finished=millis();
			elapsed=finished=millis();
			Right();
			leftSuccess=0;
			delay(elapsed);
			//send signal saying go right
			break;
		}
      }
    }
  }
  
  updateCorners();
  updateSensors();
  
  //searches for object by turning right
  if((rightSuccess==right && backRight==false && upRight==false && dis_right>=min_dis) || (leftSuccess==0)||(currentCommand==right)){
    while(rightSuccess==right && backRight==false && upRight==false && dis_right>=min_dis){
      Right();
  
      //constantly updating while spinning right
      updateCorners();
      updateSensors();
      
      if(currentCommand!=right || backRight==true || upRight==true || dis_right<min_dis){
        Stop();
  
        //if something of interest in camera it moves forward with course alterations if needed
        if(currentCommand==forward){
		  Stop();
          frontAvoid();
        }
		
		//robot is going to turn into wall
		else if(backRight==true && upRight==true){
			finished=millis();
			elapsed=finished=millis();
			Left();
			delay(elapsed);
			rightSuccess = 0;
			break;
		}
      }
    }
  }
  
  if(rightSuccess==0 && leftSuccess==0){
	  start=millis();
	  while(true){
		  
		  frontAvoid();
		  finished=millis();
		  elapsed=finished-start;
		  
		  if(elapsed>=5000){
			  leftSuccess  = 2;
			  rightSuccess = 3;
			  break;
		  }
	  }
  }

  updateCorners();
  updateSensors();
  
  //walls on both sides of robot and cant turn
  if((rightSuccess == right || leftSuccess == left) && dis_right < min_dis && dis_left < min_dis){
	  start=millis();
	  if(dis_back<min_dis){
		  while(dis_front>=min_dis){
			  frontAvoid();
			  
			  updateSensors();
			  
			  finished=millis();
			  elapsed=finished-start;
			  if(elapsed>=4000){
				  Stop();
				  updateSensors();
				  break;
			  }
		  }
	  }
	  
	  else if(dis_front<min_dis){
		  while(dis_back>=min_dis){
			  Reverse();
			  
			  updateSensors();
			  
			  finished=millis();
			  elapsed=finished-start;
			  if(elapsed>=4000){
				  Stop();
				  updateSensors();
				  break;
			  }
		  }
	  }
	  
	  else{
		  Stop();
		  //cry for help
	  }
	  
  }
  
  updateCorners();
  updateSensors();
  
  //goes forward if nothing in the way
  if(dis_front>=min_dis  && upRight==false && upLeft==false && currentCommand==forward){
    frontAvoid();
	
    if(currentCommand==stopp){
      Stop();
    }
	
	updateCorners();
    updateSensors();
	
	if(dis_front<min_dis || upRight==true || upLeft==true){
		Stop();
	}
  }

  updateCorners();
  updateSensors();
  
  //FIND TIMING FOR A 90 DEGREE TURN
  //used for if goes into corner or similar situation
  if((upRight==true || upLeft==true || dis_front<min_dis) && currentCommand==forward){
    while(dis_back>=min_dis && backLeft==false && backRight==false){
      Reverse();
      updateCorners();
      updateSensors();
    }
	
	start=millis();
    while(upRight==false && dis_right>=min_dis && backRight==false){
      Right();
      
	  updateCorners();
      updateSensors();
	  
	  finished=millis();
	  elapsed=finished-start;
	  if(elapsed>=4000){
		  Stop();
		  break;
	  }
    }
    while(upLeft == false && dis_left >= min_dis && backLeft == false) {
      Left();
      
	    updateCorners();
      updateSensors();
      
	    finished = millis();
	    elapsed = finished - start;
	    if(elapsed >= 4000){ Stop(); break; }
    }
    while(backLeft == false && dis_back >= min_dis && backRight == false) {
      Reverse();
		
	    updateCorners();
      updateSensors();
      
	    finished = millis();
	    elapsed = finished - start;
	    if(elapsed>=4000){ Stop(); break; }
    } 
  }

  updateCorners();
  updateSensors();
  
  while(currentCommand == stopp){ Stop(); }
  
}
