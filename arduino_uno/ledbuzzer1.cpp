int ledred=2;
int ledgreen=3;
int ledblue=4;
int buzzer= 5;
void setup() {
  // put your setup code here, to run once:
  pinMode(ledred, OUTPUT);
  pinMode(ledgreen, OUTPUT);
  pinMode(ledblue, OUTPUT);
  pinMode(buzzer, OUTPUT);
//
}
void loop() {
  // put your main code here, to run repeatedly:
digitalWrite(ledred, HIGH); //TURN ON LED
delay(2000);
digitalWrite(ledred, LOW);
delay(2000);
digitalWrite(ledgreen, HIGH);
delay(1000);
digitalWrite(ledgreen, LOW);
delay(1000);
digitalWrite(ledblue, HIGH);
delay(2000);
digitalWrite(ledblue, LOW);
delay(2000);
digitalWrite(buzzer, HIGH);
delay(3000);
digitalWrite(buzzer, LOW);
delay(3000);
}
