
float azimuth_angle;
float altAngle = 34;
float Lat = -96;
float dec_angle = 25.63434;
float HrAngle = 15;

void setup() {
  
  Serial.begin(9600);

}

void loop() {

 /* azimuth_angle1 = sin(radians(dec_angle))*cos(radians(Lat));
  azimuth_angle2 = cos(radians(dec_angle))*cos(radians(HrAngle))*sin(radians(Lat));
  azimuth_angle3 = azimuth_angle1 - azimuth_angle2;
  azimuth_angle4 = azimuth_angle3 / cos(radians(altAngle));
  azimuth_angle5 = acos(azimuth_angle4);
  azimuth_angle= degrees(azimuth_angle5); */

  azimuth_angle = degrees(acos(radians(sin(radians(dec_angle))*cos(radians(Lat)) - cos(radians(dec_angle))*cos(radians(HrAngle))*sin(radians(Lat)))/cos(radians(altAngle))));
  
  Serial.print("This is the azimuth angle : ");
  Serial.println(azimuth_angle);  

}







