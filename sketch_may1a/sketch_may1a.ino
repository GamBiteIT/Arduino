const int soilMoisturePin = A0;


void setup() {
 pinMode(soilMoisturePin, INPUT);
  Serial.begin(9600);
}

void loop() {
  int soilMoistureValue = analogRead(soilMoisturePin);
  Serial.print("Soil Moisture: ");
  Serial.println(soilMoistureValue);
  delay(1000);
}
