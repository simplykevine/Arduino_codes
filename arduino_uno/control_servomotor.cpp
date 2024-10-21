#include<Servo.h>
Servo myservo;
void setup() {
myservo.attach(1);
}

void loop() {
myservo.write(100);
delay(1000);
myservo.write(0);
delay(1000);
}
