#define led1 5
#define Button 4
#define led2 2
#define led3 18

void setup() {
  // put your setup code here, to run once:
  pinMode(led1, OUTPUT);
  pinMode(led2, OUTPUT);
  pinMode(led3, OUTPUT);
  pinMode(Button, INPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  int ButtonState = digitalRead(Button);
  if(ButtonState == HIGH){
    Mode01();
  }
  if(ButtonState == LOW){
    Mode02();
  }

}

void Mode01(){
  digitalWrite(led2, HIGH);
  delay(500);
  digitalWrite(led3, HIGH);
  digitalWrite(led2, LOW);
  delay(500);
  digitalWrite(led3, LOW);
}

void Mode02(){
  digitalWrite(led1, HIGH);
  delay(500);
  digitalWrite(led1, LOW);
}
