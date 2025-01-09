#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

const int locPot = A0;
const int monPot = A1;
const int dayPot = A2;


#define AZstepPin 6
#define AZdirPin 5

#define OLED 4
Adafruit_SSD1306 display(OLED);

#define NUMFLAKES 10
#define XPOS 0
#define YPOS 1
#define DELTAY 2

#define LOGO16_GLCD_HEIGHT 16
#define LOGO16_GLCD_WIDTH  16

int reset_pin = 7;

int dirPin = 8;
int stepPin = 9;


String location[] = {"Tokyo", "Hong Kong", "Hyderabad", "Tel Aviv", "Vienna", "London", "St. Johns", "New York", "Dallas", "Denver", "San Francisco", "Honolulu"};

int azMS1 = 3;

void setup() {

  Serial.begin(9600);
  display.begin(SSD1306_SWITCHCAPVCC, 0x3c);
  display.display();
  display.clearDisplay();

  pinMode(AZstepPin , OUTPUT);
  pinMode(AZdirPin , OUTPUT);
  pinMode(azMS1 , OUTPUT);

  pinMode(locPot, INPUT); pinMode(monPot, INPUT); pinMode(dayPot, INPUT);


#if (SSD1306_LCDHEIGHT != 64)
#error("Height incorrect, please fix Adafruit_SSD1306.h!");
#endif


}

