#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_GPS.h> // Include the Adafruit GPS library.
#include <SoftwareSerial.h> // Include the software serial library
#include <SD.h> // Load the SD card Library
#include <SPI.h> // Load the SPI library

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


#define REAL_MODE 50
#define DEMO_SET 51
#define ResetButton 52

int reset_pin = 7;

int dirPin = 8;
int stepPin = 9;


String location[] = {"Tokyo", "Hong Kong", "Hyderabad", "Tel Aviv", "Vienna", "London", "St. Johns", "New York", "Dallas", "Denver", "SFrancisco", "Honolulu"};
String month[] = {"Jan" , "Feb" , "Mar" , "Apr" , "May" , "Jun" , "Jul" , "Aug" , "Sep" , "Oct" , "Nov" , "Dec"};
int azMS1 = 3;

SoftwareSerial mySerial(3, 2); // Setting up the new serial port to communicate to the GPS tracker
Adafruit_GPS GPS(&mySerial); // Making the GPS object

String NMEA1; // Variable for first NMEA sentence
String NMEA2; // Variable for second NMEA sentence

float deg; //Will hold position data in simple degree format
float degWhole; //Variable for the whole part of position
float degDec;  //Variable for the decimal part of degree
float wholeLONDeg;


char cHar; // To read the charcters from the GPS

//int chipSelect = 4; // Chip select is connect to pin 4
File myGPSdata; // create variable for sd card object

float latitude;
float longitude;


int hNS;
int hEW;




void setup() {

  Serial.begin(9600);
  display.begin(SSD1306_SWITCHCAPVCC, 0x3c);
  display.display();
  display.clearDisplay();

  pinMode(AZstepPin , OUTPUT);
  pinMode(AZdirPin , OUTPUT);
  pinMode(azMS1 , OUTPUT);

#if (SSD1306_LCDHEIGHT != 64)
#error("Height incorrect, please fix Adafruit_SSD1306.h!");
#endif

  Serial.println("Starting the Program");
  pinMode(DEMO_SET, INPUT);
  pinMode(REAL_MODE, INPUT);
  pinMode(ResetButton, INPUT);

  GPS.begin(9600); // Turn on GPS at 9600 baud rate
  GPS.sendCommand("$PGCMD,33,0*6D"); // turn off anteane update nusience data
  GPS.sendCommand(PMTK_SET_NMEA_OUTPUT_RMCGGA); //Tell GPS we want only $GPRMC and $GPGGA NMEA sentences`
  GPS.sendCommand(PMTK_SET_NMEA_UPDATE_1HZ); // I only want 1 hz

  delay(1000); // Delay a 1000 milliseconds to give it a chance to digest all of the commands sent to it

  pinMode(10, OUTPUT); // Must keep pin 10 open to make it happy
  //SD.begin(chipSelect); // Connected to chipSelect also pin 4
  SD.begin(4); // Connected to chipSelect also pin 4

  if (SD.exists("NMEA.txt")) { // Check to see if file is there
    SD.remove("NMEA.txt"); // Remove the existing file

  }

  if (SD.exists("GPSData.txt")) { // Check to see if file is there
    SD.remove("GPSData.txt"); // Remove the existing file

  }

  if (SD.exists("PData.txt")) { // Check to see if file is there
    SD.remove("PData.txt"); // Remove the existing file

  }

}

