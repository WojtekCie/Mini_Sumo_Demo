// ----- Pins values ------
// Motors and MicroController:
int speedLPin = 3;
int mldir1 = 6; //Left
int mldir2 = 9;
int speedRPin = 10;
int mrdir1 = 5; //Right
int mrdir2 = 8;
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
int mLSpeed = 80;
int mRSpeed = 80;
bool mLback = false;
bool mRback = false;
float AnDistSenVal = 0;               // Zmierzona wartość czujnika odległości
float DistToOb = 0;                   // Odległość do obiektu [cm]
int AnColorSenLeftVal = 0;            // Zmierzona wartość lewego czujnika koloru
int AnColorSenRightVal = 0;           // Zmierzona wartość prawego czujnika koloru
int colorBorder = 300;                // Manualnie ustawiana granica od której robot odrużnia biały od czarnego (jeśli wartość zczytana jest mniejsza od granicy to biały)
int turnTime = 800;                    // Czas ile ma się obracać [ms]
int operationTime = 0;                // Zmienna urzywana do zliczania czasu wykonywania komendy [ms]
bool buttonMemory = 0;                // Przycisk potrzebuje aż 3 zmienny aby działać w wymagany sposób (właczać dopiero po puszczeniu, wyłączać odrazu po kliknieciu)
bool buttonPressed = false;           //
bool highVoltageButtonMemory = false; // Zmienna potrzebna do zarządzania przyciskiem
bool BattleMode = false;              // Odpowiada za stan rbota  T/F -> Walka na ringu/Spoczynek
bool SafetyManeuver = false;          // Zmienna deklarująca czy trwają aktualnie manewry które nie mogą być przerwane
          
enum class SafetyMove{
  TurnLeft,                         // Zakręt w lewo wymagany przez białą linie
  TurnRight,                        // Zakręt w prawo wymagany przez białą linie
};
SafetyMove executing;

// ----- Funkcje do zczytywania danych z czujników -----
void pobranieDanychzCzujnikow(){
  AnDistSenVal = analogRead(AnDistSenPin);
  DistToOb = ReadToDist(AnDistSenVal);
  AnColorSenLeftVal = analogRead(AnColorSenLeftPin);
  AnColorSenRightVal = analogRead(AnColorSenRightPin);
  //Serial.print("Left Colour Sensor: ");
  //Serial.println(AnColorSenLeftVal);
  //Serial.print("Right Colour Sensor: ");
  //Serial.println(AnColorSenRightVal);
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

// Updates variables related with movement priority
void updateMovement(){
  int actualManuverDurrations = millis() - operationTime ;
  //Serial.println(actualManuverDurrations);  //Loop trwa 6-8ms
  if(SafetyManeuver && actualManuverDurrations>turnTime){
    SafetyManeuver = false;
  }
  if(!SafetyManeuver && AnColorSenLeftVal<colorBorder){
    operationTime = millis();
    SafetyManeuver = true;
    executing = SafetyMove::TurnRight;
    //Serial.println("Turning Right");
  }
  if(!SafetyManeuver && AnColorSenRightVal<colorBorder){
    operationTime = millis();
    SafetyManeuver = true;
    executing = SafetyMove::TurnLeft;
    //Serial.println(operationTime);    
  }
}
void chooseMovement(){
  if(SafetyManeuver){
    switch (executing){
      case SafetyMove::TurnLeft:
        turnLeft();
        break;
      case SafetyMove::TurnRight:
        turnRight();
        break;
    }
  }else{
    // Do przodu
    forward();
  }
}

// ----- Movement functions -----
void move(){
  if(mLback){
    analogWrite(mldir1,0);
    analogWrite(mldir2,255);
  }else{
    analogWrite(mldir1,255);
    analogWrite(mldir2,0);
  }
  if(mRback){
    analogWrite(mrdir1,0);
    analogWrite(mrdir2,255);
  }else{
    analogWrite(mrdir1,255);
    analogWrite(mrdir2,0);
  }
  analogWrite(speedLPin, mLSpeed);
  analogWrite(speedRPin, mRSpeed);
  //Serial.print("SpeedLeft: ");
  //Serial.println(mLSpeed);
  //Serial.println("SpeedRight: ");
  //Serial.println(mRSpeed);
  //Serial.println("move!!!");
}
void stop(){
  analogWrite(mldir1,0);
  analogWrite(mldir2,0);
  analogWrite(mrdir1,0);
  analogWrite(mrdir2,0);
  analogWrite(speedLPin, 0);
  analogWrite(speedRPin, 0);
  //Serial.println("STOP");
}
void turnLeft(){
  mLback = true;
  mRback = false;
  mLSpeed = 255;
  mRSpeed = 255;
  //Serial.println("Turning Left...");    
}
void turnRight(){
  mLback = false;
  mRback = true;
  mLSpeed = 255;
  mRSpeed = 255;
}
void forward(){
  mLback = false;
  mRback = false;
  mLSpeed = 255;
  mRSpeed = 255;
  //Serial.println("Forward");
}
// ----- End Movement -----


void setup() {
  analogWrite(signalDiodAnOut, 255); // LED sygnalizes start of setap loop
  Serial.begin(9600); 

  // put your setup code here, to run once:

  //Motors:
  pinMode(speedLPin, OUTPUT);
  pinMode(speedRPin, OUTPUT);
  pinMode(mldir1, OUTPUT);
  pinMode(mldir2, OUTPUT);
  pinMode(mrdir1, OUTPUT);
  pinMode(mrdir2, OUTPUT);

  //Sensors:
  pinMode(AnDistSenPin, INPUT);
  pinMode(AnColorSenLeftPin, INPUT);
  pinMode(AnColorSenRightPin, INPUT);

  analogWrite(signalDiodAnOut, 0); // LED sygnalizes end of setap loop
}

void loop() {


  pobranieDanychzCzujnikow();

  // Battle Mode?
  if(BattleMode){
    updateMovement();
    chooseMovement();
    move();
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

