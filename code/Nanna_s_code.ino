int STMZMeridian[] = {180, 165, 150, 135, 120, 105, 90, 75, 60, 45, 30, 15, 0, -15, -30, -45, -60, -75, -90, -105, -120, -135, -150, -165};


void setup() {
  // put your setup code here, to run once:

  Serial.begin (9600);

}

void loop() {
  // put your main code here, to run repeatedly:

  float _sT = 0;

  float _localLong = -96.6411;

  int utcHr = random(0, 23);
  float utcMn = random(0, 59);

  int localTHr = 0;
  int localTMn = 0;

  //int TZMer=STMZMeridian[utcHR];

  int mulFactor = 1;

  if ( STMZMeridian[utcHr] < 0) {
  mulFactor = -1;
  } else {
  mulFactor = 1;
  }

  float noonAtMer = STMZMeridian[utcHr] + ( float(utcMn) / 4 * mulFactor );

  float _sTCorrection = ( noonAtMer - _localLong) * 4;

  if ( noonAtMer < _localLong ) {

  float ahead = 720 + abs(_sTCorrection);

  Serial.println(ahead);

  localTHr = int(ahead) / 60;
  localTMn = ahead - localTHr * 60;

  } else {

  // you need calcluate;
  float behind = 720 - abs(_sTCorrection);

  localTHr = int(behind) / 60;
  localTMn = behind - localTHr * 60;

  if (localTHr < 0) {
    localTHr = 24 + localTHr;
  }

  if (localTMn < 0) {
    localTMn = 60 + localTMn;
  }



  }



  //Serial.println( utcMn/4 );

  char *Hr = malloc(4);
  char *Mn = malloc(4);

  sprintf(Hr, "%02d", utcHr);

  sprintf(Mn, "%02d", int(utcMn));

  Serial.print ("UTC Time: ");
  Serial.print (Hr);
  Serial.print (":");
  Serial.print (Mn);
  Serial.print (" ");

  Serial.print ("Noon at Meridian: ");
  Serial.print (noonAtMer);

  Serial.print ("   Correction Factor: ");
  Serial.print (_sTCorrection);

  Serial.print (" ");
  Serial.print ("Local Solar Time: ");
  Serial.print (localTHr);
  Serial.print (":");
  Serial.print (localTMn);
  Serial.println (" ");


  delay (10000);


}int STMZMeridian[] = {180, 165, 150, 135, 120, 105, 90, 75, 60, 45, 30, 15, 0, -15, -30, -45, -60, -75, -90, -105, -120, -135, -150, -165};


void setup() {
  // put your setup code here, to run once:

  Serial.begin (9600);

}

void loop() {
  // put your main code here, to run repeatedly:

  float _sT = 0;

  float _localLong = -96.6411;

  int utcHr = random(0, 23);
  float utcMn = random(0, 59);

  int localTHr = 0;
  int localTMn = 0;

  //int TZMer=STMZMeridian[utcHR];

  int mulFactor = 1;

  if ( STMZMeridian[utcHr] < 0) {
  mulFactor = -1;
  } else {
  mulFactor = 1;
  }

  float noonAtMer = STMZMeridian[utcHr] + ( float(utcMn) / 4 * mulFactor );

  float _sTCorrection = ( noonAtMer - _localLong) * 4;

  if ( noonAtMer < _localLong ) {

  float ahead = 720 + abs(_sTCorrection);

  Serial.println(ahead);

  localTHr = int(ahead) / 60;
  localTMn = ahead - localTHr * 60;

  } else {

  // you need calcluate;
  float behind = 720 - abs(_sTCorrection);

  localTHr = int(behind) / 60;
  localTMn = behind - localTHr * 60;

  if (localTHr < 0) {
    localTHr = 24 + localTHr;
  }

  if (localTMn < 0) {
    localTMn = 60 + localTMn;
  }



  }



  //Serial.println( utcMn/4 );

  char *Hr = malloc(4);
  char *Mn = malloc(4);

  sprintf(Hr, "%02d", utcHr);

  sprintf(Mn, "%02d", int(utcMn));

  Serial.print ("UTC Time: ");
  Serial.print (Hr);
  Serial.print (":");
  Serial.print (Mn);
  Serial.print (" ");

  Serial.print ("Noon at Meridian: ");
  Serial.print (noonAtMer);

  Serial.print ("   Correction Factor: ");
  Serial.print (_sTCorrection);

  Serial.print (" ");
  Serial.print ("Local Solar Time: ");
  Serial.print (localTHr);
  Serial.print (":");
  Serial.print (localTMn);
  Serial.println (" ");


  delay (10000);


}

