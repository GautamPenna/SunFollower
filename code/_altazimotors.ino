
#define Alt_stepPin 5
#define Alt_dirPin 6
#define Alt_MinD 0
#define Alt_MaxD 90
#define Alt_MinS 0
#define Alt_MaxS 100
#define Alt_HomeP 12

#define Azi_stepPin 7
#define Azi_dirPin 8
#define Azi_MinD 0
#define Azi_MaxD 270
#define Azi_MinS 0
#define Azi_MaxS 300
#define Azi_HomeP 13

int altStep = 0;
float AltAngle = 56;

int aziStep = 0;
float AziAngle = 100;

void setup() {

  Serial.begin(9600);

  pinMode(Alt_stepPin , OUTPUT);
  pinMode(Alt_dirPin , OUTPUT);
  pinMode(Alt_HomeP , INPUT_PULLUP);

  pinMode(Azi_stepPin , OUTPUT);
  pinMode(Azi_dirPin , OUTPUT);
  pinMode(Azi_HomeP , INPUT_PULLUP);

}

void loop() {

  for(int g = 0; g < 90;) {

    float k = g + 0.369;
    goToCurAltPos(AltAngle);
    Serial.print("Alt Pos : "); 
    Serial.print(k);
    Serial.print(" ALt Motor step is : "); 
    Serial.println(altStep);
    
    delay(100);
    g = g + 1;

    altStep = altStep + 1;
    
  }

  delay(1000);
  gotBacktoAltHome();
  altStep = 0;



  for(int g = 0; g < 270;) {

    float k = g + 0.369;
    goToCurAziPos(AziAngle);
    Serial.print("Azi Pos : "); 
    Serial.print(k);
    Serial.print(" Azi Motor step is : "); 
    Serial.println(aziStep);
    
    delay(100);
    g = g + 1;

    aziStep = aziStep + 1;
    
  }

  delay(1000);
  gotBacktoAziHome();
  aziStep = 0;
  
}

void gotBacktoAltHome() {

  while (digitalRead(Alt_HomeP)) {
    digitalWrite(Alt_dirPin , LOW);
    digitalWrite(Alt_stepPin , HIGH);
    delay(10);
    digitalWrite(Alt_stepPin , LOW);
    delay(10);
  }

  while (!digitalRead(Alt_HomeP)) { // move one step
    digitalWrite(Alt_dirPin , HIGH);
    digitalWrite(Alt_stepPin , HIGH);
    delay(10);
    digitalWrite(Alt_stepPin , LOW);
    delay(10);
  }

  delay(1000);

  int altStep = Alt_MaxS + 15;
  Serial.print("This is the Max Steps : ");
  Serial.println(altStep);

  while (altStep > Alt_MinS) {
    digitalWrite(Alt_dirPin , HIGH);
    digitalWrite(Alt_stepPin , HIGH);
    delay(10);
    digitalWrite(Alt_stepPin , LOW);
    delay(10);
    altStep = altStep - 1;
  }

}

void goToCurAltPos(float toAlt) {

    digitalWrite(Alt_dirPin , HIGH);
    digitalWrite(Alt_stepPin , HIGH);
    delay(10);
    digitalWrite(Alt_stepPin , LOW);
    delay(10);
    
}





void gotBacktoAziHome() {

  while (digitalRead(Azi_HomeP)) {
    digitalWrite(Azi_dirPin , LOW);
    digitalWrite(Azi_stepPin , HIGH);0
    delay(10);
    digitalWrite(Azi_stepPin , LOW);
    delay(10);
  }

  while (!digitalRead(Azi_HomeP)) { // move one step
    digitalWrite(Azi_dirPin , HIGH);
    digitalWrite(Azi_stepPin , HIGH);
    delay(10);
    digitalWrite(Azi_stepPin , LOW);
    delay(10);
  }

  delay(1000);

  int aziStep = Azi_MaxS + 15;
  Serial.print("This is the Max Steps : ");
  Serial.println(aziStep);

  while (aziStep > Azi_MinS) {
    digitalWrite(Azi_dirPin , HIGH);
    digitalWrite(Azi_stepPin , HIGH);
    delay(10);
    digitalWrite(Azi_stepPin , LOW);
    delay(10);
    aziStep = aziStep - 1;
  }

}

void goToCurAziPos(float toAzi) {

    digitalWrite(Azi_dirPin , HIGH);
    digitalWrite(Azi_stepPin , HIGH);
    delay(10);
    digitalWrite(Azi_stepPin , LOW);
    delay(10);
    
}


