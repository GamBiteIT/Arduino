const int LDR_PIN = D1;

void setup() {
  Serial.begin(9600);
}

void loop() {
  int ldrValue = digitalRead(LDR_PIN);
  Serial.print("LDR voltage: ");
  Serial.println(ldrValue);
  delay(1000);
}
