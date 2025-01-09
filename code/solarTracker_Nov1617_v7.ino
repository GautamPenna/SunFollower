/* Name:        Sun Follower
   Programmer:  Gautam Penna
   Date:        08/19/2017 to 11/13/2017
   Purpose:     SunFollower follows Sun through out the day, capturing optimal Solar Radiaton on Solar Panels
   Logic:       There are 2 Modes (1) Real Mode (2) Demo Mode (i) Demo Set Mode and (ii) Demo Run Mode

   Motors:
   =======
   NEMA 17 is Altitude Motor, NEMA 23 is the Azimuth Motor. I'm using TB6600 as the Stepper Driver.
   Running in Half Step Mode - with 400 Setps for one full rotation. 1 Step = 0.9°
   Considred Allegro A4988 Driver, Adafruit Driver Board - but found TB6600 to be stable and easy to operate.

   Demo Set Mode:
   ==============
   In Demo Set Mode, users will have an opportunity to change Place, Month and Day potentiometers. These values
   serve as inputs for Demo Run Mode

   Demo Run Mode:
   ==============
   In Demo Run Mode, the program will read the user provided inputs for Place, Month and Day and then execute the
   program following the logic.   Year is hard-coded to 2017
   (1) Gets the Latitude and Longitude from the array
   (2) Calculates Earths Declination, Equation of Time, Hour Angle, Axial Angle and NDay
   (3) Calculates Altitude and Azimuth for the Location - for the given date and time.
   (4) Executes the program from 8AM Local Solar Time to 9PM Local Solar Time
   (5) Since its a Demo MODE - Program fast tracks and advances every 20 seconds as 1 Hour
   (6) Program Displays Time, Latitude, Longitude, Altitude and Azimut on OLED before Altitude and Azimuth Motors
   (7) If the User does not change inputs - Demo Run Mode restarts again.

   Real Mode:
   ==========
   In Real Mode, the program will try to acquire GPS Coordinates for the location. It can take up to 2 minutes for
   GPS Module to obtain a FIX. Once Latitude, Longitude, Date, Hour, Minute and the UTC time for the location are
   obtained - program proceeds to calulate
   (1) NDay, Hour Angle, Declination Angle, Equation of Time, Solar Time, Axial Angle, Azimuth and Altitude.
   (2) The programming logic, controls the movement of the motors every 10 minutes
   (3) We limit the range of Altitude from 0° to 90° (0° is Horizon, and 90° is Zenith)
   (4) We limit the range of Azimuth from 75° to 270° (90° is Due EAST, and 270° is Due WEST)
   (5) The program only operates from 8AM Local Solar Time to 9PM Local Solar Time
   (6) Upon reaching the end of cycle, Altitude and Azimuth reset and wait at the Home Postions.
   (7) In the Event GPS FIX is not acquired (2 minutes) The program Displays a message on the OLED and uses to Defaults
   (8) Plano East Sr.High - Latitude and Longitude Serve as Defualt Coordiantes
   (9) I set November 14th as Defualt Day - and program executes in a rapid demo mode - for an PESH Setup.

    Uses:
    ============
    Solar Farms, Homes, Buses, RV's, Trains, Boats, Ocean Liners are all potential prospects for such a project.

    Limitations:
    ============
    This Setup only works for Northern Latitudes,
    This Setup will not work for latitudes close to Arctic Circle, where days can be 24Hr longs, and long nights.

    Additonal Considerations:
    =========================
    Controls Via Bluetooth, iOS App development to control the Setup and to run the Demo.

*/

#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>   //Include for Adafruit - OLED Display
#include <Adafruit_SSD1306.h> //Include for Adafruit - OLED Display
#include <Adafruit_GPS.h> // Include the Adafruit GPS library.
#include <SoftwareSerial.h> // Include the software serial library


#define OLED_RESET 4
Adafruit_SSD1306 display(OLED_RESET);

#define LOGO16_GLCD_HEIGHT 16
#define LOGO16_GLCD_WIDTH  16

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

#define NUM_GPS_ATTEMPTS 30

