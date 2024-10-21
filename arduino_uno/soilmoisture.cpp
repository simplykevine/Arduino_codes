//int matrix[2][3] = {{1, 2, 3} {4, 5, 6}};
#define SOILPIN A0


void setup() {
  // put your setup code here, to run once:
  Serial.begin(9500);

}

void loop() {
  // put your main code here, to run repeatedly:
float soilmoisture = analogRead(A0);
Serial.println(soilmoisture);
}
