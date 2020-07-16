#include<Servo.h>
Servo myservo;
int pwm =4;
int ldr_pin = A0;
int analogValue=0;
int motorValue=0;
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  myservo.attach(pwm);
  
}

void loop() {
  // put your main code here, to run repeatedly:
  analogValue = analogRead(ldr_pin);
  Serial.println(analogValue);
  motorValue=map(analogValue, 0, 1024, 0, 180);
  myservo.write(motorValue);
  delay(500);
}
