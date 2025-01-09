float RandomPLa[] = {40.7128,  32.7767,    39.7392,     37.7749,     21.3069};
float RandomPLo[] = {-74.0059 , -96.7970 , -104.9903 ,  -122.4194 ,  -157.8583};
String PlaceNames[]  = {"New York     ", "Dallas       ", "Denver       ", "San Francisco", "Honolulu     "};





void setup() {

  Serial.begin(115200);
  Serial.println("===================================");
  Serial.println("Location  |  NDay  |  Date  |  Time  |  Latitude  |  Longitude  |  EOT  |  aA  |  Declination Angle  |  Hr  |  Alt  |  Az  |  SolarH  |  SolarM  |  Hr");


}

void loop() {

  int year  = 2017;  //random(2008 , 2018);
  int month = 10;    //random(1, 12);
  int day   = 1;     //random(1, 28);
  int utcHr = 12;    //random(9, 18);
  int utcMn = 0;     //random(10, 58);


  for (int k = 0; k < 5; k++) {
    float longitude = RandomPLo[k];
    float latitude  = RandomPLa[k];

    int Nday = NDay(year, month, day);
    float aA = axialAngle(Nday);
    float EOT = eot(aA);
    float TotalHours = solar_time(Nday , longitude , utcHr , utcMn);
    int TotalMins = TotalHours * 60;

    //    int SolarM = int(TotalHours) % 60;
    //    int SolarH = (TotalHours - SolarM) / 60;


    int SolarM = TotalMins % 60;

    int SolarH = TotalMins / 60;





    float decAngle = dec_angle(Nday);

    float HrAngle = hr_angle(TotalHours);

    float AltAngle = alt_angle(decAngle , latitude , HrAngle);

    float AzAngle = az_angle(decAngle , HrAngle , latitude , AltAngle);

    if (SolarM < 0) {
      SolarH = 23;
      SolarM = 60 - SolarM;
    }

    if (SolarM >= 60) {
      SolarM = SolarM - 60;
      SolarH = SolarH + 1;
    }

    if (SolarH >= 24) {
      SolarH = SolarH - 24;
    }



    Serial.print (PlaceNames[k]);
    Serial.print ("  ");
    Serial.print(Nday);
    Serial.print("  ");
    Serial.print(month);
    Serial.print("/");
    Serial.print(day);
    Serial.print("/");
    Serial.print(year);
    Serial.print("  ");
    Serial.print(utcHr);
    Serial.print(":");
    Serial.print(utcMn);

    if (latitude < 10) {
      Serial.print("       ");
      Serial.print(latitude);
    } else if (10 < latitude < 100) {
      Serial.print("      ");
      Serial.print(latitude);
    } else { // if (latitude > 100)
      Serial.print("     ");
      Serial.print(latitude);
    }

    // Beginning of longitude

    if (longitude < -100) {
      Serial.print("       ");
      Serial.print(longitude);
    } else if (-100 < longitude < -10) {
      Serial.print("     ");
      Serial.print(longitude);
    } else if (-10 < longitude < 0) {
      Serial.print("       ");
      Serial.print(longitude);
    } else if (0 < longitude < 10) {
      Serial.print("        ");
      Serial.print(longitude);
    } else if (10 < longitude < 100) {
      Serial.print("       ");
      Serial.print(longitude);
    } else { // if (longitude > 100)
      //Serial.print("      ");
      Serial.print(longitude);
    }


    Serial.print("       ");
    Serial.print(EOT);
    Serial.print("  ");
    Serial.print(aA);
    Serial.print("        ");
    Serial.print(decAngle);
    Serial.print("        ");
    Serial.print(HrAngle);
    Serial.print("     ");
    Serial.print(AltAngle);
    Serial.print("  ");
    Serial.print(AzAngle);
    Serial.print("  ");
    Serial.print(SolarH);
    Serial.print("  ");
    if (SolarM < 10) {
      Serial.print("0");
      Serial.println(SolarM);
    } else {
      Serial.println(SolarM);
    }







  }

  delay(1000);
  Serial.println ();
}


