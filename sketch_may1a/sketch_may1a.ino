const int soilMoisturePin = A0;
const int LED_PIN = D1;  
//const int FAN_PIN = D1;
void setup() {
  pinMode(LED_PIN, OUTPUT);
 // pinMode(FAN_PIN, OUTPUT);
  Serial.begin(9600);
}

void loop() {
  int soilMoistureValue = analogRead(soilMoisturePin);
  Serial.print("Soil Moisture: ");
  Serial.println(soilMoistureValue);
if (soilMoistureValue > 550) {  // Check if soil moisture value is greater than threshold
    digitalWrite(LED_PIN, HIGH);  // Turn on LED
  } else {
    digitalWrite(LED_PIN, LOW);   // Turn off LED
  }
  delay(1000);
}
