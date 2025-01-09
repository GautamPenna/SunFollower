float SolarTime(float et , float aA) {
  float sTime = 0;

  float RandomC[] = { -15 , -30 , 45 , 75 , 165 , 150 , 180 , -60 , -90 , 15};

  int TODAY;

  int utcHr = 12;
  int utcMn = 0;

  int TZTSolarNoonH = utcHr - 12; // Time Zones to solar Noon Hour
  int TZTSolarNoonM = utcMn - 00; // Time Zones to solar noon Minutes

  float location = RandomC[random(0, 9)];

  float degTTM = (-1) * (15 * TZTSolarNoonH + float (TZTSolarNoonM / 4)); // How many degrees it is away at

  float Degrees_Away = location - degTTM;



  float minutes = int(Degrees_Away) % 15; // Minutes process
  float hours = ((Degrees_Away - minutes) / 15);



  float clockTimeHours = 11 + hours;
  float clockTimeMinutes = 60 + minutes;
  clockTimeMinutes = clockTimeMinutes / 60;
  float TotalHours1 = clockTimeMinutes + clockTimeHours;


  float eq = _eqOfTime(_Angle(TODAY));
  eq = eq / 60;
  float TotalHours = TotalHours1 + eq; // total hours of solar time

  float TotalHoursM = TotalHours * 60;
  int SolarM = int (TotalHoursM) % 60;
  int SolarH = ((TotalHoursM - SolarM) / 60);

  if (SolarM >= 60) {
    SolarM = SolarM - 60;
    SolarH = SolarH + 1;
  }

  if (SolarH >= 24) {
    SolarH = SolarH - 24;
  }



  return sTime;
}

void setup() {
  Serial.begin(9600);


}

void loop() {

  //float RandomC[] = {-77.0353137 , -0.3817868 , 39.848496 , 20.7810066 , 82.9823814 , 152.4326753 , 177.9562897 , -166.1692368 , 140.7968592 , -78.319844};

  float RandomC[] = {-15 , -30 , 45 , 75 , 165 , 150 , 180 , -60 , -90 , 15};


    int TODAY=274;

    int utcHr = 12;
    int utcMn = 0;

    float location = RandomC[random(0,9)];

    int TZTSolarNoonH = utcHr - 12; // Time Zones to solar Noon Hour
    int TZTSolarNoonM = utcMn - 00; // Time Zones to solar noon Minutes

    float degTTM = (-1)*(15*TZTSolarNoonH + float (TZTSolarNoonM/4)); // How many degrees it is away at

    float Degrees_Away = location - degTTM;

    float minutes = int(Degrees_Away)%15; // Minutes process
    float hours = ((Degrees_Away - minutes)/15);



    float clockTimeHours = 11 + hours;
    float clockTimeMinutes = 60 + minutes;
    clockTimeMinutes = clockTimeMinutes/60;
    float TotalHours1 = clockTimeMinutes + clockTimeHours;


    float eq = _eqOfTime(_Angle(TODAY));
    eq = eq/60;
    float TotalHours = TotalHours1 + eq; // total hours of solar time

    float TotalHoursM = TotalHours*60;
    int SolarM = int (TotalHoursM)%60;
    int SolarH = ((TotalHoursM - SolarM)/60);

    if(SolarM >= 60) {
    SolarM = SolarM - 60;
    SolarH = SolarH + 1;
    }

    if(SolarH >= 24) {
    SolarH = SolarH - 24;
    }



  int utcHr = 12;
  int utcMn = 0;

  float STIH = SolarTime(; // Solar Time in hours

  Serial.println("=========================Start=========================");
  Serial.print("The UTC time is : ");
  Serial.print(utcHr);
  Serial.print(" : ");
  Serial.print(utcMn);
  Serial.println("   ");
  Serial.print("The longitute I am doing the calculation is : ");
  Serial.print(location);
  Serial.println(" ");
  Serial.print("The solar time for this location is : ");
  Serial.print(SolarH);
  Serial.print(":");
  if (SolarM < 10) {
    Serial.print("0");
    Serial.println(SolarM);
  }
  if (SolarM >= 10) {
    Serial.println(SolarM);
  }
  Serial.print("The noon is at : ");
  Serial.print(degTTM);
  Serial.println("° longitude.");
  Serial.println("=========================End=========================");



  delay(5000);





}



float _eqOfTime (float x) {
  float _eot = 0;
  _eot = ( (0.258 * cos(radians(x))) - (7.416 * sin(radians(x))) - (3.648 * cos(radians(2 * x))) - (9.228 * sin(radians(2 * x))) );
  return _eot;  // equation of time returns in minutes

}

float _Angle (int N) {
  float _aA = 0;
  _aA = (360 * (N - 1) / 365.242);
  return _aA;
}