float NDay(int Year , int Month , int Day) {

  int Nday;

  if (Month == 1) {
    Nday = Day;
  }

  if (Month == 2) {
    Nday = Day + 31;
  }

  if (Month == 3) {
    Nday = Day + 59;

    if (Year % 4 == 0) {
      Nday = Day + 60;
    }

    if (Year % 100 == 0) {
      Nday = Day + 59;
    }

    if (Year % 400 == 0) {
      Nday = Day + 60;
    }

  }

  if (Month == 4) {
    Nday = Day + 90;

    if (Year % 100 == 0) {
      Nday = Day + 90;
    }

    if (Year % 4 == 0) {
      Nday = Day + 91;
    }

    if (Year % 400 == 0) {
      Nday = Day + 91;
    }
  }

  if (Month == 5) {
    Nday = Day + 120;

    if (Year % 100 == 0) {
      Nday = Day + 120;
    }

    if (Year % 4 == 0) {
      Nday = Day + 121;
    }

    if (Year % 400 == 0) {
      Nday = Day + 121;
    }
  }

  if (Month == 6) {
    Nday = Day + 151;

    if (Year % 100 == 0) {
      Nday = Day + 151;
    }

    if (Year % 4 == 0) {
      Nday = Day + 152;
    }

    if (Year % 400 == 0) {
      Nday = Day + 152;
    }
  }

  if (Month == 7) {
    Nday = Day + 181;

    if (Year % 100 == 0) {
      Nday = Day + 181;
    }

    if (Year % 4 == 0) {
      Nday = Day + 182;
    }

    if (Year % 400 == 0) {
      Nday = Day + 182;
    }
  }

  if (Month == 8) {
    Nday = Day + 212;

    if (Year % 100 == 0) {
      Nday = Day + 212;
    }
    if (Year % 4 == 0) {
      Nday = Day + 213;
    }

    if (Year % 400 == 0) {
      Nday = Day + 213;
    }
  }

  if (Month == 9) {
    Nday = Day + 243;

    if (Year % 100 == 0) {
      Nday = Day + 243;
    }

    if (Year % 4 == 0) {
      Nday = Day + 244;
    }

    if (Year % 400 == 0) {
      Nday = Day + 244;
    }
  }

  if (Month == 10) {
    Nday = Day + 273;

    if (Year % 100 == 0) {
      Nday = Day + 273;
    }

    if (Year % 4 == 0) {
      Nday = Day + 274;
    }

    if (Year % 400 == 0) {
      Nday = Day + 274;
    }
  }


  if (Month == 11) {
    Nday = Day + 304;

    if (Year % 100 == 0) {
      Nday = Day + 304;
    }

    if (Year % 4 == 0) {
      Nday = Day + 305;
    }

    if (Year % 400 == 0) {
      Nday = Day + 305;

    }


  }

  if (Month == 12) {
    Nday = Day + 334;

    if (Year % 100 == 0) {
      Nday = Day + 334;
    }

    if (Year % 4 == 0) {
      Nday = Day + 335;
    }

    if (Year % 400 == 0) {
      Nday = Day + 335;
    }
  }

  return Nday;

}


float axialAngle(int Nday) {
  float aA = 360 * (Nday - 1) / 365.242;
  return aA;
}

float eot(float aA) {

  float EOT = 0;
  EOT = ( (0.258 * cos(radians(aA))) - (7.416 * sin(radians(aA))) - (3.648 * cos(radians(2 * aA))) - (9.228 * sin(radians(2 * aA))) );
  return EOT;  // equation of time returns in minutes


}

float solar_time(int Nday , float longitude , int utcHr , int utcMn) {

  //  int year = 2017;  I spent 1 hour looking for this
  //  int month = 9;
  //  int day = 30;



  //  utcHr = 12; I spent another hour looking for this
  //  utcMn = 0;



  int TZTSolarNoonH = utcHr - 12; // Time Zones to solar Noon Hour
  int TZTSolarNoonM = utcMn - 00; // Time Zones to solar noon Minutes

  float degTTM = (-1) * (15 * TZTSolarNoonH + float (TZTSolarNoonM / 4)); // How many degrees it is away at

  float Degrees_Away = longitude - degTTM;

  float minutes = int(Degrees_Away) % 15; // Minutes process
  float hours = ((Degrees_Away - minutes) / 15);



  float clockTimeHours = 11 + hours;
  float clockTimeMinutes = 60 + minutes;
  clockTimeMinutes = clockTimeMinutes / 60;
  float TotalHours1 = clockTimeMinutes + clockTimeHours;


  float eq = eot(axialAngle(Nday));
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

  return TotalHours;

}



float dec_angle(int Nday) {
  //float decAngle = -23.45 * cos(radians(0.98630136986 * (Nday + 10))); // Got this from http://www.pveducation.org/pvcdrom/properties-of-sunlight/declination-angle
  float decAngle = degrees(asin(0.39795 * cos(radians(0.98563 * (Nday - 173))))); // both formulas give you the same output
  return decAngle;
}


float hr_angle(float SHT) {
  float HrAngle = 15 * (SHT - 12);
  return HrAngle;
}

float alt_angle(float decAngle , float latitude , float HrAngle) {
  float Alt_Angle = 0;
  Alt_Angle = degrees(asin( (sin(radians(decAngle)) * sin(radians(latitude))) + (cos(radians(decAngle)) * cos(radians(HrAngle)) * cos(radians(latitude))) )); // Got this formula from powerformthesun.net and eq 3-17.
  return Alt_Angle;
}

float az_angle(float decAngle , float HrAngle , float Lat , float altAngle) {
  float azimuth_angle;
  float Aazimuth_angle = degrees(acos(radians(sin(radians(decAngle)) * cos(radians(Lat)) - cos(radians(decAngle)) * cos(radians(HrAngle)) * sin(radians(Lat))) / cos(radians(altAngle))));
  if (sin(radians(HrAngle)) > 0) {
    azimuth_angle = 360 - Aazimuth_angle;
  } else {
    azimuth_angle = Aazimuth_angle;
  }
  return azimuth_angle;
}

