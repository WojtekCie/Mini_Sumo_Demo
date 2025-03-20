// ----- Pins values ------
// Motors and MicroController:
int speedPin = 3;
int m1dir1 = 6;
int m1dir2 = 9;
int speedM2Pin = 10;
int m2dir1 = 5;
int m2dir2 = 8;
int mSpeed = 50;
//Analog Distance Sensor:
int AnDistSenPin = 3;
//Analog Color Sensors
int AnColorSenLeftPin = 1;
int AnColorSenRightPin = 2;
//Analog Button
int AnButtonSenPin = A0;
//Sygnal Diod
int signalDiodAnOut = 11;
// ----- End Pins -----

//Global variables
float AnDistSenVal = 0;               // Zmierzona wartość czujnika odległości
float DistToOb = 0;                   // Odległość do obiektu [cm]
int AnColorSenLeftVal = 0;            // Zmierzona wartość lewego czujnika koloru
int AnColorSenRightVal = 0;           // Zmierzona wartość prawego czujnika koloru
int turnTime = 10;                    // Czas ile ma się obracać [ms]
int operationTime = 0;                // Zmienna urzywana do zliczania czasu wykonywania komendy [ms]
bool buttonMemory = 0;                // Przycisk potrzebuje aż 3 zmienny aby działać w wymagany sposób (właczać dopiero po puszczeniu, wyłączać odrazu po kliknieciu)
bool buttonPressed = false;           //
bool highVoltageButtonMemory = false; // Zmienna potrzebna do zarządzania przyciskiem
bool BattleMode = false;              // Odpowiada za stan rbota  T/F -> Walka na ringu/Spoczynek
bool SafetyManeuver = false;          // Zmienna deklarująca czy trwają aktualnie manewry które nie mogą być przerwane

// ----- Funkcje do zczytywania danych z czujników -----
void pobranieDanychzCzujnikow(){
  AnDistSenVal = analogRead(AnDistSenPin);
  DistToOb = ReadToDist(AnDistSenVal);
  AnColorSenLeftVal = analogRead(AnColorSenLeftPin);
  AnColorSenRightVal = analogRead(AnColorSenRightPin);
  updateButton();
  BattleModeOnOff();
}

void updateButton(){
   if(buttonMemory==false && buttonPressed==false){
    highVoltageButtonMemory = 1;
  }
  buttonMemory = buttonPressed;
  if(digitalRead(AnButtonSenPin) == 0){
    analogWrite(signalDiodAnOut,230);
    buttonPressed = true;
  }else{
    buttonPressed = false;
    digitalWrite(signalDiodAnOut,LOW);
  }
}
void BattleModeOnOff(){
  if(buttonPressed == false && buttonMemory == true && highVoltageButtonMemory == 1){
    BattleMode = true;
  }else if(buttonPressed == true && buttonMemory == false && BattleMode==true){
    BattleMode = false;
    highVoltageButtonMemory = 0;
  }
}
// ----- End Data Functions ----

void updateMovement(){
  int actualManuverDurrations = milis() - operationTime 
  if(SafetyManeuver && actualManuverDurrations>turnTime){
    SafetyManeuver
  }
}

// ----- Movement functions -----
void stop(){
  analogWrite(m1dir1,0);
  analogWrite(m1dir2,0);
  analogWrite(m2dir1,0);
  analogWrite(m2dir2,0);
}
// ----- End Movement -----


void setup() {
    Serial.begin(9600); 

  // put your setup code here, to run once:

  //Motors:
  pinMode(speedPin, OUTPUT);
  pinMode(speedM2Pin, OUTPUT);
  pinMode(m1dir1, OUTPUT);
  pinMode(m1dir2, OUTPUT);
  pinMode(m2dir1, OUTPUT);
  pinMode(m2dir2, OUTPUT);

  //Sensors:
  pinMode(AnDistSenPin, INPUT);
  pinMode(AnColorSenLeftPin, INPUT);
  pinMode(AnColorSenRightPin, INPUT);

}

void loop() {


  pobranieDanychzCzujnikow();

  // Battle Mode?
  if(BattleMode){
    updateMovement();
  }else{
    stop();
  }


}
  // converting Analog Read to distance (cm)
int ReadToDist(float read){
  float volts = analogRead(AnDistSenPin)*0.0048828125;  // value from sensor * (5/1024)
  float distance = 13*pow(volts, -1);
  //Serial.print(distance);     
  //Serial.println(" cm");
  return distance;
  delay(100);
}

