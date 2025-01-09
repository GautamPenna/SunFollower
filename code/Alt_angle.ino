
int DEBUG = 1;

float latitude = -96.98;
float Alt_Angle;


void setup() {
  Serial.begin(9600);
}

void loop() {

  for (int decAngle = 0; decAngle <= 23 ; decAngle++) {
    Alt_Angle = 90 - decAngle + latitude;
    if (DEBUG == 1 || DEBUG == 99) {
    Serial.print("This is the Alitutde Angle : ");
    Serial.println(Alt_Angle);
    }
  }


}

