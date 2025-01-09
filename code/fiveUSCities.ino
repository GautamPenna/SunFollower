String PlaceNames[ ]  = {"Tokyo      ", "Hong Kong        ", "Hyderabad        ", "Tel Aviv ", "Vienna      ", "London     ", "St. Johns     ", "New York       ", "Dallas        ", "Denver        ", "San Francisco     ", "Honolulu    "}; // Different places to try out my test code so I can mke sure that my program will work anywhere on earth.

void setup() {

  Serial.begin(115200); // Turn on the serial moniter for communication
  //Serial.println("Location    |  NDay  |  Date  |  Time  |  Latitude  |  Longitude  |  EOT  |  aA  |  Declination Angle  |  Hr  |  Alt  |  Az  |  SolarH  |  SolarM  |  Hr"); // Going to print the leading line so I can see where ecah number fits into its seperate catagory
}

void loop() {

  int year  = 2017;   // print out a test information so I can check on websites if my calculation are right for that day
  int month = 1;     // print out a test information so I can check on websites if my calculation are right for that day
  int day   = 21;      // print out a test information so I can check on websites if my calculation are right for that day
  int utcHr = 8;     // print out a test information so I can check on websites if my calculation are right for that day
  int utcMn = 00;      // print out a test information so I can check on websites if my calculation are right for that day
  float latitude = 0; // print out a test information so I can check on websites if my calculation are right for that day
  float longitude = 0;// print out a test information so I can check on websites if my calculation are right for that day

  for (int k = 0; k < 11; k++) {  // start the for loop so it can repeat the process that is inside the loop for the number of cities that there are (declared above)

    if ( PlaceNames[k] == "Tokyo      " ) { // All of the code until the end of Jerusalem is basically declaring all the cities coordiantes and information about it
      latitude = 40.7128;
      longitude = -74.0059;
    }
    if (PlaceNames[k] == "Hong Kong        " ) {
      latitude = 32.7767;
      longitude = -96.7970;
    }
    if (PlaceNames[k] == "Hyderabad        ") {
      latitude = 39.7392;
      longitude = -104.9903;
    }
    if (PlaceNames[k] ==  "Tel Aviv " ) {
      latitude = 37.7749;
      longitude = -122.4194;
    }
    if (PlaceNames[k] == "Vienna      ") {
      latitude = 21.3069;
      longitude = -157.8583;
    }
    if (PlaceNames[k] == "London     ") {
      latitude = 22.3964;
      longitude = 114.1095;
    }
    if (PlaceNames[k] == "St. Johns     ") {
      latitude = 17.3850;
      longitude = 78.4867;
    }
    if (PlaceNames[k] == "New York        ") {
      latitude = 55.7558;
      longitude = 37.6173;
    }
    if (PlaceNames[k] == "Dallas        ") {
      latitude = 52.5200;
      longitude = 13.4050;
    }
    if (PlaceNames[k] == "Denver        ") {
      latitude = 51.5074;
      longitude = 0.1278;
    }
    if (PlaceNames[k] == "San Francisco     ") {
      latitude = 51.4826;
      longitude = 0.0077;
    }
    if (PlaceNames[k] == "Honolulu     ") {
      latitude = 31.7683;
      longitude = 35.2137;
    }

    // Before you read on to the rest of the comments, go down to the part where it says "Declaring All The Function Neccessary For This Code" and read all of that section first
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
    }

    if (SolarM >= 60) {
      SolarM = SolarM - 60;
      SolarH = SolarH + 1;
    }

    if (SolarH >= 24) {
      SolarH = SolarH - 24;
    }

      Serial.print("==============");
      Serial.print(PlaceNames[k]);
      Serial.println("==============");
      Serial.print("==============");
      Serial.print(latitude);
      Serial.print(",");
      Serial.print(longitude);
      Serial.println("==============");

    for (int x = 0; x < 12; x++) { // this for is for the different dates


      for (int y = 0; y < 12; y++) { // this for is for the different times

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
        
        int lcHr = lctConH(utcHr , longitude);
        int lcMn = utcMn;
        
        if (SolarM < 0) { // Caculating the solar m time from the if conditional statements
          SolarH = 23;
          SolarM = 60 - SolarM;
        }

        if (SolarM >= 60) {
          SolarM = SolarM - 60;
          SolarH = SolarH + 1;
        }

        if (SolarH >= 24) {
          SolarH = SolarH - 24;
        }

        Serial.print(month);
        Serial.print("/");
        Serial.print(day);
        Serial.print("/");
        Serial.print(year);
        Serial.print("  ");
        Serial.print(SolarH);
        Serial.print(":");
        Serial.print(SolarM);
        Serial.print("  ");
        Serial.print(utcHr);
        Serial.print(":");
        Serial.print(utcMn);
        Serial.print("  ");
        Serial.print(lcHr);
        Serial.print(":");
        Serial.print(lcMn);
        Serial.print("  ");
        Serial.print("Hour Angle : ");
        Serial.print(HrAngle);
        Serial.print(" NDAY : ");
        Serial.print(Nday);
        Serial.print("  ");
        Serial.print(" Declination : ");
        Serial.print(decAngle);
        Serial.print(" Altitude : ");
        Serial.print(AltAngle);
        Serial.print(" Azimuth : ");
        Serial.print(AzAngle);
        Serial.print(" EOT : ");
        Serial.print(EOT);
        Serial.print(" Axial Angle : ");
        Serial.println(aA);

        utcHr = utcHr + 1;

      }

      month = month + 1;
      utcHr = 8;

      Serial.println("  ");
      
    }

    delay(1000);
    Serial.println ();

    month = 1;
  }

}



