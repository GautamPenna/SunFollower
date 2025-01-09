#include <Adafruit_GPS.h> // Include the Adafruit GPS library.
#include <SoftwareSerial.h> // Include the software serial library
#include <SD.h> // Load the SD card Library
#include <SPI.h> // Loa the SPI library



SoftwareSerial mySerial(3,2); // Setting up the new serial port to communicate to the GPS tracker
Adafruit_GPS GPS(&mySerial); // Making the GPS object

String NMEA1; // Variable for first NMEA sentence
String NMEA2; // Variable for second NMEA sentence
char cHar; // To read the charcters from the GPS

int chipSelect = 4; // Chip select is connect to pin 4
File myGPSdata; // create variable for sd card object

int latitude;
int longitude;
int hNS;
int hEW;
  
  
    
 
void setup() {
 
  Serial.begin(115200); // Turn on serial moniter
  GPS.begin(9600); // Turn on GPS at 9600 baud rate
  GPS.sendCommand("$PGCMD,33,0*6D"); // turn off anteane update nusience data
  GPS.sendCommand(PMTK_SET_NMEA_OUTPUT_RMCGGA); //Tell GPS we want only $GPRMC and $GPGGA NMEA sentences`
  GPS.sendCommand(PMTK_SET_NMEA_UPDATE_1HZ); // I only want 1 hz
  
  delay(1000); // Delay a 1000 milliseconds to give it a chance to digest all of the commands sent to it

  pinMode(10, OUTPUT); // Must keep pin 10 open to make it happy
  SD.begin(chipSelect); // Connected to chipSelect also pin 4

  if(SD.exists("NMEA.txt")) { // Check to see if file is there
    SD.remove("NMEA.txt"); // Remove the existing file

  }

  if(SD.exists("GPSdata.txt")) { // Check to see if file is there
    SD.remove("GPSdata.txt"); // Remove the existing file

  }

  if(SD.exists("PData.txt")) { // Check to see if file is there
    SD.remove("PData.txt"); // Remove the existing file

  }

}

void loop() {

  getGPS(); // Make a function
  delay(900); // delay
  if(GPS.fix == 1) { // Only log data if you have a fix

    myGPSdata = SD.open("NMEA.txt",FILE_WRITE); // Open it for writing
    myGPSdata.println(NMEA1); // print the first sentence
    myGPSdata.println(NMEA2); // print the second sentence
    myGPSdata.close(); // close the file

 
    myGPSdata = SD.open("GPSdata.txt",FILE_WRITE); // open the gps data text on sd card
    myGPSdata.print(GPS.latitude,4); // print the latitude on the sd card
    myGPSdata.print(GPS.lat); // print the hemisphere
    myGPSdata.print(" , ");
    myGPSdata.print(GPS.longitude,4); // prin the longitude
    myGPSdata.print(GPS.lon); // print the hemisphere
    myGPSdata.print(" , ");
    myGPSdata.println(GPS.altitude) ; // print the altitude
    myGPSdata.close(); // close the file
    
    latitude = (GPS.latitude,4); // Get the latitude 
    longitude = (GPS.longitude,4); // Get the longitude
    hNS = GPS.lat; // North Hemisphere or South Hemisphere
    hEW = GPS.lon; // West  Hemisphere or East  Hemisphere

    Serial.println("Got to the Opening Step.");
    
    myGPSdata = SD.open("PData.txt",FILE_WRITE); // open the gps data text on sd card
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
    Serial.print(latitude); // print the coordinates
    Serial.print(hNS);
    Serial.print(" , ");
    Serial.print(longitude);        
    Serial.print(hEW);
  }

}

void getGPS() {
  eraseGPS();

  while(!GPS.newNMEAreceived()) {

    cHar = GPS.read(); // Read from the GPS sentences. Loop until you have a good NMEA sentence 
  }

   GPS.parse(GPS.lastNMEA()); // Parse that last good NMEA sentence
   NMEA1 = GPS.lastNMEA(); // Put the variable into the string

   while(!GPS.newNMEAreceived()) {

    cHar = GPS.read(); // Read from the GPS sentences. Loop until you have a good NMEA sentence   
  }

 GPS.parse(GPS.lastNMEA()); // Parse that last good NMEA sentence
 NMEA2 = GPS.lastNMEA(); // Put the variable into the string
 
 Serial.println(NMEA1);
 Serial.println(NMEA2);
 Serial.println("   "); // print a space
}

void eraseGPS() {

  while(!GPS.newNMEAreceived()) {

    cHar = GPS.read(); // Read from the GPS sentences. Loop until you have a good NMEA sentence
  }
   GPS.parse(GPS.lastNMEA()); // Parse that last good NMEA sentence
}
  

