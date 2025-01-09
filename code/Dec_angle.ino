float decAngle;

void setup() {
  // put your setup code here, to run once:

  Serial.begin(9600);

}

void loop() {

  for(int Nday = 1; Nday <= 365; Nday++) {

    //decAngle = asin(radians(0.39795*cos(radians(0.98563*(Nday - 173)))));
    decAngle = -23.45*cos(radians(0.98630136986*(Nday + 10))); // Got this from http://www.pveducation.org/pvcdrom/properties-of-sunlight/declination-angle
    Serial.print("This is the day number : ");
    Serial.print(Nday);
    Serial.print(" ");
    Serial.print("This is the declination angle for this day : ");
    Serial.println(decAngle);
  
    
  }

   delay(1000000);
  
}
