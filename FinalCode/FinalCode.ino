#include "DHTesp.h"
const int soilMoisturePin = A0;
const int LED_PIN = D3;  
const int FAN_PIN = D4;
const int DHT_PIN = 5;
DHTesp dhtSensor;



void setup() {
  pinMode(LED_PIN, OUTPUT);
  dhtSensor.setup(DHT_PIN, DHTesp::DHT11);
 pinMode(FAN_PIN, OUTPUT);
  Serial.begin(9600);
}





void loop() {
  int soilMoistureValue = analogRead(soilMoisturePin);
  Serial.print("Soil Moisture: ");
  Serial.println(soilMoistureValue);
  TempAndHumidity  data = dhtSensor.getTempAndHumidity();
  Serial.println("Temp: " + String(data.temperature, 2) + "°C");
  Serial.println("Humidity: " + String(data.humidity, 1) + "%");
  Serial.println("---");
if (data.temperature>34 || data.humidity>65){
  digitalWrite(FAN_PIN, HIGH);  // Turn on LED
  } else {
    digitalWrite(FAN_PIN, LOW);   // Turn off LED
  }

if (soilMoistureValue > 550) {  // Check if soil moisture value is greater than threshold
    digitalWrite(LED_PIN, HIGH);  // Turn on LED
  } else {
    digitalWrite(LED_PIN, LOW);   // Turn off LED
  }
  delay(1000);
}
