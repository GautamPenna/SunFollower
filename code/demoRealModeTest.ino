/* Program:
    (1) To demonstrate setting coordinates
    (2) To Issue a break statement
    (3) To run


*/


#define REAL_MODE 50
#define DEMO_SET 51
#define ResetButton 52


void setup() {
  // put your setup code here, to run once:

  Serial.begin(9600);
  Serial.println("Starting the Program");
  pinMode(DEMO_SET, INPUT);
  pinMode(REAL_MODE, INPUT);
  pinMode(ResetButton, INPUT);

  //digitalWrite(ResetButton,HIGH);
}

void loop() {
  // put your main code here, to run repeatedly:
  int inRealMode = digitalRead(REAL_MODE); //if True, Real Mode - else Demo Mode
  int inDemoSetMode = digitalRead(DEMO_SET); //if True DemoSet Mode, else Run Demo

  if (inRealMode) { //in Real Mode

   Serial.println ("in Real Mode");


  } else {  //in Demo Mode
    if (inDemoSetMode) {  // in Demo SetMode
     Serial.println("in the Demo Set Mode");
     
    
    } else {  // run the Demo
      Serial.println("in the Demo Run Mode");
    }



  }
   int P=digitalRead(ResetButton);

   Serial.println(P);
   
  delay(3000);

}
