#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define OLED 4
Adafruit_SSD1306 display(OLED);

#define NUMFLAKES 10
#define XPOS 0
#define YPOS 1
#define DELTAY 2


#define LOGO16_GLCD_HEIGHT 16 
#define LOGO16_GLCD_WIDTH  16


void setup() {

  Serial.begin(9600);
  display.begin(SSD1306_SWITCHCAPVCC, 0x3c);
  display.display();
  display.clearDisplay();

  #if (SSD1306_LCDHEIGHT != 64)
  #error("Height incorrect, please fix Adafruit_SSD1306.h!");
  #endif

}

void loop() {

  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setCursor(0,0);
  display.print("AZI:      175.3532");
  display.setCursor(0,32);
  display.print("ALT:      50.4632");
  display.setCursor(0,0);
  display.display();
  delay(1000);
  
  delay(1000);
  Serial.println("It is going through the loop.");

  delay(5000);
  display.clearDisplay();

  display.print("Location: Dallas");
  display.setCursor(0,32);
  display.print("LCT:      12:00:00");
  display.setCursor(0,0);
  display.display();
  delay(1000);
  
  delay(1000);
  Serial.println("It is going through the loop.");

  delay(5000);
  display.clearDisplay();

  display.print("LST:      11:53:37");
  display.setCursor(0,32);
  display.print("DecAngle: 32.5646");
  display.setCursor(0,0);
  display.display();
  delay(1000);
  
  delay(1000);
  Serial.println("It is going through the loop.");

  delay(5000);
  display.clearDisplay();

  display.print("HrAngle : 54.6324");
  display.setCursor(0,32);
  display.print("NDAY: 235");
  display.setCursor(0,50);
  display.print("EOT: 16.43");
  display.setCursor(0,0);
  display.display();
  delay(1000);
  
  delay(1000);
  Serial.println("It is going through the loop.");

  delay(5000);
  display.clearDisplay();
  
  
}


