#include<Servo.h>


Servo myservo; //Object servo
int pwm = 4; //pin 4 on board
void setup() {
  // put your setup code here, to run once:
  myservo.attach(pwm);
  
}

void loop() {
  // put your main code here, to run repeatedly:
  myservo.write(90);
  delay(1000);
  myservo.write(0);
  delay(1000);
  myservo.write(180);
  delay(1000);
}
