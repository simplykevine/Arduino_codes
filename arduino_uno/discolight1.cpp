int led=5;
int leD=6;
int lED= 4;
void setup() {
  pinMode(led, OUTPUT);
  pinMode(leD, OUTPUT);
  pinMode(lED,OUTPUT);
 
}

void loop() {
  digitalWrite(led, HIGH);
  delay(500);
  digitalWrite(led, LOW);
  delay(500);
digitalWrite(leD, HIGH);
  delay(500);
  digitalWrite(leD, LOW);
  delay(500);
digitalWrite(lED, HIGH);
  delay(500);
  digitalWrite(lED, LOW);
  delay(500);

}