////Solar Tracker Solar Panel values
//#define R1a 1842
//#define R2a 18000
//float Ia;
//float V1a;
//float V2a;
//float Pa;
//
////Stationary Solar Panel values
//
//#define R1b 1902
//#define R2b 18100
//float Ib;
//float V1b;
//float V2b;
//float Pb;


//GPS Definitions
SoftwareSerial mySerial(50, 52); // Setting up the new serial port to communicate to the GPS tracker(TX , RX)
Adafruit_GPS GPS(&mySerial); // Making the GPS object

String NMEA1; // Variable for first NMEA sentence
String NMEA2; // Variable for second NMEA sentence

float wholeLONDeg;
float wholeLATDeg;
char gpsChar; // To read the charcters from the GPS


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

  Serial.begin(115200);
  Serial.println("Starting the Setup");

  display.begin(SSD1306_SWITCHCAPVCC, 0x3c);
  display.display();
  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setCursor(0, 0);
  display.write(15);
  display.print(" Follower");
  display.setCursor(0, 16);
  display.setTextSize(1);
  display.println(" ");
  display.setTextSize(2);
  display.println("Starting.");
  display.display();
  for (int p = 0; p < 8; p++) {
    display.print(".");
    display.display();
    delay(500);
  }
  display.write(15);
  display.display();
  delay(2000);

  //GPS Setup
  GPS.begin(9600); // Turn on GPS at 9600 baud rate
  GPS.sendCommand("$PGCMD,33,0*6D"); // turn off anteane update nusience data
  GPS.sendCommand(PMTK_SET_NMEA_OUTPUT_RMCGGA); //Tell GPS we want only $GPRMC and $GPGGA NMEA sentences`
  GPS.sendCommand(PMTK_SET_NMEA_UPDATE_1HZ); // I only want 1 hz

  delay(1000); // Delay a 1000 milliseconds to give it a chance to digest all of the commands sent to it



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

  int gpsYEAR = 19;
  int gpsDAY = 14;
  int gpsMON = 11;
  int gpsHOUR = 0;
  int gpsMIN = 0;
  int localHOUR = 8;
  int localMIN = 0;


  gpsDAY = GPS.day;
  gpsMON = GPS.month;
  gpsHOUR = GPS.hour;
  gpsMIN = GPS.minute;
  gpsYEAR = GPS.year;

}

