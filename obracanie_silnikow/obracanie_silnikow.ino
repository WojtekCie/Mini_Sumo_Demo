//GitHub - Test
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
float AnDistSenVal = 0;

float DistToOb = 0;

//Analog Color Sensors
int AnColorSenLeftPin = 1;
int AnColorSenRightPin = 2;


void setup() {
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

  Serial.begin(9600); 
}

void loop() {


  AnDistSenVal = analogRead(AnDistSenPin);
  DistToOb = ReadToDist(AnDistSenVal);
  Serial.println(DistToOb);
  int AnColorSenLeftVal = analogRead(AnColorSenLeftPin);
  int AnColorSenRightVal = analogRead(AnColorSenRightPin);

  if(DistToOb < 100){
    mSpeed = 255;
  }else{
    mSpeed = 50;
  }
 
  if(AnColorSenLeftVal < 200 || AnColorSenRightVal < 200){
     mSpeed = 0;
  }
  analogWrite(m1dir1,255);
  analogWrite(m1dir2,0);
  analogWrite(m2dir1,255);
  analogWrite(m2dir2,0);
  //digitalWrite(m1dir1,HIGH);
  //digitalWrite(m1dir2,LOW);
  //digitalWrite(m2dir1,HIGH);
  //digitalWrite(m2dir2,LOW);
  mSpeed = 0; //Prevents motors from spining
  analogWrite(speedPin, mSpeed); //Obraca sie
  analogWrite(speedM2Pin, mSpeed); //Obraca sie
  delay(10);


}
  // converting Analog Read to distance (cm)
int ReadToDist(float read){
  float volts = analogRead(AnDistSenPin)*0.0048828125;  // value from sensor * (5/1024)
  float distance = 13*pow(volts, -1);
  Serial.print(distance);     
  Serial.println(" cm");
  return distance;
  delay(100);
}

