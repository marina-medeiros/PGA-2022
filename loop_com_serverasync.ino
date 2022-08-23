// Importa bibliotecas necessárias
#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <IOXhop_FirebaseESP32.h>
#include <ArduinoJson.h>

#define bombaREAL 33 // BOMBA 
#define valvulaDES 25
#define valvulaSUB 26
#define SENSOR 13

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


// credenciais de rede e do banco de dados firebase
//String FIREBASE_HOST; //"https://hidra-12609-default-rtdb.firebaseio.com"
//String FIREBASE_AUTH; //"OEjbMc0QKRxMHh4QvmyNkKz7okkacTEKM1VYqk2j"

const char* ssid = "Redmi Note 11";
const char* password = "kpvf6368";

// Define parâmetros de comunicação GET
const char* PARAM_INPUT_1 = "output";
const char* PARAM_INPUT_2 = "state";

// Cria objeto AsyncWebServer na porta 80
AsyncWebServer server(80);

void setup(){
  // Porta Serial pra depuração
  Serial.begin(115200);

  // Inicialização dos pinos
  pinMode(bombaREAL, OUTPUT);
  pinMode(valvulaDES, OUTPUT);
  pinMode(valvulaSUB, OUTPUT);
  pinMode(SENSOR, INPUT_PULLUP);

  pulseCount = 0;
  flowRate = 0.0;
  flowMilliLitres = 0;
  totalMilliLitres = 0;
  previousMillis = 0;

  attachInterrupt(digitalPinToInterrupt(SENSOR), pulseCounter, FALLING);
  
  // Conexão com WiFi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi..");
  }
  
  // Adiciona header para permitir CORS
  DefaultHeaders::Instance().addHeader("Access-Control-Allow-Origin", "*");

  //Firebase.setString("espIp", WiFi.localIP().toString());

  // Inicializa conexão firebase
  //Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);

  // Lê requisições GET em <ESP_IP>/update?output=<inputMessage1>&state=<inputMessage2>
  server.on("/update", HTTP_GET, [] (AsyncWebServerRequest *request) {
    String inputMessage1;
    String inputMessage2;
    // Valor imput1 do GET em <ESP_IP>/update?output=<inputMessage1>&state=<inputMessage2>
    if (request->hasParam(PARAM_INPUT_1) && request->hasParam(PARAM_INPUT_2)) {
      inputMessage1 = request->getParam(PARAM_INPUT_1)->value();
      inputMessage2 = request->getParam(PARAM_INPUT_2)->value();
      digitalWrite(inputMessage1.toInt(), inputMessage2.toInt());
      String nome = (inputMessage1.toInt() == 2) ?  "bomba" : "EVbomba";
      bool valor = (inputMessage2.toInt() == 1);

      // Atualiza valor no firebase
      //Firebase.setBool(nome, valor);
    }
    else {
      inputMessage1 = "No message sent";
      inputMessage2 = "No message sent";
    }
    Serial.print("GPIO: ");
    Serial.print(inputMessage1);
    Serial.print(" - Set to: ");
    Serial.println(inputMessage2);
    request->send(200, "text/plain", "OK");
  });

  // Inicializa o servidor
  server.begin();
}

void loop(){
  delay(1000);
  Serial.println(WiFi.localIP().toString());
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

    // Print the cumulative total of litres flowed since starting
    Serial.print("Output Liquid Quantity: ");
    Serial.print(totalMilliLitres);
    Serial.print(" mL / ");
    Serial.print(totalMilliLitres / 1000);
    Serial.println(" L");
  }
}
