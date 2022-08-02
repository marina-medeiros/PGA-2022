#define Button1 4 // sensor de nível 1 100%
#define Button2 12 // sensor de nível 2 50%
#define Button3 13 // sensor de nivel 3 25%

#include <WiFi.h>
#include <IOXhop_FirebaseESP32.h>
#include <ArduinoJson.h>

#define WIFI_SSID "AnaLaura"
#define WIFI_PASSWORD "astres07"
#define FIREBASE_HOST "https://teste-esp32-d3fd3-default-rtdb.firebaseio.com"
#define FIREBASE_AUTH "3JJOz8jUnOlzXUdhULfdpusL8TH1s0JX3FJZh3gp"

void setup() {
  // put your setup code here, to run once:
  pinMode(Button1, INPUT);
  pinMode(Button2, INPUT);
  pinMode(Button3, INPUT);

  Serial.begin(9600);

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
  int sensor1 = digitalRead(Button1);
  int sensor2 = digitalRead(Button2);
  int sensor3 = digitalRead(Button3);
  int a = 0;
  int b = 0;
  int c = 0;

  if(sensor1 == HIGH){
    a = 1;
  }
  if(sensor2 == HIGH){
    b = 2;
  }
  if(sensor3 == HIGH){
    c = 4;
  }
  int valor = a + b + c;

  if(sensor1 == LOW && sensor2 == LOW && sensor3 == LOW){
    Serial.println("0% de água");
    Firebase.setInt("/sensoresDeNivel/25", 0);
    Firebase.setInt("/sensoresDeNivel/75", 0);
    Firebase.setInt("/sensoresDeNivel/100", 0);
  }
  if(sensor1 == LOW && sensor2 == LOW && sensor3 == HIGH){
    Serial.println("25% de água");
    Firebase.setInt("/sensoresDeNivel/25", 1);
    Firebase.setInt("/sensoresDeNivel/75", 0);
    Firebase.setInt("/sensoresDeNivel/100", 0);
  }
  if(sensor1 == LOW && sensor2 == HIGH && sensor3 == HIGH){
    Serial.println("75% de água");
    Firebase.setInt("/sensoresDeNivel/25", 1);
    Firebase.setInt("/sensoresDeNivel/75", 1);
    Firebase.setInt("/sensoresDeNivel/100", 0);
  }
  if(sensor1 == HIGH && sensor2 == HIGH && sensor3 == HIGH){
    Serial.println("100% de água");
    Firebase.setInt("/sensoresDeNivel/25", 1);
    Firebase.setInt("/sensoresDeNivel/75", 1);
    Firebase.setInt("/sensoresDeNivel/100", 1);
  }
  //ANOMALIAS:
  if(valor == 5 || valor == 3 || valor == 2 || valor || 1){
    Serial.println("Anomalia dectada em algum dos sensores de nível");
    Firebase.setInt("/sensoresDeNivel/25", 3);
    Firebase.setInt("/sensoresDeNivel/75", 3);
    Firebase.setInt("/sensoresDeNivel/100", 3);
  }

}
