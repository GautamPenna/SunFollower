/*This code is to declare the functions neccessary for my code.
  It will include the :
       1. getAZangle
       2. getALTangle
       3. getHrAngle
       4. getDecAngle
       5. getTZ
       6. getLTime
       7. getNday
       8. getSTime
*/

#include <Adafruit_GPS.h> // Include the Adafruit GPS library.
#include <SoftwareSerial.h> // Include the software serial library
#include <SD.h> // Load the SD card Library
#include <SPI.h> // Load the SPI library



SoftwareSerial mySerial(3, 2); // Setting up the new serial port to communicate to the GPS tracker
Adafruit_GPS GPS(&mySerial); // Making the GPS object

String NMEA1; // Variable for first NMEA sentence
String NMEA2; // Variable for second NMEA sentence

float deg; //Will hold positin data in simple degree format
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

int NDay; // Number of days from Jan 1.

int hour = GPS.hour;
int minute = GPS.minute;
int second = GPS.seconds;


int SolarH;
int SolarM;
int totalTimeH;





void setup() {

  Serial.begin(115200); // Turn on serial moniter

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

  Serial.print(SolarH);
  Serial.print(" : ");
  Serial.println(SolarM);

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

void eraseGPS() {

  while (!GPS.newNMEAreceived()) {

    cHar = GPS.read(); // Read from the GPS sentences. Loop until you have a good NMEA sentence
  }
  GPS.parse(GPS.lastNMEA()); // Parse that last good NMEA sentence
}




/*float Nday() {

  // All of the below information was found in Chapter 3 in the Book : Power From The Sun
  if (GPS.month == 1) {
    NDay = GPS.day; // Figure out the day number
  }

  if (GPS.month == 2) {
    NDay = GPS.day + 31; // Figure out the day number
  }

  if (GPS.month == 3) {
    NDay = GPS.day + 59; // Figure out the day number
  }

  if (GPS.month == 4) {
    NDay = GPS.day + 90; // Figure out the day number
  }

  if (GPS.month == 5) {
    NDay = GPS.day + 120; // Figure out the day number
  }

  if (GPS.month == 6) {
    NDay = GPS.day + 151; // Figure out the day number
  }

  if (GPS.month == 7) {
    NDay = GPS.day + 181; // Figure out the day number
  }

  if (GPS.month == 8) {
    NDay = GPS.day + 212; // Figure out the day number
  }

  if (GPS.month == 9) {
    NDay = GPS.day + 243; // Figure out the day number
  }

  if (GPS.month == 10) {
    NDay = GPS.day + 273; // Figure out the day number
  }

  if (GPS.month == 11) {
    NDay = GPS.day + 304; // Figure out the day number
  }

  if (GPS.month == 12) {
    NDay = GPS.day + 334; // Figure out the day number
  }

  Serial.print("This is the number od day from January 1st : ");
  Serial.println(NDay);

  return NDay;

} */

float _Angle (int N) {
  float _aA = 0;
  _aA = (360*(N-1) / 365.242);
  return _aA;
}


float _eqOfTime (float x) {
  float _eot=0;
  _eot = ( (0.258*cos(radians(x)))-(7.416*sin(radians(x)))-(3.648*cos(radians(2*x)))-(9.228*sin(radians(2*x))) );
  return _eot;  // equation of time returns in minutes
  
}

float hrAngle (float HA) {
   HA = 0;
  HA = 15 * (totalTimeH - 12);
  return HA;
}

/*void Solar_Time (float _eot , float _aA) {

  // The Equation Of Time or called the EOT is the difference between mean solar time and true solar time.
  // Since Solar time is based on the sun being due south at 12 in the noon
  // The formula for the EOT is : EOT = 0.258cos(x) - 7.416sin(x) - 3.648cos(2x) - 9.228sin(2x) where x = 360(NDay-1)/365.242
  // All I need to do is to figure out the solar time right now is to first get the gps to print me the clock time for right now
  // Next I need to do the EOT and add or subtract that from the clock time noon which is 12:00
  // After that, I now the solar hour noon time and then I subtract the time right now from the clock time noon which is 12:00

  int utcHr = GPS.hour; // That gives me the UTC hour
  int utcMn = GPS.minute; // That gives me the UTC minute

  int TODAY = 274; // change this using the NDay function

  int TZTSolarNoonH = utcHr - 12; // Time Zones to solar Noon Hour
  int TZTSolarNoonM = utcMn - 00; // Time Zones to solar noon Minutes

  float Degrees_Away;

  float degTTM = (15*TZTSolarNoonH + float (TZTSolarNoonM/4)); // How many degrees it is away at from UTC
  
  if (longitude > degTTM) {
     Degrees_Away = longitude - degTTM; // Away form our location
  }

  if (longitude < degTTM) {
     Degrees_Away = degTTM - longitude;
  }
  

  float minutes = ((int(Degrees_Away)%15)*4); // Minutes process
  float hours = ((Degrees_Away - minutes)/15);
  
  float clockTimeHours = 12 + hours;
  float clockTimeMinutes = 60 + minutes;
  clockTimeMinutes = clockTimeMinutes/60;
  float TotalHours1 = clockTimeMinutes + clockTimeHours;


  float eq = _eqOfTime(_Angle(TODAY));
  eq = eq/60;
  float TotalHours = TotalHours1 + eq; // total hours of solar time 

  float TotalHoursM = TotalHours*60;
  int SolarM = int (TotalHoursM)%60;
  int SolarH = ((TotalHoursM - SolarM)/60);

  totalTimeH = SolarH + (SolarM/60);

  

  

  delay(10000);
  
  
}

float Declinaiton_Angle () {

  float decAngle = asin(radians(0.39795*cos(radians(NDay - 173))));
  return decAngle;
  
}

float Alt_Angle(float Alt) {

  //Alt = 90 - latitude +- decAngle
  if(
  
}
*/ 