void loop() {

  int year  = 2017;   // print out a test information so I can check on websites if my calculation are right for that day
  int month;     // print out a test information so I can check on websites if my calculation are right for that day
  int day;      // print out a test information so I can check on websites if my calculation are right for that day
  int utcHr;     // print out a test information so I can check on websites if my calculation are right for that day
  int utcMn;      // print out a test information so I can check on websites if my calculation are right for that day
  float latitude = 0; // print out a test information so I can check on websites if my calculation are right for that day
  float longitude = 0;// print out a test information so I can check on websites if my calculation are right for that day

  String Place;


  digitalWrite(dirPin , HIGH);
  // motor and lcd part


  // this is in half step mode so 0,9 degrees per step





  for (int y = 8; y <= 21; y++) { // this for loop loops for the 12 hours that there are

    int locIn = 0, monIn = 0, dayIn = 0;

    Serial.print(Place);

    locIn = analogRead(locPot); monIn = analogRead(monPot); dayIn = analogRead(dayPot);

    int k = 0, l = 0, m = 0;
    k = round((locIn) / 86);
    l = round((monIn) / 85);  if (l == 0) l = 1;
    m = round((dayIn) / 36);  if (m == 0) m = 1;

    // Serial.print(locIn); Serial.print(" "); Serial.print (monIn); Serial.print(" "); Serial.println(dayIn);

    Serial.print ("Location: "); Serial.print(Place);
    Serial.print (" Month: "); Serial.print(l);
    Serial.print (" Day: "); Serial.println(m);

    Place = location[k];

    if (Place == "Tokyo") {
      latitude = 35.6895;
      longitude = 139.6917;
    } else if (Place == "Hong Kong") {
      latitude = 22.3964;
      longitude = 114.1095;
    } else if (Place == "Hyderabad") {
      latitude = 17.3850;
      longitude = 78.4867;
    } else if (Place == "Tel Aviv") {
      latitude = 32.0853;
      longitude = 34.7818;
    } else if (Place == "Vienna") {
      latitude = 48.2082;
      longitude = 16.3738;
    } else if (Place == "London") {
      latitude = 51.5074;
      longitude = -0.1278;
    } else if (Place == "St.Johns") {
      latitude = 47.5615;
      longitude = -52.7126;
    } else if (Place == "New York") {
      latitude = 40.7134;
      longitude = -74.0055;
    } else if (Place == "Dallas") {
      latitude = 32.7767;
      longitude = -96.7970;
    } else if (Place == "Denver") {
      latitude = 39.7392;
      longitude = -104.9903;
    } else if (Place == "San Francisco") {
      latitude = 37.7749;
      longitude = -122.4194;
    } else if (Place == "Honolulu") {
      latitude = 21.3069;
      longitude = -157.8583;
    }

    month = l;
    day = m;




    int Nday = NDay(year, month, day);  // decalring the NDay value so that I can get the Nth day form the function. This is how I declare and execute a function
    float aA = axialAngle(Nday); // This float value holds the value of the axial angle function by calling the funciton and giving the value needed
    float EOT = eot(aA); // This float holds the eot in a minute value
    float TotalHours = solar_time(Nday , longitude , utcHr , utcMn); // Calculating the Total_Hours from solar time
    int TotalMins = TotalHours * 60; // This process is to make it into minutes
    int SolarM = TotalMins % 60; // Calculating the solar minute
    int SolarH = TotalMins / 60; // Calculatin the solar hour time
    float decAngle = dec_angle(Nday); // Calculating the declination angle given the NDay
    float HrAngle = hr_angle(TotalHours); // calculating the Hr angle from the Total Hours
    float AltAngle = alt_angle(decAngle , latitude , HrAngle); // Calculating the alitude angle from the decAngle , latitude , and the Hr Angle
    float AzAngle = az_angle(decAngle , HrAngle , latitude , AltAngle); // Calculating the Azimuth Angle given the decAngle , HrAngle , Latitude , and altitude
    float lcHr = lctConH(utcHr , longitude);
    float lcMn = utcMn;

    if (SolarM < 0) { // Caculating the solar m time from the if conditional statements
      SolarH = 23;
      SolarM = 60 - SolarM;
    } else if (SolarM >= 60) {
      SolarM = SolarM - 60;
      SolarH = SolarH + 1;
    }
    if (SolarH >= 24) {
      SolarH = SolarH - 24;
    }

    Serial.print(day);

   // float z = AzAngle / TIB;
  //  z = z / 0.9; // move per rotation//how many rotations
   // for (int y = 0; y <= z; y++) {
    //  digitalWrite(stepPin , HIGH);
     // delay(5000);
    

    display.clearDisplay();
    display.setTextSize(2);
    display.setTextColor(WHITE);
    display.setCursor(0, 0);
    display.write(15);
    display.print(" Follower");
    display.setCursor(0, 16);
    display.print("Azimuth : ");
    display.setCursor(0, 32);
    display.setTextSize(3);
    display.print(AzAngle);
    display.display();

    delay(2000);

    display.clearDisplay();
    display.setTextSize(2);
    display.setCursor(0, 0);
    display.write(15);
    display.print(" Follower");
    display.setCursor(0, 16);
    display.print("Altitude : ");
    display.setCursor(0, 32);
    display.setTextSize(3);
    display.print(AltAngle);
    display.display();

    delay(2000);

    display.clearDisplay();
    display.setTextSize(2);
    display.setCursor(0, 0);
    display.write(15);
    display.print(" Follower");
    display.setCursor(0, 16);
    display.print(Place);
    display.print("  Solar Time");
    display.print(SolarH);
    display.print(":");
    display.print(SolarM);
    display.display();

    delay(2000);

    display.clearDisplay();
    display.setTextSize(2);
    display.setCursor(0, 0);
    display.write(15);
    display.print(" Follower");
    display.setCursor(0, 26);
    display.println(latitude);
    display.println(longitude);
    display.display();

    delay(2000);

    utcHr = utcHr + 1;
    lcHr = lcHr + 1;


  }









float lctConH(int utcHr , float longitude) {

  int TZA = int(longitude / 15);
  int lcHr = utcHr + TZA;
  return lcHr;

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
  } else if (Month == 2) {
    Nday = Day + 31;
  } else if (Month == 3) {
    Nday = Day + 59;

    if (Year % 4 == 0) {
      Nday = Day + 60;
    } else if (Year % 100 == 0) {
      Nday = Day + 59;
    } else if (Year % 400 == 0) {
      Nday = Day + 60;
    }

  } else if (Month == 4) {
    Nday = Day + 90;

    if (Year % 100 == 0) {
      Nday = Day + 90;
    } else if (Year % 4 == 0) {
      Nday = Day + 91;
    } else if (Year % 400 == 0) {
      Nday = Day + 91;
    }
  } else if (Month == 5) {
    Nday = Day + 120;

    if (Year % 100 == 0) {
      Nday = Day + 120;
    } else if (Year % 4 == 0) {
      Nday = Day + 121;
    } else if (Year % 400 == 0) {
      Nday = Day + 121;
    }
  } else if (Month == 6) {
    Nday = Day + 151;

    if (Year % 100 == 0) {
      Nday = Day + 151;
    } else if (Year % 4 == 0) {
      Nday = Day + 152;
    } else if (Year % 400 == 0) {
      Nday = Day + 152;
    }
  } else if (Month == 7) {
    Nday = Day + 181;

    if (Year % 100 == 0) {
      Nday = Day + 181;
    } else if (Year % 4 == 0) {
      Nday = Day + 182;
    } else if (Year % 400 == 0) {
      Nday = Day + 182;
    }
  } else if (Month == 8) {
    Nday = Day + 212;

    if (Year % 100 == 0) {
      Nday = Day + 212;
    } else if (Year % 4 == 0) {
      Nday = Day + 213;
    } else if (Year % 400 == 0) {
      Nday = Day + 213;
    }
  } else if (Month == 9) {
    Nday = Day + 243;

    if (Year % 100 == 0) {
      Nday = Day + 243;
    } else if (Year % 4 == 0) {
      Nday = Day + 244;
    } else if (Year % 400 == 0) {
      Nday = Day + 244;
    } 
  } else if (Month == 10) {
    Nday = Day + 273;

    if (Year % 100 == 0) {
      Nday = Day + 273;
    } else if (Year % 4 == 0) {
      Nday = Day + 274;
    } else if (Year % 400 == 0) {
      Nday = Day + 274;
    }
  } else if (Month == 11) {
    Nday = Day + 304;

    if (Year % 100 == 0) {
      Nday = Day + 304;
    } else if (Year % 4 == 0) {
      Nday = Day + 305;
    } else if (Year % 400 == 0) {
      Nday = Day + 305;

    }


  }else if (Month == 12) {
    Nday = Day + 334;

    if (Year % 100 == 0) {
      Nday = Day + 334;
    } else if (Year % 4 == 0) {
      Nday = Day + 335;
    } else if (Year % 400 == 0) {
      Nday = Day + 335;
    }
  }

  return Nday; // return the Nday function

}


