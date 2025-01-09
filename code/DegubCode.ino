#include <SD.h> //Load SD card library
#include<SPI.h> //Load SPI Library

int chipSelect = 4; //chipSelect pin for the SD card Reader
File mySensorData; //Data object you will write your sesnor data to
int i = 0;

void setup() {
  
  pinMode(10, OUTPUT); //Must declare 10 an output and reserve it to keep SD card happy
  SD.begin(chipSelect); //Initialize the SD card reader
  Serial.begin(9600);

  if (SD.exists("Debug.txt")) { //Delete old data files to start fresh
    SD.remove("Debug.txt");
  }
  
  mySensorData = SD.open("Debug.txt", FILE_WRITE);  



  

}

void loop() {
 Serial.println("=======================================================");
  mySensorData.println("=======================================================");
  while (i != 10 ) {
    mySensorData.print("2 x ");
    mySensorData.print(i);
    mySensorData.print(" = ");
    mySensorData.println(2*i);  
    Serial.print("2 x ");
    Serial.print(i);
    Serial.print(" = ");
    Serial.println(2*i); 
    i=i+1; 
   
  }

  Serial.println("======================Closed=====================");
  mySensorData.println("=======================================================");
  mySensorData.close();
  

  }
