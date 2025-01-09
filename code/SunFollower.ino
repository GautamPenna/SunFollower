/*
    1. Potentiometers are not reading the values.
    2. Motor out of control
    3. Check the wires and connections.
    4. Switches are not working
 *  */

#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>


#define LOCPOT A0
#define MONPOT A1
#define DAYPOT A2

#define ALT_STEP_PIN 6
#define ALT_DIR_PIN 7
#define ALT_MINA 0
#define ALT_MAXA 90
#define ALT_MINS 0
#define ALT_MAXS 100
#define ALT_HOMEP 12

#define AZI_STEP_PIN 8
#define AZI_DIR_PIN 9
#define AZI_MINA 75
#define AZI_MAXA 275
#define AZI_MINS 83
#define AZI_MAXS 306
#define AZI_HOMEP 13


#define OLED 4
Adafruit_SSD1306 display(OLED);


#define LOGO16_GLCD_HEIGHT 16
#define LOGO16_GLCD_WIDTH  16


#define REALMODE 2
#define DEMOMODE 3
#define DEMOSET 4
#define DEMORUN 5
#define RESETBUTTON 52

#if (SSD1306_LCDHEIGHT != 64)
#error("Height incorrect, please fix Adafruit_SSD1306.h!");
#endif




String location[] = {"Tokyo", "Hong Kong", "Hyderabad", "Tel Aviv", "Vienna", "London", "St. Johns", "New York", "Dallas", "Denver", "San Fran", "Honolulu"};

int altStep = 0;
int aziStep = 0;



void setup() {

  Serial.begin(9600);
  Serial.println("Starting the Program");

  display.begin(SSD1306_SWITCHCAPVCC, 0x3c);
  //display.display();
  display.clearDisplay();

  pinMode(LOCPOT, INPUT);
  pinMode(MONPOT, INPUT);
  pinMode(DAYPOT, INPUT);

  pinMode(ALT_STEP_PIN , OUTPUT);
  pinMode(ALT_DIR_PIN , OUTPUT);
  pinMode(ALT_HOMEP , INPUT_PULLUP);

  pinMode(AZI_STEP_PIN , OUTPUT);
  pinMode(AZI_DIR_PIN , OUTPUT);
  pinMode(AZI_HOMEP , INPUT_PULLUP);

  pinMode(DEMOSET, INPUT_PULLUP);
  pinMode(REALMODE, INPUT_PULLUP);
  pinMode(DEMOMODE, INPUT_PULLUP);
  pinMode(DEMORUN, INPUT_PULLUP);

  //pinMode(RESETBUTTON, INPUT);

}

