#include <Wire.h>
#include <Adafruit_MotorShield.h>


Adafruit_MotorShield AFMS = Adafruit_MotorShield();
Adafruit_StepperMotor *myMotor = AFMS.getStepper(200, 2); // The first number is the number of steps and the second number is the stepper number



void setup() {

  *myMotor.setSpeed(34);

}

void loop() {

  *myMotor.step(34,FORWARD,MICROSTEP);

}
