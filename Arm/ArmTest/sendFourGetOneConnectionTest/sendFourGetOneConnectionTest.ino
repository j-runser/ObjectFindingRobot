int M1, M2, M3, M4;
bool lightTracker = true;

void setup() {
  
  Serial.begin(9600);
  pinMode(LED_BUILTIN, OUTPUT);

}

void loop() {
  
  if(Serial.available() > 0) {
    M1 = Serial.parseInt();
    M2 = Serial.parseInt();
    M3 = Serial.parseInt();
    M4 = Serial.parseInt();
    
    Serial.println(M4);

    if(lightTracker) {
      digitalWrite(LED_BUILTIN, HIGH);
      lightTracker = false; 
    } // end if
    else {
      digitalWrite(LED_BUILTIN, LOW);
      lightTracker = true;
    } // end else
  } // end if

} // end loop
