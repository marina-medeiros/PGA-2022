#define fluxo 13 // fluxo subida

void setup() {
  pinMode(fluxo, INPUT);
  Serial.begin(9600);

}
void loop(){
  float valvulaValor = digitalRead(fluxo);
  Serial.println(valvulaValor);

  delay(3000);
}