void loop() {

  for (int k = 0; k <= 72; k++) {



    int year  = 2017;   // print out a test information so I can check on websites if my calculation are right for that day
    int month;     // print out a test information so I can check on websites if my calculation are right for that day
    int day;      // print out a test information so I can check on websites if my calculation are right for that day
    int utcHr;     // print out a test information so I can check on websites if my calculation are right for that day
    int utcMn;      // print out a test information so I can check on websites if my calculation are right for that day
    float latitude = 0; // print out a test information so I can check on websites if my calculation are right for that day
    float longitude = 0;// print out a test information so I can check on websites if my calculation are right for that day


    getGPS(); // Make a function
    delay(900); // delay

    latitude = (GPS.latitude); // Get the latitude
    longitude = (GPS.longitude); // Get the longitude
    hNS = GPS.lat; // North Hemisphere or South Hemisphere
    hEW = GPS.lon; // West  Hemisphere or East  Hemisphere

    if (GPS.fix == 1) { // Only log data if you have a fix

      myGPSdata = SD.open("NMEA.txt", FILE_WRITE); // Open it for writing
      myGPSdata.println(NMEA1); // print the first sentence
      myGPSdata.println(NMEA2); // print the second sentence
      myGPSdata.close(); // close the file


      myGPSdata = SD.open("GPSData.txt", FILE_WRITE);

      degWhole = float(int(GPS.longitude / 100)); //gives me the whole degree part of Longitude
      degDec = (GPS.longitude - degWhole * 100) / 60; //give me fractional part of longitude
      deg = degWhole + degDec; //Gives complete correct decimal form of Longitude degrees
      if (GPS.lon == 'W') { //If you are in Western Hemisphere, longitude degrees should be negative
        deg = (-1) * deg;
      }
      myGPSdata.print(deg, 4); //writing decimal degree longitude value to SD card
      myGPSdata.print(","); //write comma to SD card

      wholeLONDeg = deg;

      degWhole = float(int(GPS.latitude / 100)); //gives me the whole degree part of latitude
      degDec = (GPS.latitude - degWhole * 100) / 60; //give me fractional part of latitude
      deg = degWhole + degDec; //Gives complete correct decimal form of latitude degrees
      if (GPS.lat == 'S') { //If you are in Southern hemisphere latitude should be negative
        deg = (-1) * deg;
      }
      myGPSdata.print(deg, 4); //writing decimal degree longitude value to SD card
      myGPSdata.print(","); //write comma to SD card

      myGPSdata.print(GPS.altitude); //write altitude to file
      myGPSdata.print(" ");  //format with one white space to delimit data sets

      myGPSdata.close();

      /// endhere

      Serial.println("Got to FIX the Opening Step.");

      myGPSdata = SD.open("PData.txt", FILE_WRITE); // open the gps data text on sd card
      myGPSdata.print(latitude); // prints the latitude
      myGPSdata.print(" , "); // prints a comma
      myGPSdata.print(longitude); // prints the longitude
      myGPSdata.print(" , "); // prints a comma
      myGPSdata.print(GPS.day);
      myGPSdata.print("/");
      myGPSdata.print(GPS.month);
      myGPSdata.print("/");
      myGPSdata.print(GPS.year);
      Serial.println("In the Middle. Almost Done.");
      myGPSdata.print(" , "); // prints a comma
      myGPSdata.print(GPS.hour);
      myGPSdata.print(":");
      myGPSdata.print(GPS.minute);
      myGPSdata.print(":");
      myGPSdata.print(GPS.seconds);
      myGPSdata.print(".");
      myGPSdata.println(GPS.milliseconds);
      myGPSdata.close();

      Serial.println("Got to the closing step.");
    }

    Serial.print(latitude); // print the coordinates
    Serial.print(hNS);
    Serial.print(" , ");
    Serial.print(longitude);
    Serial.print(hEW);

    while (GPS.fix == 1) {
      continue;
    } while (GPS.fix != 1) {
      latitude = 32.7767;
      longitude = 96.7970;
      display.clearDisplay();
      display.setTextSize(2);
      display.setTextColor(WHITE);
      display.setCursor(0, 0);
      display.println("Place:");
      display.println("You are probably inside. Try taking me outside. Using the last known coordinates");
      display.display();
      continue;
    }

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

    while (g = 0) {
      float preAlt = AltAngle;
      int AltAngleSteps = AltAngle / 0.9
      for (k <= AltAngleSteps) {
        goToCurAltPos();
      }
      g = 1;
    }


    int AltAngleSteps = (AltAngle - preAlt) / 0.9
    for (k <= AltAngleSteps) {
      goToCurAltPos();
    }
    delay(6000000);

    utcM = utcM + 10;
    lcM = lcM + 10;

    float preAlt = AltAngle;

  }

  gotBacktoAltHome();


}













void getGPS() {
  eraseGPS();

  while (!GPS.newNMEAreceived()) {

    cHar = GPS.read(); // Read from the GPS sentences. Loop until you have a good NMEA sentence
  }

  GPS.parse(GPS.lastNMEA()); // Parse that last good NMEA sentence
  NMEA1 = GPS.lastNMEA(); // Put the variable into the string

  while (!GPS.newNMEAreceived()) {

    cHar = GPS.read(); // Read from the GPS sentences. Loop until you have a good NMEA sentence
  }

  GPS.parse(GPS.lastNMEA()); // Parse that last good NMEA sentence
  NMEA2 = GPS.lastNMEA(); // Put the variable into the string

  while (!GPS.newNMEAreceived()) {

    cHar = GPS.read(); // Read from the GPS sentences. Loop until you have a good NMEA sentence
  }

  GPS.parse(GPS.lastNMEA()); // Parse that last good NMEA sentence


  Serial.println(NMEA1);
  Serial.println(NMEA2);

  Serial.println("   "); // print a space






}












void gotBacktoAltHome() {

  while (digitalRead(Alt_HomeP)) {
    digitalWrite(Alt_dirPin , HIGH);
    digitalWrite(Alt_stepPin , HIGH);
    delay(10);
    digitalWrite(Alt_stepPin , LOW);
    delay(10);
  }

  while (!digitalRead(Alt_HomeP)) { // move one step
    digitalWrite(Alt_dirPin , LOW);
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
    digitalWrite(Alt_dirPin , LOW);
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










void eraseGPS() {

  while (!GPS.newNMEAreceived()) {

    cHar = GPS.read(); // Read from the GPS sentences. Loop until you have a good NMEA sentence
  }
  GPS.parse(GPS.lastNMEA()); // Parse that last good NMEA sentence
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


  } else if (Month == 12) {
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


