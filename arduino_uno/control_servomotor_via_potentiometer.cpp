#include <Servo.h>
Servo Servo1;

int servoPin = 9;
int potPin = A0;
 
void setup() {
    Servo1.attach(servoPin);
    pinMode(potPin, INPUT);

 }
void loop() {
    int reading = analogRead(potPin);
    int angle = map(reading, 0, 1023, 0, 180);
    Servo1.write(angle);
}
