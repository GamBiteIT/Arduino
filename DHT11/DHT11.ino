/**
   ESP32 + DHT22 Example for Wokwi
   
   https://wokwi.com/arduino/projects/322410731508073042
*/

#include "DHTesp.h"

const int DHT_PIN = 5;
const int FAN_PIN = 4;

DHTesp dhtSensor;

void setup() {
  pinMode(LED_PIN, OUTPUT);
  Serial.begin(9600);

  dhtSensor.setup(DHT_PIN, DHTesp::DHT11);
}

void loop() {
  TempAndHumidity  data = dhtSensor.getTempAndHumidity();
  Serial.println("Temp: " + String(data.temperature, 2) + "°C");
  Serial.println("Humidity: " + String(data.humidity, 1) + "%");
  Serial.println("---");
  delay(10000);
}