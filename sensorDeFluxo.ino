#define LED_BUILTIN 2
#define SENSOR 15 // POTENCIÔMETRO
//ATENÇÃO: colocar velocidade 115200 no terminal.

#include <WiFi.h>
#include <IOXhop_FirebaseESP32.h>
#include <ArduinoJson.h>

#define WIFI_SSID "AnaLaura"
#define WIFI_PASSWORD "astres07"
#define FIREBASE_HOST "https://teste-esp32-d3fd3-default-rtdb.firebaseio.com"
#define FIREBASE_AUTH "3JJOz8jUnOlzXUdhULfdpusL8TH1s0JX3FJZh3gp"

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

void setup(){
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(SENSOR, INPUT);
  
  Serial.begin(115200);

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

  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(SENSOR, INPUT_PULLUP);

  pulseCount = 0;
  flowRate = 0.0;
  flowMilliLitres = 0;
  totalMilliLitres = 0;
  previousMillis = 0;

  attachInterrupt(digitalPinToInterrupt(SENSOR), pulseCounter, FALLING);
}

void loop(){
  currentMillis = millis();
  if (currentMillis - previousMillis > interval) {
    
    pulse1Sec = pulseCount;
    pulseCount = 0;
    flowRate = ((1000.0 / (millis() - previousMillis)) * pulse1Sec) / calibrationFactor;
    previousMillis = millis();
    flowMilliLitres = (flowRate / 60) * 1000;
    totalMilliLitres += flowMilliLitres;
    Serial.print("Flow rate: ");
    Serial.print(int(flowRate));  // Print the integer part of the variable
    Serial.print("L/min");
    Serial.print("\t");       // Print tab space
    Serial.print("Output Liquid Quantity: ");
    Serial.print(totalMilliLitres / 1000);
    Serial.println("L");

    Firebase.setInt("/valFluxo/vazao", flowRate);
    Firebase.setInt("/valFluxo/litrosReservatorio", totalMilliLitres/1000);
  }

}