void loop() {

  int year  = 2017;
  int month = 1;
  int day = 1;
  int utcHr = 0;
  int utcMn = 0;

  // We will set OMS Coordinates as default 33.005803,-96.6528103
  float latitude = 33.005803;
  float longitude = -96.6528103;

  String Place = "Otto Middle School";

  // motor and lcd part

  int inRealMode = digitalRead(REALMODE); //if True, Real Mode - else Demo Mode
  int inDemoMode = digitalRead(DEMOMODE);
  int inDemoSetMode = digitalRead(DEMOSET); //if True DemoSet Mode, else Run Demo
  int inDemoRunMode = digitalRead(DEMORUN);

  Serial.println("Past Reading the potentiometers");

  if (inRealMode) {
    Serial.println("In Real Mode");
  }
  else if (inDemoMode) {

    Serial.println("In Demo Mode");
    gotBacktoAltHome();
    gotBacktoAziHome();

    while (inDemoSetMode) {

      int locIn = analogRead(LOCPOT);
      int monIn = analogRead(MONPOT);
      int dayIn = analogRead(DAYPOT);

      int locRead = 0;
      int monRead = 0;
      int dayRead = 0;

      locRead = round((locIn) / 85);
      monRead = round((monIn) / 85.25);
      dayRead = round((dayIn) / 36);

      if (monRead == 0) monRead = 1;
      if (dayRead == 0) dayRead = 1;
      if (locRead == 12) locRead = 11;

      monRead = 13 - monRead;
      dayRead = 29 - dayRead;

      Place = location[locRead];

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
      } else if (Place == "San Fran") {
        latitude = 37.7749;
        longitude = -122.4194;
      } else if (Place == "Honolulu") {
        latitude = 21.3069;
        longitude = -157.8583;
      } else {
        latitude = 33.005803;
        longitude = -96.6528103;
      }

      month = monRead;
      day =  dayRead;

      Serial.print (" Location: "); Serial.print(Place);
      Serial.print (" Month: "); Serial.print(monRead);
      Serial.print (" Day: "); Serial.println(dayRead);


      display.clearDisplay();
      display.setTextSize(2);
      display.setTextColor(WHITE);
      display.setCursor(0, 0);
      display.println("Place:");
      display.println(Place);
      display.print("Month:");
      display.println(month);
      display.print("Day:");
      display.print(day);
      display.display();

      Serial.print(">> In Demo SET MODE: Read the Place, Month and Date");

    }

    while (inDemoRunMode) {

      int locIn = analogRead(LOCPOT);
      int monIn = analogRead(MONPOT);
      int dayIn = analogRead(DAYPOT);

      int locRead = 0;
      int monRead = 0;
      int dayRead = 0;

      locRead = round((locIn) / 85);
      monRead = round((monIn) / 85.25);
      dayRead = round((dayIn) / 36);

      if (monRead == 0) monRead = 1;
      if (dayRead == 0) dayRead = 1;
      if (locRead == 12) locRead = 11;

      monRead = 13 - monRead;
      dayRead = 29 - dayRead;

      Place = location[locRead];

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
      } else if (Place == "San Fran") {
        latitude = 37.7749;
        longitude = -122.4194;
      } else if (Place == "Honolulu") {
        latitude = 21.3069;
        longitude = -157.8583;
      } else {
        latitude = 33.005803;
        longitude = -96.6528103;
      }

      month = monRead;
      day =  dayRead;

      Serial.print (" Location: "); Serial.print(Place);
      Serial.print (" Month: "); Serial.print(monRead);
      Serial.print (" Day: "); Serial.println(dayRead);


      display.clearDisplay();
      display.setTextSize(2);
      display.setTextColor(WHITE);
      display.setCursor(0, 0);
      display.println("Place:");
      display.println(Place);
      display.print("Month:");
      display.println(month);
      display.print("Day:");
      display.print(day);
      display.display();

      Serial.println(">>> In Demo RUN MODE: Past Reading the potentiometers");

      // We will run the Demo from 8AM to 8PM

      float preAlt = 0;
      float preAzi = 0;

      int resetZeroAlt = 0; // a variable for a loop to make it do it only once
      int resetZeroAzi = 0; // a variable for a loop to make it do it only once

      for (int locH = 8; locH < 21; locH++) {

        int Nday = NDay(year, month, day);  // decalring the NDay value so that I can get the Nth day form the function. This is how I declare and execute a function
        float aA = axialAngle(Nday); // This float value holds the value of the axial angle function by calling the funciton and giving the value needed
        float EOT = eot(aA); // This float holds the eot in a minute value

        int lcMn = 0;
        float utcMins = 0;

        utcMins = utcTime(longitude , locH , lcMn);
        int utcHr = int(utcMins / 60);
        int utcMn = int(utcMins) % 60;

        int solarMins = 0;
        solarMins = solar_time(longitude , utcHr , utcMn); // solar_time returns hours, since day started
        //int solarMins = solarHours * 60; // This process is to make it into minutes
        int SolarM = int(solarMins) % 60; // Calculating the solar minute
        int SolarH = int(solarMins / 60); // Calculatin the solar hour time

        float decAngle = dec_angle(Nday); // Calculating the declination angle given the NDay
        float HrAngle = hr_angle(solarMins / 60); // calculating the Hr angle from the Total Hours
        float AltAngle = alt_angle(decAngle , latitude , HrAngle); // Calculating the alitude angle from the decAngle , latitude , and the Hr Angle
        float AziAngle = az_angle(decAngle , HrAngle , latitude , AltAngle); // Calculating the Azimuth Angle given the decAngle , HrAngle , Latitude , and altitude

        Serial.print(" Location: "); Serial.print(Place); Serial.print(" Latitude: "); Serial.print(latitude); Serial.print(" Longitude: "); Serial.print(longitude);
        Serial.print(" NDay: "); Serial.print(Nday); Serial.print(" Axial Angle: "); Serial.print(aA); Serial.print(" Eq of Time: "); Serial.print(EOT);
        Serial.print(" UTC Time: "); Serial.print(utcHr); Serial.print(":"); Serial.print(utcMn);  Serial.print (" SolarTime: "); Serial.print(SolarH); Serial.print(" : "); Serial.print(SolarM);
        Serial.print(" Hr Angle: "); Serial.print(HrAngle); Serial.print(" Dec Angle: "); Serial.print(decAngle);
        Serial.print(" Local Time: "); Serial.print(locH); Serial.print (":"); Serial.print("00");
        Serial.print(" Altitude: "); Serial.print(AltAngle); Serial.print (" Azimuth: "); Serial.println(AziAngle);

        //Moving the Altitude Motor
        if (AltAngle > ALT_MINA && AltAngle < ALT_MAXA ) {
          while (resetZeroAlt == 0) {
            //Serial.println("Doing it the first time");
            preAlt = AltAngle;
            int AltAngleSteps = AltAngle / 0.9;
            for (int a = 0; a < AltAngleSteps; a++) {
              goToCurAltPos();
            }
            resetZeroAlt = 1;  // We just run it once a day
            //Serial.println("Past doing the loop the first time.");
          }

          if ( AltAngle < preAlt ) {
            int AltAngleSteps = (preAlt - AltAngle) / 0.9;
            for (int a = 0; a < AltAngleSteps; a++) {
              goToCurAltPosCCW();
            }

          } else {
            int AltAngleSteps = (AltAngle - preAlt) / 0.9;
            for (int a = 0; a < AltAngleSteps; a++) {
              goToCurAltPos();
            }
          }

          preAlt = AltAngle;

        } else {
          Serial.println("ALT -ve angle Motor will not move");
        }

        //Moving the Azimuth Motor
        if (AziAngle > AZI_MINA && AziAngle < AZI_MAXA ) {
          while (resetZeroAzi == 0) {
            //Serial.println("Doing it the first time");
            preAzi = AziAngle;
            int AziAngleSteps = (AziAngle / 0.9) - AZI_MINS ;
            for (int a = 0; a < AziAngleSteps; a++) {
              goToCurAziPos();
            }
            resetZeroAzi = 1;  // We just run it once a day
            //Serial.println("Past doing the loop the first time.");
          }


          int AziAngleSteps = (AziAngle - preAzi) / 0.9;
          for (int a = 0; a < AziAngleSteps; a++) {
            goToCurAziPos();
          }

          preAzi = AziAngle;

        } else {
          Serial.println("AZI angle over 270º Motor will not move");
        }

        // Lets Start the Display on the Screen

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
        display.print(AziAngle);
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
        display.println(Place);
        display.print("Time: ");
        display.print(locH);
        display.print(":");
        display.print("00");
        display.display();

        delay(2000);

        display.clearDisplay();
        display.setTextSize(2);
        display.setCursor(0, 0);
        display.write(15);
        display.print(" Follower");
        display.setCursor(0, 16);
        display.println("Lat & Lon:");
        display.println(latitude);
        display.println(longitude);
        display.display();

        delay(2000);

      } //End of the For Hour Loop - Demo RUN MODE
      // Run the Motors back to Home
      gotBacktoAltHome();
      gotBacktoAziHome();

    } // End of Demo Run Mode
  } else {
    while (1) {
      Serial.println("This should never happen, fix it!!!");
    }
  }


}

