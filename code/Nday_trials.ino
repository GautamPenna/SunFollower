int month[] = {1,2,3,4,5,6,7,8,9,10,11,12};
int date[] = {1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31};
int year[] = {1980,1981,1982,1983,1984,1985,1986,1987,1988,1989,1990,1991,1992,1993,1994,1995,1996,1997,1998,1999,2000,2001,2002,2003,2004,2005,2006,2007,2008,2009,2010,2011,2012,2013,2014,2015,2016,2017};
int Nday;
void setup() {
  
  Serial.begin(9600);

}

void loop() {

  //int Month = month[random(0,11)];
  //int Date = date[random(0,30)];
  //int Year = year[random(0,35)];

int Month = 3;
int Date =  1;

 
for (int Year=1899; Year< 2018; Year++) {
  


  if (Month == 1) {
    Nday = Date;
  }

  if (Month == 2) {
    Nday = Date + 31;
  }
  
  if (Month == 3) {
    Nday = Date + 59;

    if(Year%4 == 0) {
      Nday = Date + 60;
    }
    
    if(Year%100 == 0) {
      Nday = Date + 59;
    }

    if(Year%400 == 0) {
      Nday = Date + 60;
    }
    
  }

  if (Month == 4) {
    Nday = Date + 90;

    if(Year%100 == 0) {
      Nday = Date + 90;
    }

    if(Year%4 == 0) {
      Nday = Date + 91;
    }

    if(Year%400 == 0) {
      Nday = Date + 91;
    }
  }

  if (Month == 5) {
   Nday = Date + 120;
   
   if(Year%100 == 0) {
      Nday = Date + 120;
    }

    if(Year%4 == 0) {
      Nday = Date + 121;
    }

    if(Year%400 == 0) {
      Nday = Date + 121;
    }
  }

  if (Month == 6) {
    Nday = Date + 151;
    
    if(Year%100 == 0) {
      Nday = Date + 151;
    }

    if(Year%4 == 0) {
      Nday = Date + 152;
    }

    if(Year%400 == 0) {
      Nday = Date + 152;
    }
  }
  
  if (Month == 7) {
    Nday = Date + 181;
    
    if(Year%100 == 0) {
      Nday = Date + 181;
    }

    if(Year%4 == 0) {
      Nday = Date + 182;
    }

    if(Year%400 == 0) {
      Nday = Date + 182;
    }
  }

  if (Month == 8) {
   Nday = Date + 212;
   
   if(Year%100 == 0) {
      Nday = Date + 212;
   }
    if(Year%4 == 0) {
      Nday = Date + 213;
    }

    if(Year%400 == 0) {
      Nday = Date + 213;
    }
  }

  if (Month == 9) {
    Nday = Date + 243;
    
    if(Year%100 == 0) {
      Nday = Date + 243;
    }

    if(Year%4 == 0) {
      Nday = Date + 244;
    }

    if(Year%400 == 0) {
      Nday = Date + 244;
    }
  }

  if (Month == 10) {
    Nday = Date + 273;
    
    if(Year%100 == 0) {
      Nday = Date + 273;
    }

    if(Year%4 == 0) {
      Nday = Date + 274;
    }

    if(Year%400 == 0) {
      Nday = Date + 274;
    }
  }
  

  if (Month == 11) {
    Nday = Date + 304;
    
    if(Year%100 == 0) {
      Nday = Date + 304;
    }

    if(Year%4 == 0) {
      Nday = Date + 305;
    }

    if(Year%400 == 0) {
      Nday = Date + 305;
    
  }


  }

  if (Month == 12) {
    Nday = Date + 334;
    
    if(Year%100 == 0) {
      Nday = Date + 334;
    }

    if(Year % 4 == 0) {
      Nday = Date + 335;
    }

    if(Year%400 == 0) {
      Nday = Date + 335;
    }
  }
  Serial.print("This is my Date : ");
  Serial.print(Month);
  Serial.print("/");
  Serial.print(Date);
  Serial.print("/");
  Serial.print(Year);
  
  Serial.print("This is the day number from January 1st : ");
  Serial.println(Nday);
  
  delay(30);

  

delay (30);


}
delay(30000);

}
