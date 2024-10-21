#include <ESP8266WiFi.h>  // Include the ESP8266 WiFi library
#include <Wire.h>         // Include the I2C communication library (not needed for this code)
#include <Servo.h>        // Include the Servo library for controlling the servo motor

Servo servo;  // Create a Servo object

// Wi-Fi credentials
#define WIFI_SSID "ALU_Student"
#define WIFI_PASSWORD "@fr1canLU"

// Ultrasonic sensor pins
const int trigPin = 1;  // Trig pin of the ultrasonic sensor
const int echoPin = 3;  // Echo pin of the ultrasonic sensor

// Variables for ultrasonic sensor
long duration;
float distance;

// Define the distance threshold to trigger the servo motor (in centimeters)
const float thresholdDistance = 15.0;  // Adjust this as per your requirement

void setup() {
  // Initialize Serial Monitor
  Serial.begin(115200);

  // Connect to Wi-Fi
  connectToWiFi();

  // Initialize the servo
  servo.attach(2);  // Connect the servo to pin 2 of the ESP8266

  // Set up the ultrasonic sensor pins
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
}

void loop() {
  // Measure the distance using the ultrasonic sensor
  distance = getDistance();

  // Print the distance for debugging
  Serial.print("Distance: ");
  Serial.println(distance);

  // Check if the distance is less than the threshold
  if (distance < thresholdDistance) {
    openDustbin();  // Open the dustbin when someone is near
  } else {
    closeDustbin(); // Close the dustbin when no one is nearby
  }

  delay(500);  // Delay to avoid multiple readings in a short time
}

// Function to connect to Wi-Fi
void connectToWiFi() {
  Serial.println();
  Serial.println("Connecting to WiFi...");

  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);  // Start connecting to Wi-Fi

  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }

  Serial.println();
  Serial.println("WiFi connected.");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());  // Print the IP address after connection
}

// Function to get the distance from the ultrasonic sensor
float getDistance() {
  // Clear the trigPin
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);

  // Set the trigPin HIGH for 10 microseconds to send the ultrasonic pulse
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  // Read the echoPin, returns the sound wave travel time in microseconds
  duration = pulseIn(echoPin, HIGH);

  // Calculate the distance (Speed of sound = 343 m/s)
  distance = (duration * 0.0343) / 2;

  return distance;
}

// Function to open the dustbin (servo moves to 90 degrees)
void openDustbin() {
  Serial.println("Opening Dustbin...");
  servo.write(90);  // Open the dustbin by rotating the servo to 90 degrees
  delay(3000);      // Keep the dustbin open for 3 seconds (adjust this if needed)
}

// Function to close the dustbin (servo moves back to 0 degrees)
void closeDustbin() {
  Serial.println("Closing Dustbin...");
  servo.write(0);  // Close the dustbin by rotating the servo back to 0 degrees
}

