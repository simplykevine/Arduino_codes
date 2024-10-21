const int soundSensorPin = A0; // Analog pin for sound sensor
const int relayPin = 2;        // Digital pin for relay module

int claps = 0;
int threshold = 300; // Adjust this threshold value for your environment

void setup() {
  pinMode(relayPin, OUTPUT);
  Serial.begin(9600);
}

void loop() {
  int soundLevel = analogRead(soundSensorPin);  // Read sound sensor value
  Serial.println(soundLevel);  // Print sound level to serial monitor

  if (soundLevel > threshold) {
    claps++;
    delay(1000);  // Add delay to prevent double counting of claps
  }

  if (claps == 1) {
    digitalWrite(relayPin, HIGH);  // Turn on relay for one clap
  } else if (claps == 2) {
    digitalWrite(relayPin, HIGH);  // Turn on relay for two claps
  } else if (claps == 3) {
    digitalWrite(relayPin, LOW);  // Turn off relay for three claps
    claps = 0;  // Reset clap counter
  }
}
