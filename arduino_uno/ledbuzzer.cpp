int buzzer=6;
int ledLight= 5;

void setup() {
  pinMode(buzzer, OUTPUT);
  pinMode(ledLight, OUTPUT);

}

void loop() {
  // put your main code here, to run repeatedly:
digitalWrite(buzzer, HIGH);
delay(5000);
digitalWrite(buzzer, LOW);
delay(5000);
digitalWrite(ledLight, HIGH);
delay(5000);
digitalWrite(ledLight, LOW);
delay(5000);

}
