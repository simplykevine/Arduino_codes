int ldr=A0;
int led=4;
void setup() {
  // put your setup code here, to run once:
  pinMode(ldr, INPUT);
  Serial.begin(9600);

}

void loop() {
  // put your main code here, to run repeatedly:
  int ldrvalue= analogRead(ldr);
  Serial.println("Light intensity");
  Serial.print(ldrvalue);
  delay(1000);

  if (ldrvalue<500){
  digitalWrite(led, LOW);
 }
 else{
  digitalWrite(led, HIGH);
 }

}