/////////////////////////////////////////////////////////////////////////////////////////////////////////BEWARE , FUNCTIONS ARE BELOW/////////////////////////////////////////////////////////////////////////////////////////////////////////


void gotBacktoAltHome() {

  while (digitalRead(ALT_HOMEP)) {
    digitalWrite(ALT_DIR_PIN , HIGH);
    digitalWrite(ALT_STEP_PIN , HIGH);
    delay(10);
    digitalWrite(ALT_STEP_PIN , LOW);
    delay(10);
  }

  while (!digitalRead(ALT_HOMEP)) { // move one step
    digitalWrite(ALT_DIR_PIN , LOW);
    digitalWrite(ALT_STEP_PIN , HIGH);
    delay(10);
    digitalWrite(ALT_STEP_PIN , LOW);
    delay(10);
  }

  delay(1000);

  int altStep = ALT_MAXS + 15;
  Serial.print("This is the Max Steps : ");
  Serial.println(altStep);

  while (altStep > ALT_MINS) {
    digitalWrite(ALT_DIR_PIN , LOW);
    digitalWrite(ALT_STEP_PIN , HIGH);
    delay(10);
    digitalWrite(ALT_STEP_PIN , LOW);
    delay(10);
    altStep = altStep - 1;
  }

}

void goToCurAltPos() {
  digitalWrite(ALT_DIR_PIN , HIGH);
  digitalWrite(ALT_STEP_PIN , HIGH);
  delay(10);
  digitalWrite(ALT_STEP_PIN , LOW);
  delay(10);
}


