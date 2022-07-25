//está incompleto e com erros
#define Button 4 // ativa a bomba
#define led1 33 // BOMBA d33
#define led2 13 // Eletroválvula de subida
#define led3 12 // Eletroválvula de descida
#define SENSOR 27 // SENSOR DE FLUXO

#include <WiFi.h>
#include <IOXhop_FirebaseESP32.h>
#include <ArduionoJson.h>

#define WIFI_SSID "AnaLaura"
#define WIFI_PASSWORD "astres07"
#define FIREBASE_HOST "https://teste-esp32-d3fd3-default-rtdb.firebaseio.com"
#define FIREBASE_AUTH "3JJOz8jUnOlzXUdhULfdpusL8TH1s0JX3FJZh3gp"

////// CONFIGURAÇÕES DO SENSOR DE FLUXO //////////
long currentMillis = 0;
long previousMillis = 0;
int interval = 1000;
boolean ledState = LOW;
float calibrationFactor = 1.5; // ajustar aqui caso esteja lendo errado
volatile byte pulseCount;
byte pulse1Sec = 0;
float flowRate;
unsigned int flowMilliLitres;
unsigned long totalMilliLitres;

void IRAM_ATTR pulseCounter()
{
  pulseCount++;
}
///////////////////////////////////////////////

void setup() {
  //////////////SENSOR DE FLUXO//////////////
  Serial.begin(115200);

  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(SENSOR, INPUT_PULLUP);

  pulseCount = 0;
  flowRate = 0.0;
  flowMilliLitres = 0;
  totalMilliLitres = 0;
  previousMillis = 0;

  attachInterrupt(digitalPinToInterrupt(SENSOR), pulseCounter, FALLING);
  //////////////////////////////////////////////
  
  pinMode(led1, OUTPUT);
  pinMode(led2, OUTPUT);
  pinMode(led3, OUTPUT);
  pinMode(Button, INPUT);

  //Serial.begin(9600);

  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Conectando");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println();
  Serial.print("Conectado ");
  Serial.println(WiFi.localIP());

  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
}


void loop() {
  // put your main code here, to run repeatedly:
  int ButtonState = digitalRead(Button);
  int evSubida = Firebase.getInt("evSubida"); //0 = fechada; 1 = aberta
  int evDescida = Firebase.getInt("evDescida");
  
  if(ButtonState == HIGH && evSubida == 1){
    digitalWrite(led1, HIGH);
  }
  if(evSubida == 1){
    if(ButtonState == HIGH){
    digitalWrite(led1, HIGH);
    }
    else{
    digitalWrite(led1,LOW);
    }
  }
  if(evSubida == 0 && ButtonState == HIGH){
    Serial.println("É necessário ligar a eletroválvula de subida.");
  }
  
   // pensar em uma form de como fazer executar as duas partes sem q tenha delay
}

void SensorDeFluxo(){
  currentMillis = millis();
  if (currentMillis - previousMillis > interval) {
    pulse1Sec = pulseCount;
    pulseCount = 0;
    flowRate = ((1000.0 / (millis() - previousMillis)) * pulse1Sec) / calibrationFactor;
    previousMillis = millis();
    flowMilliLitres = (flowRate / 60) * 1000;
    totalMilliLitres += flowMilliLitres;
    Serial.print("Flow rate: ");
    Serial.print(int(flowRate));
    Serial.print("L/min");
    Serial.print("\t");
    Serial.print("Output Liquid Quantity: ");
    Serial.print(totalMilliLitres); // total de ml entrando
    Serial.print("mL / ");
    Serial.print(totalMilliLitres / 1000);
    Serial.println("L");
  }

  Firebase.setInt("vazãoFluxoBomba", flowRate);
   delay(10800000); //3h
}
