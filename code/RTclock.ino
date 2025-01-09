 
#include <RTClib.h>
#include<SD.h>
#include<SPI.h>
 
// Date and time functions using a DS3231 RTC connected via I2C and Wire lib
#include "RTClib.h"
 
RTC_DS3231 rtc;
const int csPIN = 53; //Arduino MEGA
File dataCAP;
 
char daysOfTheWeek[7][12] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};
 
float vBridge (float V2, float R1, float R2 ) {
  return ((V2)/(R2/(R1+R2)));
}
 
const int Vfixed=3; // Using Pin3 for fixed panel
const int Vtracer=4; //Using Pin4 for Solar Tracer
 
 
void setup() {
  Serial.begin(9600);
  pinMode(csPIN, OUTPUT);
  delay(3000); // wait for console opening
 
  if (! rtc.begin()) {
    Serial.println("Couldn't find RTC");
    while (1);
  }
 
  //  if (rtc.lostPower()) {
  //    Serial.println("RTC lost power, lets set the time!");
  //    // following line sets the RTC to the date & time this sketch was compiled
  //    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  //    // This line sets the RTC with an explicit date & time, for example to set
  //    // January 21, 2014 at 3am you would call:
  //    // rtc.adjust(DateTime(2014, 1, 21, 3, 0, 0));
  //  }
 
  //rtc.adjust(DateTime(2019, 10, 15, 21,20,0));
 
  if (SD.begin())
  {
    Serial.println("SD card is ready to use.");
  } else
  {
    Serial.println("SD card initialization failed");
    //return;  Exit program?
  }
 
  // Create/Open file
 
  DateTime fRTC = rtc.now();
  //String FName = String("STROBE_" + String(fRTC.timestamp(DateTime::TIMESTAMP_DATE)) + ".csv");  (Can only be in 8.3 format)
  String FName = String(String (fRTC.year()) + String (fRTC.month()) + String (fRTC.day()) + ".csv");
  dataCAP = SD.open(FName, FILE_WRITE);
 
  if (dataCAP) {
    Serial.print("Opening Data Capture File: ");
    Serial.println(FName);
    while (dataCAP.available()) {
      Serial.write(dataCAP.read());
    }
    dataCAP.close();
  } else {
    Serial.print("Error Opening file: ");
    Serial.println(FName);
  }
  pinMode (Vfixed,INPUT);  //defining the pin for Fixed Read
  pinMode (Vtracer,INPUT); //defining the pin for Tracing Read
}
 
int counter = 1;
 
void loop() {
 
  DateTime cRTC = rtc.now();
  String cDate = String(cRTC.timestamp(DateTime::TIMESTAMP_DATE));
  String cTime = String(cRTC.timestamp(DateTime::TIMESTAMP_TIME));
  String cDow =  daysOfTheWeek[cRTC.dayOfTheWeek()];
  float cTemp = rtc.getTemperature();
  //float cTemp=3.87;
 
  //String cName = String("STROBE_" + String(cRTC.timestamp(DateTime::TIMESTAMP_DATE)) + ".csv"); (Can only be in 8.3 format);
  String cName = String(String (cRTC.year()) + String (cRTC.month()) + String (cRTC.day()) + ".csv");
 
  //Calculate Voltage, Current and Power for Fixed Panel
  float Vef=0; float Ief=0; float Pef=0;
  int Vef_reading=0;
  Vef_reading = analogRead(Vfixed);
  float V2a=0; float R1a=2214.0; float R2a=22198.0;
  V2a = (Vef_reading * 5.0)/1024;
 
  Vef = vBridge (V2a, R1a, R2a);
  Ief = V2a/R2a*1000; //milli Amps
  Pef = Vef * Ief; //milliWatts
 
 
  //Calculate Voltage, Current and Power for Solar Tracker
  float Vest=0; float Iest=0; float Pest=0;
  int Vest_reading=0;
  Vest_reading = analogRead(Vtracer);
  float V2b=0; float R1b=2166.0; float R2b=21958.0;
  V2b = (Vest_reading * 5.0)/1024;
 
  Vest = vBridge (V2b, R1b, R2b);
  Iest = V2b/R2b*1000; //milli Amps
  Pest = Vest * Iest; //milli watts
 
 
  dataCAP = SD.open(cName, FILE_WRITE);
  if (dataCAP) {
    Serial.print(counter);
    Serial.println(" Printing Date and Time");
 
    dataCAP.print(cDate); dataCAP.print(",");        dataCAP.print(cDow); dataCAP.print(","); dataCAP.print(cTime);
    dataCAP.print(",");   dataCAP.print((cTemp*9/5)+32); dataCAP.print(",");  dataCAP.print(Vef); dataCAP.print(",");
    dataCAP.print(Ief);   dataCAP.print(",");        dataCAP.print(Pef);  dataCAP.print(","); dataCAP.print(Vest);
    dataCAP.print(",");   dataCAP.print(Iest);       dataCAP.print(",");  dataCAP.print(Pest); dataCAP.print(",");
    if (Pef <=0 ){
      dataCAP.print("0%");
    } else {
      dataCAP.print ((((Pest-Pef)/Pef)*100));
      dataCAP.print ("%");
    }
   
    dataCAP.println();
 
    dataCAP.close();
  } else {
    Serial.println("Could not Open the file");
  }
  delay(600000);
  counter++;
  if (counter == 10) {
    dataCAP = SD.open(cName, FILE_READ);
    if (dataCAP) {
      Serial.print("Opening Data Capture File: ");
      Serial.println(cName);
      while (dataCAP.available()) {
        Serial.write(dataCAP.read());
      }
      dataCAP.close();
    } else {
      Serial.print("Error Opening file: ");
      Serial.println(cName);
    }
    counter = 1;
  }
 
}