void goToCurAltPosCCW() {
  digitalWrite(ALT_DIR_PIN , LOW);
  digitalWrite(ALT_STEP_PIN , HIGH);
  delay(10);
  digitalWrite(ALT_STEP_PIN , LOW);
  delay(10);
}

void gotBacktoAziHome() {

  while (digitalRead(AZI_HOMEP)) {
    digitalWrite(AZI_DIR_PIN , HIGH);
    digitalWrite(AZI_STEP_PIN , HIGH);
    delay(10);
    digitalWrite(AZI_STEP_PIN , LOW);
    delay(10);
  }

  while (!digitalRead(AZI_HOMEP)) { // move one step
    digitalWrite(AZI_DIR_PIN , LOW);
    digitalWrite(AZI_STEP_PIN , HIGH);
    delay(10);
    digitalWrite(AZI_STEP_PIN , LOW);
    delay(10);
  }

  delay(1000);

  int aziStep = AZI_MAXS + 9;
  Serial.print("This is the Max Steps : ");
  Serial.println(aziStep);

  while (aziStep > AZI_MINS) {
    digitalWrite(AZI_DIR_PIN , LOW);
    digitalWrite(AZI_STEP_PIN , HIGH);
    delay(10);
    digitalWrite(AZI_STEP_PIN , LOW);
    delay(10);
    aziStep = aziStep - 1;
  }

}

void goToCurAziPos() {

  digitalWrite(AZI_DIR_PIN , HIGH);
  digitalWrite(AZI_STEP_PIN , HIGH);
  delay(10);
  digitalWrite(AZI_STEP_PIN , LOW);
  delay(10);

}



float utcTime (float localLong, int lclHr, int lclMn) {
  // Function accepts local longitude, hour and minutes, and returns UTC Time in Minutes

  float utcTime = 0;

  float lclTime = lclHr * 60 + lclMn;

  float apartMins = localLong * 4;

  if (apartMins < 0 ) {
    utcTime = lclTime + abs(apartMins);
    if (utcTime > 1440) {
      utcTime = utcTime - 1440;
    }
  } else {
    utcTime = lclTime - apartMins;

    if (utcTime < 0) {
      utcTime = 1440 + utcTime;
    }
  }
  return utcTime;  // in Minutes
}




