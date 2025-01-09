#define pot0 A0
#define pot1 A1
#define pot2 A2


void setup() {
  Serial.begin(9600);
  pinMode(pot0 , INPUT);
  pinMode(pot1 , INPUT);
  pinMode(pot2 , INPUT);


}

void loop() {
  int a = analogRead(pot0);
  int b = analogRead(pot1);
  int c = analogRead(pot2);
  Serial.print(a);
  Serial.print("  ");
  Serial.print(b);
  Serial.print("  ");
  Serial.print(c);
  Serial.println("  ");
}
