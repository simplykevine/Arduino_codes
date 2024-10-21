#include <LiquidCrystal.h>
#include <DHT.h>
#include <NewPing.h>

// Define pins for the LCD
const int rs = 7, en = 8, d4 = 9, d5 = 10, d6 = 11, d7 = 12;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

// Define pins for ultrasonic sensor
#define TRIG_PIN 2
#define ECHO_PIN 3
#define MAX_DISTANCE 200 // Maximum distance to measure

// Define pin for DHT sensor
#define DHT_PIN 4
#define DHT_TYPE DHT11 // Change to DHT22 if you are using that

// Initialize the DHT sensor
DHT dht(DHT_PIN, DHT_TYPE);

// Initialize the ultrasonic sensor
NewPing sonar(TRIG_PIN, ECHO_PIN, MAX_DISTANCE);

void setup() {
  // Start the LCD
  lcd.begin(16, 2); // Specify the number of columns and rows
  lcd.clear();
  
  // Start the DHT sensor
  dht.begin();
  
  // Set up the serial communication
  Serial.begin(9600);
}

void loop() {
  // Read the DHT sensor
  float humidity = dht.readHumidity();
  float temperature = dht.readTemperature();
  
  // Read the ultrasonic sensor
  unsigned int distance = sonar.ping_cm();

  // Check if any reads failed and exit early (to try again).
  if (isnan(humidity) || isnan(temperature)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }

  // Display data on the LCD
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Temp: ");
  lcd.print(temperature);
  lcd.print("C");
  lcd.setCursor(0, 1);
  lcd.print("Hum: ");
  lcd.print(humidity);
  lcd.print("%");

  delay(2000);

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Distance:");
  lcd.setCursor(0, 1);
  lcd.print(distance);
  lcd.print(" cm");
  delay(2000);
  Serial.print("Distance:");
  Serial.print(distance);
  Serial.println(" cm");
}
