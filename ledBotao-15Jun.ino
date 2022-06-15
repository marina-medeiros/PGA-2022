#define LED 5
#define Button 4
#define LED2 2

void setup() {
  // put your setup code here, to run once:
  pinMode(LED, OUTPUT);
  pinMode(Button, INPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  digitalWrite(LED2, LOW);
  delay(400);
  digitalWrite(LED2, HIGH);
  delay(400);

  int ButtonState = digitalRead(Button);
  if(ButtonState == HIGH){
    digitalWrite(LED, HIGH);
  }
  else{
    digitalWrite(LED, LOW);
  }

  
}