// Declaring All The Function Neccessary For This Code is below
// This section will explain you how to declare functions all the functions I used and why I used them

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
  }

  if (Month == 2) {
    Nday = Day + 31;
  }

  if (Month == 3) {
    Nday = Day + 59;

    if (Year % 4 == 0) {
      Nday = Day + 60;
    }

    if (Year % 100 == 0) {
      Nday = Day + 59;
    }

    if (Year % 400 == 0) {
      Nday = Day + 60;
    }

  }

  if (Month == 4) {
    Nday = Day + 90;

    if (Year % 100 == 0) {
      Nday = Day + 90;
    }

    if (Year % 4 == 0) {
      Nday = Day + 91;
    }

    if (Year % 400 == 0) {
      Nday = Day + 91;
    }
  }

  if (Month == 5) {
    Nday = Day + 120;

    if (Year % 100 == 0) {
      Nday = Day + 120;
    }

    if (Year % 4 == 0) {
      Nday = Day + 121;
    }

    if (Year % 400 == 0) {
      Nday = Day + 121;
    }
  }

  if (Month == 6) {
    Nday = Day + 151;

    if (Year % 100 == 0) {
      Nday = Day + 151;
    }

    if (Year % 4 == 0) {
      Nday = Day + 152;
    }

    if (Year % 400 == 0) {
      Nday = Day + 152;
    }
  }

  if (Month == 7) {
    Nday = Day + 181;

    if (Year % 100 == 0) {
      Nday = Day + 181;
    }

    if (Year % 4 == 0) {
      Nday = Day + 182;
    }

    if (Year % 400 == 0) {
      Nday = Day + 182;
    }
  }

  if (Month == 8) {
    Nday = Day + 212;

    if (Year % 100 == 0) {
      Nday = Day + 212;
    }
    if (Year % 4 == 0) {
      Nday = Day + 213;
    }

    if (Year % 400 == 0) {
      Nday = Day + 213;
    }
  }

  if (Month == 9) {
    Nday = Day + 243;

    if (Year % 100 == 0) {
      Nday = Day + 243;
    }

    if (Year % 4 == 0) {
      Nday = Day + 244;
    }

    if (Year % 400 == 0) {
      Nday = Day + 244;
    }
  }

  if (Month == 10) {
    Nday = Day + 273;

    if (Year % 100 == 0) {
      Nday = Day + 273;
    }

    if (Year % 4 == 0) {
      Nday = Day + 274;
    }

    if (Year % 400 == 0) {
      Nday = Day + 274;
    }
  }


  if (Month == 11) {
    Nday = Day + 304;

    if (Year % 100 == 0) {
      Nday = Day + 304;
    }

    if (Year % 4 == 0) {
      Nday = Day + 305;
    }

    if (Year % 400 == 0) {
      Nday = Day + 305;

    }


  }

  if (Month == 12) {
    Nday = Day + 334;

    if (Year % 100 == 0) {
      Nday = Day + 334;
    }

    if (Year % 4 == 0) {
      Nday = Day + 335;
    }

    if (Year % 400 == 0) {
      Nday = Day + 335;
    }
  }

  return Nday; // return the Nday function

}