void loop() {

  Serial.println("Starting the Loop");


  int year  = 2019;
  int month = 12;
  int day = 14;
  int utcHr = 0;
  int utcMn = 0;

  // We will set PESH Coordinates as default 33.005803,-96.6528103
  float latitude = 33.005803;
  float longitude = -96.6528103;

  String Place = "PESH";

  // motor and lcd part

  int inRealMode = digitalRead(REALMODE); //if True, Real Mode - else Demo Mode
  int inDemoMode = digitalRead(DEMOMODE);
  int inDemoSetMode = digitalRead(DEMOSET); //if True DemoSet Mode, else Run Demo
  int inDemoRunMode = digitalRead(DEMORUN);

  float preAlt = 0;
  float preAzi = 0;
  int resetZeroAlt = 0; // a variable for a loop to make it do it only once
  int resetZeroAzi = 0; // a variable for a loop to make it do it only once


  Serial.println("Past Reading the potentiometers");

  if (inRealMode) {
    Serial.println("In Real Mode");
    display.clearDisplay();
    display.setTextSize(2);
    display.setTextColor(WHITE);
    display.setCursor(0, 0);
    display.write(15);
    display.print(" Follower");
    display.setCursor(0, 16);
    display.setTextSize(2);
    display.println(" ");
    display.println ("REAL MODE");
    display.display();
    delay(8000);

    gotBacktoAltHome();
    gotBacktoAziHome();

    float deg; //Will hold position data in simple degree format
    float degWhole; //Variable for the whole part of position
    float degDec;  //Variable for the decimal part of degree
    int gpsYEAR = 19;
    int gpsDAY = 14;
    int gpsMON = 11;
    int gpsHOUR = 0;
    int gpsMIN = 0;
    int localHOUR = 8;
    int localMIN = 0;

    Serial.println(" ");
    Serial.println ("==================================================================================");
    Serial.println ("Trying to get a GPS FIX  It may take upto 30 Seconds, please be patient. ");
    Serial.println ("" );
    Serial.println ("==================================================================================");


    getGPS(); // Make a function

    if (GPS.fix != 1) {

      Serial.println(" ");
      Serial.println ("==================================================================================");
      Serial.println ("We don't have a GPS Fix, you're perhaps indoors, consider moving outside and reset");
      Serial.println ("We are going use GPS Coordinates for PESH 33.005803 N, 96.6528103 W and run a DEMO!");
      Serial.println ("==================================================================================");

      display.clearDisplay();
      display.setTextSize(2);
      display.setTextColor(WHITE);
      display.setCursor(0, 0);
      display.write(15);
      display.print(" Follower");
      display.setCursor(0, 16);
      display.setTextSize(1);
      display.println(" ");
      display.println ("We don't have a GPS  Fix, you're perhaps  indoors, consider moving outside and reset");
      display.display();
      delay(8000);

      display.clearDisplay();
      display.setTextSize(2);
      display.setTextColor(WHITE);
      display.setCursor(0, 0);
      display.write(15);
      display.print(" Follower");
      display.setCursor(0, 16);
      display.setTextSize(1);
      display.println(" ");
      display.println ("Setting Lat&Long for:");
      display.println(" ");
      display.println ("Plano East Sr.High");
      display.println(" ");
      display.println ("33.0058 N, 96.6528 W");
      display.display();
      delay(8000);
      Place = "PESH";
      latitude = 33.005803;
      longitude = -96.6528103;

    } else if (GPS.fix == 1) {
      display.clearDisplay();
      display.setTextSize(2);
      display.setTextColor(WHITE);
      display.setCursor(0, 0);
      display.write(15);
      display.print(" Follower");
      display.setCursor(0, 16);
      display.setTextSize(2);
      display.setTextColor(WHITE);
      display.println(" ");
      display.println("GPS FIX");
      display.println("ACQUIRED !");
      display.display();
      delay (5000);
      Serial.println ("==================================");
      Serial.println ("GPS FIX ACQUIRED");
      Serial.println ("==================================");

    } else {
      while (1) Serial.println("This shoud never Happen");
    }
    //Write code to display... that you're waiting for GPS Fix



    while (localHOUR < 21) {

      getGPS();

      if (GPS.fix == 1) { // Only log data if you have a fix

        degWhole = float(int(GPS.longitude / 100)); //gives me the whole degree part of Longitude
        degDec = (GPS.longitude - degWhole * 100) / 60; //give me fractional part of longitude
        deg = degWhole + degDec; //Gives complete correct decimal form of Longitude degrees
        if (GPS.lon == 'W') { //If you are in Western Hemisphere, longitude degrees should be negative
          deg = (-1) * deg;
        }

        wholeLONDeg = deg;

        degWhole = float(int(GPS.latitude / 100)); //gives me the whole degree part of latitude
        degDec = (GPS.latitude - degWhole * 100) / 60; //give me fractional part of latitude
        deg = degWhole + degDec; //Gives complete correct decimal form of latitude degrees
        if (GPS.lat == 'S') { //If you are in Southern hemisphere latitude should be negative
          deg = (-1) * deg;
        }

        wholeLATDeg = deg;

        gpsDAY = GPS.day;
        gpsMON = GPS.month;
        gpsHOUR = GPS.hour;
        gpsMIN = GPS.minute;
        gpsYEAR = GPS.year;

        latitude = wholeLATDeg;
        longitude = wholeLONDeg;

        Serial.print("Latitude: "); Serial.print(latitude);
        Serial.print(" Longitude: "); Serial.println(longitude);

      }

      int Nday = NDay(year, gpsMON, gpsDAY);  // decalring the NDay value so that I can get the Nth day form the function. This is how I declare and execute a function
      float aA = axialAngle(Nday); // This float value holds the value of the axial angle function by calling the funciton and giving the value needed
      float EOT = eot(aA); // This float holds the eot in a minute value

      //    int lcMn = 0;
      //    float utcMins = 0;

      int solarMins = 0;
      if (GPS.fix != 1 ) {

        float utcTempTime = utcTime(longitude, localHOUR, localMIN);
        gpsHOUR = int(utcTempTime / 60);
        gpsMIN = int(utcTempTime) % 60;

      }

      solarMins = solar_time(longitude , gpsHOUR , gpsMIN); // solar_time returns hours, since day started
      //int solarMins = solarHours * 60; // This process is to make it into minutes
      int SolarM = int(solarMins) % 60; // Calculating the solar minute
      int SolarH = int(solarMins / 60); // Calculatin the solar hour time

      float decAngle = dec_angle(Nday); // Calculating the declination angle given the NDay
      float HrAngle = hr_angle(solarMins / 60); // calculating the Hr angle from the Total Hours
      float AltAngle = alt_angle(decAngle , latitude , HrAngle); // Calculating the alitude angle from the decAngle , latitude , and the Hr Angle
      float AziAngle = az_angle(decAngle , HrAngle , latitude , AltAngle); // Calculating the Azimuth Angle given the decAngle , HrAngle , Latitude , and altitude

      if (GPS.fix == 1) {
        localHOUR = SolarH;
        Place = "Any Town, USA";
      }

      Serial.print(" Location: "); Serial.print(Place); Serial.print(" Latitude: "); Serial.print(latitude); Serial.print(" Longitude: "); Serial.print(longitude);
      Serial.print(" NDay: "); Serial.print(Nday); Serial.print(" Axial Angle: "); Serial.print(aA); Serial.print(" Eq of Time: "); Serial.print(EOT);
      Serial.print(" UTC Time: "); Serial.print(gpsHOUR); Serial.print(":"); Serial.print(gpsMIN);  Serial.print (" SolarTime: "); Serial.print(SolarH); Serial.print(" : "); Serial.print(SolarM);
      Serial.print(" Hr Angle: "); Serial.print(HrAngle); Serial.print(" Dec Angle: "); Serial.print(decAngle);
      Serial.print(" Altitude: "); Serial.print(AltAngle); Serial.print (" Azimuth: "); Serial.println(AziAngle);

      // Start Printing to OLED
      display.clearDisplay();
      display.setTextSize(2);
      display.setCursor(0, 0);
      display.write(15);
      display.print(" Follower");
      display.setCursor(0, 16);
      display.setTextSize(1);
      display.println(" ");
      display.setTextSize(2);
      display.println("Time: ");
      display.setCursor(0, 32);
      display.setTextSize(1);
      display.println(" ");
      display.setTextSize(3);
      if (localHOUR < 10) display.print("0");
      display.print(localHOUR);
      display.print(":");
      if (localMIN < 10) display.print("0");
      display.print(localMIN);
      display.setTextSize(2);
      display.print (" Hr");
      display.display();

      delay(2000);

      display.clearDisplay();
      display.setTextSize(2);
      display.setCursor(0, 0);
      display.write(15);
      display.print(" Follower");
      display.setCursor(0, 16);
      display.setTextSize(1);
      display.println(" ");
      display.setTextSize(2);
      display.println("Latitude:");
      display.setCursor(0, 32);
      display.setTextSize(1);
      display.println(" ");
      display.setTextSize(3);
      display.println(latitude);
      display.display();

      delay(2000);

      display.clearDisplay();
      display.setTextSize(2);
      display.setCursor(0, 0);
      display.write(15);
      display.print(" Follower");
      display.setCursor(0, 16);
      display.setTextSize(1);
      display.println(" ");
      display.setTextSize(2);
      display.println("Longitude:");
      display.setCursor(0, 32);
      display.setTextSize(1);
      display.println(" ");
      display.setTextSize(3);
      display.println(longitude);
      display.display();

      delay(2000);
      display.clearDisplay();
      display.setTextSize(2);
      display.setTextColor(WHITE);
      display.setCursor(0, 0);
      display.write(15);
      display.print(" Follower");
      display.setCursor(0, 16);
      display.setTextSize(1);
      display.println(" ");
      display.setTextSize(2);
      display.print("Azimuth : ");
      display.setCursor(0, 32);
      display.setTextSize(1);
      display.println(" ");
      display.setTextSize(3);
      display.print(AziAngle);
      display.setTextSize(1);
      display.write(167);
      display.display();

      delay(2000);

      display.clearDisplay();
      display.setTextSize(2);
      display.setCursor(0, 0);
      display.write(15);
      display.print(" Follower");
      display.setCursor(0, 16);
      display.setTextSize(1);
      display.println(" ");
      display.setTextSize(2);
      display.print("Altitude : ");
      display.setCursor(0, 32);
      display.setTextSize(1);
      display.println(" ");
      display.setTextSize(3);
      display.print(AltAngle);
      display.setTextSize(1);
      display.write(167);
      display.display();

      delay(2000);

      // End Printing to OLED   - Start Moving the Motors

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




      if (GPS.fix != 1) {
        delay(60);  //Since we dont have a GPS Fix, we will run fast
        localMIN = localMIN + 10;

        if (localMIN == 60) {
          localMIN = 0;
          localHOUR = localHOUR + 1;
        } else if (localMIN > 60) {
          localMIN = localMIN - 60;
          localHOUR = localHOUR + 1;
        } else {
          localMIN = localMIN + 0;
        }
      } else {
        localHOUR = SolarH;
        delay(600000); // We will run Slow
      }
 
     }

    gotBacktoAltHome();
    gotBacktoAziHome();

    // End of Getting GPS

    Serial.println("End of Real Mode");

  }

  ////////////////////////////////////////////////////////BEWARE , DEMO MODE IS BELOW//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

  else if (inDemoMode) {

    Serial.println("In Demo Mode");
    display.clearDisplay();
    display.setTextSize(2);
    display.setTextColor(WHITE);
    display.setCursor(0, 0);
    display.write(15);
    display.print(" Follower");
    display.setCursor(0, 16);
    display.setTextSize(2);
    display.println(" ");
    display.println ("DEMO MODE");
    display.display();
    delay(8000);
    display.clearDisplay();


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
      delay(2000);

      Serial.println(">>> In Demo RUN MODE: Past Reading the potentiometers");

      // We will run the Demo from 8AM to 8PM


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
        Serial.print(" UTC Time: "); Serial.print(utcHr); Serial.print(":"); Serial.print(utcMn);  Serial.print (" SolarTime: "); Serial.print(SolarH);
        Serial.print(" : "); Serial.print(SolarM); Serial.print(" Hr Angle: "); Serial.print(HrAngle); Serial.print(" Dec Angle: "); Serial.print(decAngle);
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
        display.setCursor(0, 0);
        display.write(15);
        display.print(" Follower");
        display.setCursor(0, 16);
        display.setTextSize(1);
        display.println(" ");
        display.setTextSize(2);
        display.println(Place);
        display.setCursor(0, 32);
        display.setTextSize(1);
        display.println(" ");
        display.setTextSize(3);
        if (locH < 10) display.print("0");
        display.print(locH);
        display.print(":");
        display.print("00");
        display.setTextSize(2);
        display.print (" Hr");
        display.display();

        delay(2000);

        display.clearDisplay();
        display.setTextSize(2);
        display.setCursor(0, 0);
        display.write(15);
        display.print(" Follower");
        display.setCursor(0, 16);
        display.setTextSize(1);
        display.println(" ");
        display.setTextSize(2);
        display.println("Latitude: ");
        display.setCursor(0, 32);
        display.setTextSize(1);
        display.println(" ");
        display.setTextSize(3);
        display.println(latitude);
        display.display();

        delay(2000);

        display.clearDisplay();
        display.setTextSize(2);
        display.setCursor(0, 0);
        display.write(15);
        display.print(" Follower");
        display.setCursor(0, 16);
        display.setTextSize(1);
        display.println(" ");
        display.setTextSize(2);
        display.println("Longitude: ");
        display.setCursor(0, 32);
        display.setTextSize(1);
        display.println(" ");
        display.setTextSize(3);
        display.println(longitude);
        display.display();

        delay(2000);

        display.clearDisplay();
        display.setTextSize(2);
        display.setTextColor(WHITE);
        display.setCursor(0, 0);
        display.write(15);
        display.print(" Follower");
        display.setCursor(0, 16);
        display.setTextSize(1);
        display.println(" ");
        display.setTextSize(2);
        display.print("Azimuth : ");
        display.setCursor(0, 32);
        display.setTextSize(1);
        display.println(" ");
        display.setTextSize(3);
        display.print(AziAngle);
        display.setTextSize(1);
        display.write(167);
        display.display();

        delay(2000);

        display.clearDisplay();
        display.setTextSize(2);
        display.setCursor(0, 0);
        display.write(15);
        display.print(" Follower");
        display.setCursor(0, 16);
        display.setTextSize(1);
        display.println(" ");
        display.setTextSize(2);
        display.print("Altitude : ");
        display.setCursor(0, 32);
        display.setTextSize(1);
        display.println(" ");
        display.setTextSize(3);
        display.print(AltAngle);
        display.setTextSize(1);
        display.write(167);
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

  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setCursor(0, 0);
  display.write(15);
  display.print(" Follower");
  display.setCursor(0, 16);
  display.print("ALT Motor");
  display.setCursor(0, 32);
  display.print("Resetting....");
  display.display();



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

  delay(200);

  int altStep = ALT_MAXS + 15;
  Serial.print("This is the ALT Max Steps : ");
  Serial.println(altStep);

  while (altStep > ALT_MINS) {
    digitalWrite(ALT_DIR_PIN , LOW);
    digitalWrite(ALT_STEP_PIN , HIGH);
    delay(10);
    digitalWrite(ALT_STEP_PIN , LOW);
    delay(10);
    altStep = altStep - 1;
  }
  display.print("...OK");
  display.display();


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
  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setCursor(0, 0);
  display.write(15);
  display.print(" Follower");
  display.setCursor(0, 16);
  display.print("AZI Motor");
  display.setCursor(0, 32);
  display.print("Resetting....");
  display.display();

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

  delay(200);

  int aziStep = AZI_MAXS + 9;
  Serial.print("This is the AZI Max Steps : ");
  Serial.println(aziStep);

  while (aziStep > AZI_MINS) {
    digitalWrite(AZI_DIR_PIN , LOW);
    digitalWrite(AZI_STEP_PIN , HIGH);
    delay(10);
    digitalWrite(AZI_STEP_PIN , LOW);
    delay(10);
    aziStep = aziStep - 1;
  }
  display.print("...OK");
  display.display();

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
  //                0HR  1AM  2AM  3AM  4AM  5AM  6AM  7AM  8AM  9AM  10AM 11AM  NOON 1PM  2PM  3PM  4PM  5PM  6PM  7PM   8PM   9PM   10PM  11PM
  //                0 1 2 3 4 5 6 7 8 9   10   11 12  13  14   15   16   17   18   19 20  21  22  23

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

void getGPS() {

  //We dont want to be here forever - if the setup is indoors - so we will break out of here.

  clearGPS();

  int gControl = 0;

  while (!GPS.newNMEAreceived() ) {
    //while (!GPS.newNMEAreceived() && gControl < NUM_GPS_ATTEMPTS  ) {

    gpsChar = GPS.read();
    gControl++;
    //delay(1000);
  }

  gControl = 0;

  GPS.parse(GPS.lastNMEA() );
  NMEA1 = GPS.lastNMEA();

  while (!GPS.newNMEAreceived() ) {
    //while (!GPS.newNMEAreceived() && gControl < NUM_GPS_ATTEMPTS ) {
    gpsChar = GPS.read();
    gControl++;
    //delay(1000);
  }

  GPS.parse(GPS.lastNMEA() );
  NMEA2 = GPS.lastNMEA();

  Serial.println(NMEA1);
  Serial.println(NMEA2);
  Serial.println(" ");

}

void clearGPS() {  // clear old and bad data from serial port

  int cControl = 0;

  while (!GPS.newNMEAreceived()) { // loop until you have a good nema sentance
    //while (!GPS.newNMEAreceived() && cControl < NUM_GPS_ATTEMPTS) { // loop until you have a good nema sentance
    gpsChar = GPS.read();
    cControl++;
    //delay(1000);
  }
  cControl = 0;
  GPS.parse(GPS.lastNMEA());
  while (!GPS.newNMEAreceived()) {
    //while (!GPS.newNMEAreceived() && cControl < NUM_GPS_ATTEMPTS) {
    gpsChar = GPS.read();
    cControl++;
    //delay(1000);
  }

  GPS.parse(GPS.lastNMEA());
}
