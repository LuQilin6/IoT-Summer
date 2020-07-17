#include<SoftwareSerial.h>
SoftwareSerial mySerialPort(4,5); //Read X, Transmit X

char msgIn;
char msgOut;
bool isJoe = false;
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600); //Serial communication for the serial monitor
  mySerialPort.begin(9600); // communication between serial monitor and software Serial Pins
}

void loop() {
  // put your main code here, to run repeatedly:
  while(mySerialPort.available()){ //While I have data in the buffer, run this loop
    if(isJoe){
      Serial.print("You: ");
      isJoe=false;
    }
    //eg. send "hello"
    msgIn = mySerialPort.read();//receiving message, reading each character at a time
    Serial.print(msgIn);
    delay(2);
  }

  while(Serial.available()){//while i have data in Serial Buffer, run this loop
    if(!isJoe){
      Serial.print("Joe: ");
      isJoe=true;
    }
    msgOut = Serial.read();//read each character
    mySerialPort.write(msgOut);
    Serial.print(msgOut);
    delay(2);
  }
}
