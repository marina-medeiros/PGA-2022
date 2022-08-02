#define Button 15 // ativa a bomba
#define led1 2 // BOMBA 
#define led2 4 // Eletroválvula de subida 

#include <WiFi.h>
#include <IOXhop_FirebaseESP32.h>
#include <ArduinoJson.h>

#define WIFI_SSID "AnaLaura"
#define WIFI_PASSWORD "astres07"
#define FIREBASE_HOST "https://teste-esp32-d3fd3-default-rtdb.firebaseio.com"
#define FIREBASE_AUTH "3JJOz8jUnOlzXUdhULfdpusL8TH1s0JX3FJZh3gp"

void setup() {
  // put your setup code here, to run once:
  pinMode(led1, OUTPUT);
  pinMode(led2, OUTPUT);
  pinMode(Button, INPUT);

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
  int ButtonState = digitalRead(Button);
  int evSubida = Firebase.getInt("/valvulas/evSubida"); //0 = fechada; 1 = aberta
  
  if(evSubida == 1){
    if(ButtonState == HIGH){
    digitalWrite(led1, HIGH); //BOMBA
    Firebase.setInt("/bomba/estado", 1);
    }
    else{
    digitalWrite(led1,LOW);
    Firebase.setInt("/bomba/estado", 0);
    }
    digitalWrite(led2,HIGH);
  }
  if(evSubida == 0 && ButtonState == HIGH){
    Serial.println("É necessário ligar a eletroválvula de subida.");
  }


  
}
