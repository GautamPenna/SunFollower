// This program is to convert the lcMn and lcHr into utcMn and utcHr.

void setup() {

  Serial.begin(9600);

}

void loop() {

  float local_long = -120;

  int lcHrs[] = {8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23};
  int lcMns[] = {10,20,30,40,50};
  int lcHr = lcHrs[random(0,16)];
  int lcMn = lcMns[random(0,4)];
  int utcMins = utcTime(local_long , lcHr , lcMn);
  int utcHr = utcMins/60;
  int utcMn = utcMins % 60;
  Serial.print(lcHr);
  Serial.print(":");
  Serial.print(lcMn);
  Serial.print("=================");
  Serial.print(utcHr);
  Serial.print(":");
  Serial.println(utcMn);
  delay(1000);
  

  
}

float utcTime(float local_long , int lcHr , int lcMn) {
  float timeZAway = local_long/15;
  float utcHr = float(lcHr) - timeZAway;
  float utcMins = utcHr*60 + float(lcMn);

  if (utcMins > 1440) utcMins=utcMins-1440;
  if (utcMins < 0)  utcMins=1440+utcMins;
  
  return utcMins;

  
  
}

