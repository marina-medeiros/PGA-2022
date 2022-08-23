#define bombaREAL 33 // BOMBA 
#define valvulaDES 25
#define SENSOR 13
//ATENÇÃO: colocar velocidade 115200 no terminal.

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

void setup() {
  pinMode(bombaREAL, OUTPUT);
  pinMode(valvulaDES, OUTPUT);

  Serial.begin(115200);

  pinMode(SENSOR, INPUT_PULLUP);

  pulseCount = 0;
  flowRate = 0.0;
  flowMilliLitres = 0;
  totalMilliLitres = 0;
  previousMillis = 0;

  attachInterrupt(digitalPinToInterrupt(SENSOR), pulseCounter, FALLING);
}

void loop(){
  digitalWrite(bombaREAL, HIGH);
  //Serial.println("Bomba LIGADA");
  //Serial.println(" ");
  delay(2000);
  digitalWrite(valvulaDES, HIGH);
  //Serial.println("Válvula LIGADA");
  //Serial.println(" ");
  delay(3000);
  digitalWrite(bombaREAL, LOW);
  //Serial.println("Bomba desligada");
  //Serial.println(" ");
  delay(2000);
  digitalWrite(valvulaDES, LOW);
  //Serial.println("Válvula desligada");
  //Serial.println(" ");
  delay(3000);

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
