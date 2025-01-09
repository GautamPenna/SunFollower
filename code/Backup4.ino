#include <Servo.h> // Open up the servo motor library.


int SunriseHtime; // This is the variable that will give the user the Hour time for the Sunrise
int sunriseSecondVariable; // This is a second variable that will be equal to the sunrise H time so I can change this one and not the actual Sunrise variable
int SunriseMtime; // This is the variable that will give the user the Minute time for the sunrise
int sunriseMsecond; // This is the second variable that will be equal to the sunrise M variable

int ServoPin = 9; // This variable holds the servo pin number which is pin 9

int sunsetHours; // This is the variable that will give the user the Hour time for the sunset
int sunsetMinutes; // This is the variable that will give the user the minute time for the sunset

int Time_In_Between_Hours; // This is the time in between the hours of sunrise and sunset
int TimeInHoursM; // This is the time_in_Between_hours in minutes
int Time_In_Between_Minutes; // This is the time in between the minutes of the sunriseMtime and sunsetMinutes

int Total_Time; //This is the total time between sunrise and sunset
int TotalTime_Number; // This is the number of times the while loop will repeat

int Angle; // This is the starting angle which will be set to ten degrees because ot can wait 45 minutes and then start moving

int i; // This variable is the condition of the while loop

int noteLED = 13; // This is the pin number of the LED

int The_First_Step_To_Two_Degrees; // This variabe is the first step in making the two degree turn each time
int The_Second_Step_To_Two_Degrees; // This variabe is the second step in making the two degree turn each time

int TIPD; // This variable calculates the time increment per degree
int TIPD2; // This variable holds the time increment per degree

int Sun60;

Servo sunFollower ; // This declares the servo has an object called sunFollower

void setup() { 

  Serial.begin(250000); // Serial moniter is set to 115200 baud rate
  sunFollower.attach(ServoPin); // SunFollower is attached to the Servo Pin 9.
  

  pinMode(noteLED , OUTPUT); // Declares the pinMode of the object
  
  
  Serial.println("When does the Sun rise (SunriseHtime time) at your current location: "); // Prints the statement to the Serial Moniter
  while (Serial.available()==0 ){} // Asking if the user set any information
  SunriseHtime = Serial.parseInt(); // Taking the input and putting it into the variable

  Serial.println("When does the Sun rise (SunriseMtime time) at your current location: "); // Prints the statement to the Serial Moniter
  while (Serial.available()==0 ){} // Asking if the user set any information
  SunriseMtime = Serial.parseInt(); // Taking the input and putting it into the variable

  
  Serial.println("When does the Sun set (sunsetHours time) at your current location: "); // Prints the statement to the Serial Moniter
  while (Serial.available()==0 ){} // Asking if the user set any information
  sunsetHours = Serial.parseInt(); // Taking the input and putting it into the variable

  Serial.println("When does the Sun set (sunsetMinutes time) at your current location: "); // Prints the statement to the Serial Moniter
  while (Serial.available()==0 ){} // Asking if the user set any information
  sunsetMinutes = Serial.parseInt(); // Taking the input and putting it into the variable

  
 
}

void loop() {

  //sunFollower.write(78);

  
  Time_In_Between_Hours = sunsetHours - SunriseHtime; // This statement is suntracting the time from sunrise Hour time and sunset Hour time
  Serial.print("The time in between your sunrise and sunset is : "); // Prints the statement to the Serial Moniter
  Serial.println(Time_In_Between_Hours);// Prints the statement to the Serial Moniter

  
  TimeInHoursM = Time_In_Between_Hours*60; // This statement is taking that difference and converting it into minutes
  Serial.print("The time in between your sunrise and sunset in minutes is : ");// Prints the statement to the Serial Moniter 
  Serial.println(TimeInHoursM);// Prints the statement to the Serial Moniter

  
  Time_In_Between_Minutes = sunsetMinutes - SunriseMtime; // This is subtracting the minute times from the minute time from the sunrise and sunset
  Serial.print("The time in between your SunriseMtime and sunsetMinutes : ");// Prints the statement to the Serial Moniter
  Serial.println(Time_In_Between_Minutes);// Prints the statement to the Serial Moniter
  

  Total_Time = TimeInHoursM + Time_In_Between_Minutes; //This is adding both of the minute times from the above equations
  Serial.print("Your Total_Time is : ");// Prints the statement to the Serial Moniter
  Serial.println(Total_Time);// Prints the statement to the Serial Moniter

  
  The_First_Step_To_Two_Degrees = Total_Time/160; // This is first step to making every angle reach two degrees
  The_Second_Step_To_Two_Degrees = 2*The_First_Step_To_Two_Degrees; // This is the second step to making every angle reach two degrees
 
  TotalTime_Number=Total_Time/The_Second_Step_To_Two_Degrees;

  Serial.print("Your TotalTime_Number is : ");// Prints the statement to the Serial Moniter
  Serial.println(TotalTime_Number);// Prints the statement to the Serial Moniter

  
  Angle = 10; // Declares that the variable angle is always going to be equal to 10 degrees
  sunFollower.write(Angle); // The servo is set to the ten degree angle

  TIPD = Total_Time/160; 
  TIPD2 = TIPD*2; 
  Serial.println("The TIPD per 2 degrees is : ");
  Serial.println(TIPD2);

  

  

  sunriseSecondVariable = SunriseHtime;
  sunriseMsecond = SunriseMtime;
    
 


  i=0;

  while (TotalTime_Number != 0) { 
    Angle = Angle + 2;
    
    sunFollower.write(Angle);
    
    digitalWrite(noteLED , HIGH);
    
    Serial.print(" You are on : ");// Prints the statement to the Serial Moniter
    Serial.println(TotalTime_Number);// Prints the statement to the Serial Moniter
    Serial.print("The time is : " );// Prints the statement to the Serial Moniter
    Serial.print(sunriseSecondVariable);// Prints the statement to the Serial Moniter
    Serial.print(" : ");// Prints the statement to the Serial Moniter
    Serial.println(sunriseMsecond);// Prints the statement to the Serial Moniter

    sunriseMsecond = sunriseMsecond + TIPD2; // Adding the time to the current time
    Serial.print("The sunrise Millisecods in this while loop is : ");
    Serial.println(sunriseMsecond);
    
    if (sunriseMsecond >= 60) {
      sunriseSecondVariable = sunriseSecondVariable + 1;
      sunriseMsecond = sunriseMsecond - 60;

      }
    
    
        
    Serial.print("The angle for you servo is : ");// Prints the statement to the Serial Moniter
    Serial.println(Angle);// Prints the statement to the Serial Moniter
    TotalTime_Number = TotalTime_Number - 1;
    //delay(The_Second_Step_To_Two_Degrees_Degrees_Degrees);
    delay(1000); 

    digitalWrite(noteLED , LOW);
    
}
  
 
    digitalWrite(noteLED , HIGH);
  

   delay(2000); 

   digitalWrite(noteLED , LOW);
  // sunFollower.write(10);
  

  
}
