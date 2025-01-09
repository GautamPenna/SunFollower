#include <Servo.h> // Open up the servo motor library.


int SunriseHtime; 
int sunriseSecondVariable; 
int SunriseMtime; 
int sunriseMsecond;

int ServoPin = 9; 

int sunsetHours; 
int sunsetMinutes; 

int Time_In_Between_Hours;  
int TimeInHoursM; 
int Time_In_Between_Minutes;
int Total_Time; 
int TotalTime_Number; 
int Angle; 

int i; 

int noteLED = 13; 

int oneTWO; 
int twoTWO; 

int TIPD;
int TIPD2;


Servo sunFollower ;

void setup() { 

  Serial.begin(115200); 
  sunFollower.attach(ServoPin); 
  

  pinMode(noteLED , OUTPUT); 
  
  
  Serial.println("When does the Sun rise (SunriseHtime time) at your current location: "); 
  while (Serial.available()==0 ){} 
  SunriseHtime = Serial.parseInt(); 

  Serial.println("When does the Sun rise (SunriseMtime time) at your current location: "); 
  while (Serial.available()==0 ){} 
  SunriseMtime = Serial.parseInt(); 

  
  Serial.println("When does the Sun set (sunsetHours time) at your current location: "); 
  while (Serial.available()==0 ){} 
  sunsetHours = Serial.parseInt(); 

  Serial.println("When does the Sun set (sunsetMinutes time) at your current location: "); 
  while (Serial.available()==0 ){} 
  sunsetMinutes = Serial.parseInt(); 

  
 
}

void loop() {

  //sunFollower.write(78);

  
  Time_In_Between_Hours = sunsetHours - SunriseHtime; 
  Serial.print("The time in between your sunrise and sunset is : "); 
  Serial.println(Time_In_Between_Hours);

  
  TimeInHoursM = Time_In_Between_Hours*60; 
  Serial.print("The time in between your sunrise and sunset in minutes is : "); 
  Serial.println(TimeInHoursM);

  
  Time_In_Between_Minutes = sunsetMinutes - SunriseMtime; 
  Serial.print("The time in between your SunriseMtime and sunsetMinutes : ");
  Serial.println(Time_In_Between_Minutes);
  

  Total_Time = TimeInHoursM + Time_In_Between_Minutes; 
  
  Serial.print("Your Total_Time is : ");
  Serial.println(Total_Time);

  
  oneTWO = Total_Time/160;   
  Serial.print("Your first two is :  ");
  Serial.println(oneTWO);

  twoTWO = 2*oneTWO;
  Serial.print("Your second two is :  ");
  Serial.println(twoTWO);
  

  TotalTime_Number = Total_Time/twoTWO; 
  Serial.print("Your TotalTime_Number is : ");
  Serial.println(TotalTime_Number);

  
  Angle = 10; 
  sunFollower.write(Angle);

  TIPD = Total_Time/160; 
  TIPD2 = TIPD*2; 

  

  

  sunriseSecondVariable = SunriseHtime;
  sunriseMsecond = SunriseMtime;
    
  


  i=0;

  while (TotalTime_Number!= 0) { 
    Angle = Angle + 2;
    
    sunFollower.write(Angle);
    
    digitalWrite(noteLED , HIGH);
    
    Serial.print(" You are on : ");
    Serial.println(TotalTime_Number);
    Serial.print("The time is : " );
    Serial.print(sunriseSecondVariable);
    Serial.print(" : ");
    Serial.println(sunriseMsecond);

    sunriseMsecond = sunriseMsecond + TIPD2;
    if (sunriseMsecond = 60) {
      sunriseSecondVariable = sunriseSecondVariable+1;
      sunriseMsecond = 0;
      
    }

    else if(sunriseSecondVariable == sunsetHours and sunriseMsecond == sunsetMinutes) {
      Serial.print("That was one day. Good Night");
    }
        
    Serial.print("The angle for you servo is : ");
    Serial.println(Angle);
    TotalTime_Number = TotalTime_Number - 1;
    //delay(twoTWO);
    delay(1000); 

    digitalWrite(noteLED , LOW);
    
}
  
 
    digitalWrite(noteLED , HIGH);
  

   delay(2000); 

   digitalWrite(noteLED , LOW);
  // sunFollower.write(10);
  

  
}