float lctConH(int utcHr , float longitude) {

  int TZA = int (longitude / 15);
  int lcHr = utcHr + TZA;
  return lcHr;

}


float solar_time ( float localLong, int UTCHr, int UTCMn) {
  //This function returns Solar Time in Minutes for the given longitude and UTC time.
  //Coordinated Universal Time abbreviated to UTC, is the primary time standard by which the world regulates clocks and time. It is within
  //about 1 second of mean solar time at 0° longitude, it does not observe daylight saving time.  GPS Provides UTC


  int STMZMeridian[] = {180, 165, 150, 135, 120, 105, 90,  75,  60,  45,  30,  15,   0,   -15, -30, -45, -60, -75, -90, -105, -120, -135, -150, -165};
  //                  0HR  1AM  2AM  3AM  4AM  5AM  6AM  7AM  8AM  9AM  10AM 11AM  NOON 1PM  2PM  3PM  4PM  5PM  6PM  7PM   8PM   9PM   10PM  11PM
  //                  0 1 2 3 4 5 6 7 8 9   10   11 12  13  14   15   16   17   18   19 20  21  22  23

  //Day starts at 180E meridian or International Date Line, and ends at 179W Meridian. So at UTC Time 2AM, people around 150E longitude are at
  // solar noon, and at 3PM UTC, people around -60W longitude are at Solar Noon.

  float noonAtMer = 0;  //Initialize - Noon is at Meridian variable to Zero
  float solarTime = 0; // Local solar time

  int mulFactor = 1;  // Correction factor of UTC Time. If Longitude is -ve, subtract minutes if +ve add minutes

  if ( STMZMeridian[UTCHr] < 0) {
    mulFactor = -1;
  } else {
    mulFactor = 1;
  }

  if ( UTCHr < 13) {
    noonAtMer = STMZMeridian[UTCHr] - ( float(UTCMn) / 4 * mulFactor );  // Earth rotates 4mins per Degree of Longitude
  } else {
    noonAtMer = STMZMeridian[UTCHr] + ( float(UTCMn) / 4 * mulFactor );
  }


  if ( localLong < noonAtMer ) {
    if ( localLong < 0  &&  noonAtMer > 0 ) {
      solarTime = ( (180 - abs(localLong)) + (180 - abs(noonAtMer)) ) * 4 + 720;

      if (solarTime > 1380 ) {
        solarTime = solarTime - 1440;
      }

    } else {
      solarTime = abs(abs(localLong - noonAtMer) * 4 - 720);
    }
  } else {
    solarTime = abs (localLong - noonAtMer) * 4 + 720;
    if (solarTime > 1380 ) {
      solarTime = solarTime - 1440;
    }
  }

  //Finally we need subtract Equation of Time  - lets not worry about it now.

  return solarTime;  // in Minutes

}

float axialAngle(int Nday) { // This is how I declared the axial angle function. I could have declared it as anything I wanted but because the value of axial angle is a float, it is declared as a float
  float aA = (360 * (Nday - 1) / 365.242); // I am declaring the value that will hold the axial angle's value. Axial angle is the angular distance from , with the sun as the center, from the position of the Earth on Jan 1 to the present day or the day it is working on.
  return aA; //  returns the value of the axial angle when the funciton is called
}

float eot(float aA) { // This is the Equation of tim funciton or known as the EOT

  float EOT = 0;
  EOT = ( (0.258 * cos(radians(aA))) - (7.416 * sin(radians(aA))) - (3.648 * cos(radians(2 * aA))) - (9.228 * sin(radians(2 * aA))) );
  return EOT;  // equation of time returns in minutes

}

