#include"DHT.h"
#include<Servo.h>
#define DHTPIN 4
#define DHTTYPE DHT22
DHT dht (DHTPIN, DHTTYPE);
Servo myservo;
int R=15;
int G=16;
int B=5;
int pwm=13;
int motorValue=0;
float tem;
float C;
float hum;
int red=0;
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(R, OUTPUT);
  pinMode(B, OUTPUT);
  pinMode(G, OUTPUT);
  myservo.attach(pwm);
  dht.begin();
  analogWriteRange(255);
  analogWrite(G, 255);
  analogWrite(R, 255);
}

void loop() {
  // put your main code here, to run repeatedly:
  tem=dht.readTemperature();
  Serial.println(tem);
  motorValue=map(tem, 0, 30, 0, 180);
  myservo.write(motorValue);
  red=map(tem, 0, 30, 0, 255);
  analogWrite(B, 255-red);
  delay(2000);
}
