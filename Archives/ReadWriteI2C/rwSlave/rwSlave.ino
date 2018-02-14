#include <Wire.h>

#define ADDRESS 0x04
int numOne = 0;
int numTwo = 0;
int counter = 0;
int sum;

void setup() {

  pinMode(LED_BUILTIN, OUTPUT);
  Serial.begin(9600);
  Wire.begin(ADDRESS);

  Wire.onReceive(receiveData);
  Wire.onRequest(sendData);

  Serial.println("Ready.");

}

void loop() {

  //sum = numOne + numTwo;
  delay(100);

}

void receiveData(int byteCount) {

  while(Wire.available()) {
    if(counter == 0) {
      numOne = Wire.read();

      Serial.print("data recived: ");
      Serial.println(numOne);

      counter++;
    } else if(counter == 1) {
      numTwo = Wire.read();

      Serial.print("data recived: ");
      Serial.println(numTwo);

      counter++;
    }
  }

  sum = numOne + numTwo;
  
}

void sendData() {
  
  Wire.write(sum);
  
}

