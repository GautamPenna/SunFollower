int Pot0 = A0;
int Pot1 = A1;
int Pot2 = A2;


String Place_Name[]  = {"San Fransicso" , "Denver" , "Dallas" , "Boston" , " London" , "Berlin" , "Moscow" , "Hyderabad" , "Hong Kong" , "Tokyo" , "Brisbane" , "Wellingtion"} ;
float Month_Num[]    = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12};
float Date_Num[]     = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28};

#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define OLED 4
Adafruit_SSD1306 display(OLED);

#define NUMFLAKES 10
#define XPOS 0
#define YPOS 1
#define DELTAY 2


#define LOGO16_GLCD_HEIGHT 16 
#define LOGO16_GLCD_WIDTH  16

void setup() {

  Serial.begin(9600);
  pinMode(Pot0 , INPUT);
  pinMode(Pot1 , INPUT);
  pinMode(Pot2 , INPUT);


  display.begin(SSD1306_SWITCHCAPVCC, 0x3c);
  display.display();
  display.clearDisplay();

  #if (SSD1306_LCDHEIGHT != 64)
  #error("Height incorrect, please fix Adafruit_SSD1306.h!");
  #endif








}

void loop() {

  float longitude;
  float latitude;
  String Place;

  int Month = analogRead(Pot1);
  float Year = 2017;
  float Date = analogRead(Pot2);

  int utcHr = 12;
  int utcMn = 0;

  

  if (analogRead(Pot0) < 22) {
    String Place = "San Frasicso";
    float latitude = 37.7749;
    float longitude = -122.4194;
  } else if (22 < analogRead(Pot0) < 43) {
    String Place = "Denver";
    float latitude = 39.7392;
    float longitude = -104.9903;
  } else if (43 < analogRead(Pot0) < 64) {
    String Place = "Dallas";
    float latitude = 32.7767;
    float longitude = -96.7970;
  } else if (64 < analogRead(Pot0) < 85) {
    String Place = "Boston";
    float latitude = 42.3601;
    float longitude = 71.0589;
  } else if (85 < analogRead(Pot0) < 106) {
    String Place = "London";
    float latitude = 51.5074;
    float longitude = 0.1278;
  } else if (106 < analogRead(Pot0) < 127) {
    String Place = "Berlin";
    float latitude = 52.5200;
    float longitude = 13.4050;
  } else if (127 < analogRead(Pot0) < 148) {
    String Place = "Moscow";
    float latitude = 55.7558;
    float longitude = 37.6173;
  } else if (148 < analogRead(Pot0) < 169) {
    String Place = "Hyderabad";
    float latitude = 17.3850;
    float longitude = 78.4867;
  } else if (169 < analogRead(Pot0) < 190) {
    String Place = "Hong Kong";
    float latitude = 22.3964;
    float longitude = 114.1095;
  } else if (190 < analogRead(Pot0) < 211) {
    String Place = "Tokyo";
    float latitude = 35.6895;
    float longitude = 139.6917;
  } else if (211 < analogRead(Pot0) < 234) {
    String Place = "Brisbane";
    float latitude = 27.4698;
    float longitude = 153.0251;
  } else if (234 < analogRead(Pot0) < 255) {
    String Place = "Wellingtion";
    float latitude = -41.2865;
    float longitude = -174.7762;
  } else {
    Serial.println("This is impossible. Go to the hospital.");
  }



  int Nday = NDay(Year, Month, Date);  // decalring the NDay value so that I can get the Nth day form the function. This is how I declare and execute a function
  float aA = axialAngle(Nday); // This float value holds the value of the axial angle function by calling the funciton and giving the value needed
  float EOT = eot(aA); // This float holds the eot in a minute value
  float TotalHours = solar_time(Nday , longitude , utcHr , utcMn); // Calculating the Total_Hours from solar time
  int TotalMins = TotalHours * 60; // This process is to make it into minutes

  int SolarM = TotalMins % 60; // Calculating the solar minute

  int SolarH = TotalMins / 60; // Calculating the solar hour time

  float decAngle = dec_angle(Nday); // Calculating the declination angle given the NDay

  float HrAngle = hr_angle(TotalHours); // calculating the Hr angle from the Total Hours


  float AltAngle = alt_angle(decAngle , latitude , HrAngle); // Calculating the alitude angle from the decAngle , latitude , and the Hr Angle

  float AzAngle = az_angle(decAngle , HrAngle , latitude , AltAngle); // Calculating the Azimuth Angle given the decAngle , HrAngle , Latitude , and altitude

  if (SolarM < 0) { // Caculating the solar m time from the if conditional statements
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



  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setCursor(0,0);
  display.print("AZI : ");
  display.print(AzAngle);









  delay(1000);
  Serial.println ();

}

float axialAngle(int Nday) { // This is how I declared the axial angle function. I could have declared it as anything I wanted but because the value of axial angle is a float, it is declared as a float
  float aA = 360 * (Nday - 1) / 365.242; // I am declaring the value that will hold the axial angle's value. Axial angle is the angular distance from , with the sun as the center, from the position of the Earth on Jan 1 to the present day or the day it is working on.
  return aA; //  returns the value of the axial angle when the funciton is called
}

float eot(float aA) { // This is the Equation of tim funciton or known as the EOT

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

  float Degrees_Away = longitude - degTTM; // calculating the degrees away form the UTC time zone

  float minutes = int(Degrees_Away) % 15; // Minutes process
  float hours = ((Degrees_Away - minutes) / 15); // Hours process



  float clockTimeHours = 11 + hours; // clock time hours
  float clockTimeMinutes = 60 + minutes; // clok time minutes
  clockTimeMinutes = clockTimeMinutes / 60; // dividing the clock time monitues to get the time in hours
  float TotalHours1 = clockTimeMinutes + clockTimeHours;


  float eq = eot(axialAngle(Nday));
  eq = eq / 60;
  float TotalHours = TotalHours1 + eq; // total hours of solar time

  float TotalHoursM = TotalHours * 60; // The rest of the solar time funciton code is to calculate the solar hour and minute
  int SolarM = int (TotalHoursM) % 60;
  int SolarH = ((TotalHoursM - SolarM) / 60);

  if (SolarM >= 60) {
    SolarM = SolarM - 60;
    SolarH = SolarH + 1; // This function calculates the solar time from the clock time and coordiantes
  }

  if (SolarH >= 24) {
    SolarH = SolarH - 24;
  }

  return TotalHours; // returning the solar time in hours.

}



float dec_angle(int Nday) { // declaring the declination angle funciton
  //float decAngle = -23.45 * cos(radians(0.98630136986 * (Nday + 10))); // Got this from http://www.pveducation.org/pvcdrom/properties-of-sunlight/declination-angle
  float decAngle = degrees(asin(0.39795 * cos(radians(0.98563 * (Nday - 173))))); // both formulas give you the same output
  return decAngle; // returing the decAngle as a float
}


float hr_angle(float SHT) { // decalring the hr_angle function
  float HrAngle = 15 * (SHT - 12); // calculating the HrAngle function
  return HrAngle; // returning the HrAngle function
}

float alt_angle(float decAngle , float latitude , float HrAngle) { // calculating the altitude angle function and giving it the values of decAngle and the rest of the function values
  float Alt_Angle = 0; // delcaring the Alt_Angle as a 0
  Alt_Angle = degrees(asin( (sin(radians(decAngle)) * sin(radians(latitude))) + (cos(radians(decAngle)) * cos(radians(HrAngle)) * cos(radians(latitude))) )); // Got this formula from powerformthesun.net and eq 3-17.
  return Alt_Angle; // returning the Alt_Angle function
}

float az_angle(float decAngle , float HrAngle , float Lat , float altAngle) { // calculting the azimuth angle function , the most important
  float azimuth_angle;

  float Aazimuth_angle = degrees(acos((sin(radians(decAngle)) * cos(radians(Lat)) - cos(radians(decAngle)) * cos(radians(HrAngle)) * sin(radians(Lat))) / cos(radians(altAngle)))); // calculating the azimuth angle prime
  if (sin(radians(HrAngle)) > 0) { // the bottom two are if conditional statements to calculating the azimuth angle for different conditions
    azimuth_angle = 360 - Aazimuth_angle;
  } else {

  } azimuth_angle = Aazimuth_angle; // declaring the output if the this conditional statement is true

  return azimuth_angle; // returning the azimiuth angle function
}

float NDay(int Year , int Month , int Day) { // the rest of the function is for the Nday function

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

  return Nday; // return the Nday function

}





