int moveState;
bool lightTracker;

void setup() {

  Serial.begin(9600);
  pinMode(LED_BUILTIN, OUTPUT);


}

void loop() {
  
  if(Serial.available() > 0) {
    
    moveState = Serial.parseInt();
    if(lightTracker) {
      digitalWrite(LED_BUILTIN, HIGH);
      lightTracker = false; 
    } // end if
    else {
      digitalWrite(LED_BUILTIN, LOW);
      lightTracker = true;
    } // end else
  } // end if

}
