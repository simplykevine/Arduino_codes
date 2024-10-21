#include <ESP8266WiFi.h>

int red = D4;
int green = D2;
int yellow = D3;

void setup() {
  pinMode(red, OUTPUT);
  pinMode(green, OUTPUT);
  pinMode(yellow, OUTPUT);
  Serial.begin(115200);
  
  // Set WiFi mode to station
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  delay(1000);
}

void loop() {
  Serial.println("Scanning for available networks...");
  digitalWrite(red, HIGH);
  digitalWrite(green, LOW);

  int n = WiFi.scanNetworks();
  if (n > 0) {
    digitalWrite(green, HIGH);
    digitalWrite(red, LOW);  
    Serial.print(n); Serial.println(" network(s) found");
    for (int i = 0; i < n; ++i) {
      Serial.print("Network "); Serial.print(i + 1); Serial.print(": ");
      Serial.print(WiFi.SSID(i));
      Serial.print(" ("); Serial.print(WiFi.RSSI(i)); Serial.print(" dBm)");
      Serial.print((WiFi.encryptionType(i) == ENC_TYPE_NONE) ? " open" : " secured");
      Serial.println();
      delay(50);
    }
  } else {
    Serial.println("No available networks found");
    digitalWrite(red, LOW);
    digitalWrite(green, HIGH); 
  }
  Serial.println("---------------------------------------------------------------");
    delay(5000);
}
