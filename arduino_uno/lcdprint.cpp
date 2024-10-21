#include <LiquidCrystal.h>

// Pin definitions for LCD
const int rs = 7, en = 6, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

void setup() {
  lcd.begin(16, 2); // Initialize LCD with 16 columns and 2 rows
  lcd.print("Kevine"); // Print a test message
}

void loop() {
  // Nothing to do here
}
