void setup() {
  // put your setup code here, to run once:
  pinMode(4, OUTPUT); //G
  pinMode(5, OUTPUT);//R
  pinMode(2, OUTPUT);//B
  analogWriteRange(255);
  digitalWrite(4, HIGH);
  digitalWrite(5, HIGH);
  digitalWrite(2, HIGH);
}

void loop() {
  // put your main code here, to run repeatedly:
    /*digitalWrite(2, LOW);
    digitalWrite(5, LOW);
    delay(500);
    digitalWrite(2, HIGH);
    digitalWrite(5, HIGH);
    digitalWrite(5, LOW);
    digitalWrite(4, LOW);
    delay(500);
    digitalWrite(5, HIGH);
    digitalWrite(4, HIGH);
    digitalWrite(4, LOW);
    digitalWrite(2, LOW);
    delay(500);
    digitalWrite(4, HIGH);
    digitalWrite(2, HIGH);*/

    for(int i=255; i>=0; i--){
      analogWrite(2, 255-i);
      analogWrite(4, 255-2*i);
      analogWrite(5, 255-i/2);
      delay(10);
    }
    delay(1000);
}