float dec_angle(int Nday) { // declaring the declination angle funciton
  float decAngle = 0;
  //float decAngle = -23.45 * cos(radians(0.98630136986 * (Nday + 10))); // Got this from http://www.pveducation.org/pvcdrom/properties-of-sunlight/declination-angle
  decAngle = degrees(asin(0.39795 * cos(radians(0.98563 * (Nday - 173))))); // both formulas give you the same output
  return decAngle; // returing the decAngle as a float
}


float hr_angle(float SHT) { // decalring the hr_angle function
  float HrAngle = 0;
  HrAngle = 15 * (SHT - 12); // calculating the HrAngle function
  return HrAngle; // returning the HrAngle function
}

float alt_angle ( float decAngle, float lati, float hrAngle ) {// calculating the altitude angle function and giving it the values of decAngle and the rest of the function values
  float Alt_Angle = 0; // delcaring the Alt_Angle as a 0
  Alt_Angle = degrees( asin(sin(radians(decAngle)) * sin(radians(lati)) + cos(radians(decAngle)) * cos(radians(hrAngle)) * cos(radians(lati)) )); // Got this formula from powerformthesun.net and eq 3-17.
  return Alt_Angle;
}


float az_angle (float decAngle,  float hrAngle, float lati, float altAngle) {

  float azA = 0;

  float azAFcheck = 0;
  float azAScheck = 0;
  float aPrime = 0;
  float aDPrime = 0;

  float cosHa = 0;
  float sinHa = 0;
  float tanDaBtanLa = 0;

  // Calculating Azimuth using the first of Equations of Earth-Observor Geometry

  aPrime = degrees(asin( -1 * cos(radians(decAngle)) * sin(radians(hrAngle)) / cos(radians(altAngle)) ));

  tanDaBtanLa = tan(radians(decAngle)) / tan(radians(lati));

  cosHa = cos(radians(hrAngle));

  if (cosHa < tanDaBtanLa ) {
    azAFcheck = 360 + aPrime;
  } else {
    azAFcheck = 180 - aPrime;
  }

  // Calculating Azimuth using the first of Equations of Earth-Observor Geometry

  aDPrime =  degrees(acos( ((sin(radians(decAngle)) * cos(radians(lati))) - (cos(radians(decAngle)) * cos(radians(hrAngle)) * sin(radians(lati)))) / (cos(radians(altAngle))) ));

  sinHa = sin(radians(hrAngle));

  if (sinHa > 0) {
    azAScheck = 360 - aDPrime;
  } else {
    azAScheck = aDPrime;
  }

  azA = azAScheck;

  return azA;
}

float NDay(int Year , int Month , int Day) { // the rest of the function is for the Nday function

  int Nday;

  int lYear = 0;

  // Checking if Year is a Leap Year

  if (Year % 400 == 0) {
    lYear = 1;
  } else if (Year % 100 == 0) {
    lYear = 0;
  } else if (Year % 4 == 0) {
    lYear = 1;
  } else {
    lYear = 0;
  }

  if (Month == 1) {
    Nday = Day;
  } else if (Month == 2) {
    Nday = Day + 31;
  } else if (Month == 3) {
    Nday = Day + 59 + lYear;
  } else if (Month == 4) {
    Nday = Day + 90 + lYear;
  } else if (Month == 5) {
    Nday = Day + 120 + lYear;
  } else if (Month == 6) {
    Nday = Day + 151 + lYear;
  } else if (Month == 7) {
    Nday = Day + 181 + lYear;
  } else if (Month == 8) {
    Nday = Day + 212 + lYear;
  } else if (Month == 9) {
    Nday = Day + 243 + lYear;
  } else if (Month == 10) {
    Nday = Day + 273 + lYear;
  } else if (Month == 11) {
    Nday = Day + 304 + lYear;
  } else {
    Nday = Day + 334 + lYear;
  }
  
  return Nday; // return the Nday function
  
}
