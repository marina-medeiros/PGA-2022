void setup() {
  // put your setup code here, to run once:
pinMode(18, OUTPUT);

}

void loop() {
  // put your main code here, to run repeatedly:
  digitalWrite(18, LOW);
  delay(400);
  digitalWrite(18, HIGH);
  delay(400);

}
