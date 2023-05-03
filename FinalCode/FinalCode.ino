#include "DHTesp.h"
#include <Wire.h>
#include <BH1750.h>
const int soilMoisturePin = A0;
const int DHT_PIN = D0;
const int FAN_PIN = D7;
const int LED_PIN = D6;
const int LED_RAI_PIN  = D5; 


DHTesp dhtSensor;

BH1750 lightMeter;

void setup() {
  Serial.begin(9600);
  pinMode(LED_PIN, OUTPUT);
  pinMode(LED_RAI_PIN, OUTPUT);
  pinMode(FAN_PIN, OUTPUT);
  dhtSensor.setup(DHT_PIN, DHTesp::DHT11);
  
  Wire.begin();
  lightMeter.begin();
  
}





void loop() {
  uint16_t lux = lightMeter.readLightLevel();
  Serial.print("Light: ");
  Serial.print(lux);
  Serial.println(" lx");
  delay(1000);
  int soilMoistureValue = analogRead(soilMoisturePin);
  Serial.print("Soil Moisture: ");
  Serial.println(soilMoistureValue);
  delay(1000);

  TempAndHumidity  data = dhtSensor.getTempAndHumidity();
  Serial.println("Temp: " + String(data.temperature, 2) + "°C");
  Serial.println("Humidity: " + String(data.humidity, 1) + "%");
  delay(1000);
if (data.temperature>25 || data.humidity>40){
  digitalWrite(FAN_PIN, HIGH);  // Turn on LED
  } else {
    digitalWrite(FAN_PIN, LOW);   // Turn off LED
  }

if (soilMoistureValue > 550) {  // Check if soil moisture value is greater than threshold
    digitalWrite(LED_RAI_PIN, HIGH);  // Turn on LED
  } else {
    digitalWrite(LED_RAI_PIN, LOW);   // Turn off LED
  }
  if (lux < 50 ){
    digitalWrite(LED_PIN,HIGH);
  }else{
    digitalWrite(LED_PIN,LOW); 
  }
  delay(10000);
}
