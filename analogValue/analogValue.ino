int ldr_pin = A0;
int analogValue = 0;
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(4, OUTPUT);
  pinMode(5, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  analogValue = analogRead(ldr_pin);
  Serial.println(analogValue);
  if(analogValue<=400 && analogValue>=0){
    digitalWrite(4, LOW);
    digitalWrite(5, LOW);
  }else if(analogValue>400 && analogValue<=700){
    digitalWrite(4, LOW);
    digitalWrite(5, HIGH);
  }else if(analogValue>700 && analogValue<=1024){
    digitalWrite(4, HIGH);
    digitalWrite(5, HIGH);
  }
  delay(100);
}
