int Month;
int Date;
int Year;


float latitude;

int location;

float decAngle;

float AltAngle;




void setup() {

  Serial.begin(9600);
  

}

void loop() {


}

float NDay(int Nday) {

  if (Month == 1) {
    Nday = Date;
  }

  if (Month == 2) {
    Nday = Date + 31;
  }
  
  if (Month == 3) {
    Nday = Date + 59;

    if(Year%4 == 0) {
      Nday = Date + 60;
    }
    
    if(Year%100 == 0) {
      Nday = Date + 59;
    }

    if(Year%400 == 0) {
      Nday = Date + 60;
    }
    
  }

  if (Month == 4) {
    Nday = Date + 90;

    if(Year%100 == 0) {
      Nday = Date + 90;
    }

    if(Year%4 == 0) {
      Nday = Date + 91;
    }

    if(Year%400 == 0) {
      Nday = Date + 91;
    }
  }

  if (Month == 5) {
   Nday = Date + 120;
   
   if(Year%100 == 0) {
      Nday = Date + 120;
    }

    if(Year%4 == 0) {
      Nday = Date + 121;
    }

    if(Year%400 == 0) {
      Nday = Date + 121;
    }
  }

  if (Month == 6) {
    Nday = Date + 151;
    
    if(Year%100 == 0) {
      Nday = Date + 151;
    }

    if(Year%4 == 0) {
      Nday = Date + 152;
    }

    if(Year%400 == 0) {
      Nday = Date + 152;
    }
  }
  
  if (Month == 7) {
    Nday = Date + 181;
    
    if(Year%100 == 0) {
      Nday = Date + 181;
    }

    if(Year%4 == 0) {
      Nday = Date + 182;
    }

    if(Year%400 == 0) {
      Nday = Date + 182;
    }
  }

  if (Month == 8) {
   Nday = Date + 212;
   
   if(Year%100 == 0) {
      Nday = Date + 212;
   }
    if(Year%4 == 0) {
      Nday = Date + 213;
    }

    if(Year%400 == 0) {
      Nday = Date + 213;
    }
  }

  if (Month == 9) {
    Nday = Date + 243;
    
    if(Year%100 == 0) {
      Nday = Date + 243;
    }

    if(Year%4 == 0) {
      Nday = Date + 244;
    }

    if(Year%400 == 0) {
      Nday = Date + 244;
    }
  }

  if (Month == 10) {
    Nday = Date + 273;
    
    if(Year%100 == 0) {
      Nday = Date + 273;
    }

    if(Year%4 == 0) {
      Nday = Date + 274;
    }

    if(Year%400 == 0) {
      Nday = Date + 274;
    }
  }
  

  if (Month == 11) {
    Nday = Date + 304;
    
    if(Year%100 == 0) {
      Nday = Date + 304;
    }

    if(Year%4 == 0) {
      Nday = Date + 305;
    }

    if(Year%400 == 0) {
      Nday = Date + 305;
    
  }


  }

  if (Month == 12) {
    Nday = Date + 334;
    
    if(Year%100 == 0) {
      Nday = Date + 334;
    }

    if(Year % 4 == 0) {
      Nday = Date + 335;
    }

    if(Year%400 == 0) {
      Nday = Date + 335;
    }
  }
  Serial.print("This is my Date : ");
  Serial.print(Month);
  Serial.print("/");
  Serial.print(Date);
  Serial.print("/");
  Serial.print(Year);
  
  Serial.print("This is the day number from January 1st : ");
  Serial.println(Nday);
  
  delay(30);

  return Nday;

}

float Solar_Time() {

  int utcHr = 12;
  int utcMn = 0;

  float location;

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

  
  float eq = _eqOfTime(_Angle(NDay(Date)));
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
  if(SolarM < 10) {
    Serial.print("0");
    Serial.println(SolarM);
  }
  if(SolarM >= 10) {
    Serial.println(SolarM);
  }
  Serial.print("The noon is at : ");
  Serial.print(degTTM);
  Serial.println("° longitude.");
  Serial.println("=========================End=========================");
  
   

  delay(5000);

  return TotalHours;
  
}


float _eqOfTime (float x) {
  float _eot=0;
  _eot = ( (0.258*cos(radians(x)))-(7.416*sin(radians(x)))-(3.648*cos(radians(2*x)))-(9.228*sin(radians(2*x))) );
  return _eot;  // equation of time returns in minutes

}

float _Angle (int N) {
  float _aA = 0;
  _aA = (360*(N-1) / 365.242);
  return _aA;
}

float dec_angle() {

  float decAngle = 0;
  decAngle = -23.45*cos(radians(0.98630136986*(NDay(Date) + 10))); // Got this from http://www.pveducation.org/pvcdrom/properties-of-sunlight/declination-angle
  Serial.print("This is the day number : ");
  Serial.print(NDay(Date));
  Serial.print(" ");
  Serial.print("This is the declination angle for this day : ");
  Serial.println(decAngle);
  return decAngle;
    
}

float Alt_Angle() {

  float AltAngle =0;
  AltAngle = 90 - decAngle + latitude;
  Serial.print("This is the Alitutde Angle : ");
  Serial.println(AltAngle);
  return AltAngle;
}

float AzAngle() {

  float azimuth_angle =0;
  azimuth_angle = degrees(acos(radians(sin(radians(dec_angle))*cos(radians(latitude)) - cos(radians(dec_angle))*cos(radians(HrAngle))*sin(radians(latitude)))/cos(radians(AltAngle))));
  
  Serial.print("This is the azimuth angle : ");
  Serial.println(azimuth_angle);  
  return azimuth_angle;
  
}

float HrAngle() {

  float _hrA=0;
  
  _hrA = 15*(Solar_Time(location)) - 180;
  
  return _hrA;
  
}


  


