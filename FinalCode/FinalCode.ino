#include "DHTesp.h"
#include <Wire.h>
#include <BH1750.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <ArduinoJson.h>

const int soilMoisturePin = A0;
const int DHT_PIN = D0;
const int FAN_PIN = D7;
const int LED_PIN = D6;
const int LED_RAI_PIN  = D5; 
const char* ssid = "AndroidAP";
const char* password = "modamoda";
const char* serverName = "http://192.168.43.48:8000/api/data";


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
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  
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
  int pres = map(soilMoistureValue,wet,dry,100,0);
  Serial.print(pres);
  Serial.println("%")
  delay(1000);

  TempAndHumidity  data = dhtSensor.getTempAndHumidity();
  Serial.println("Temp: " + String(data.temperature, 2) + "°C");
  Serial.println("Humidity: " + String(data.humidity, 1) + "%");
  delay(1000);
if (data.temperature>28 || data.humidity>80){
  digitalWrite(FAN_PIN, HIGH);  // Turn on LED
  } else {
    digitalWrite(FAN_PIN, LOW);   // Turn off LED
  }

if (pres < 60  ) {  // Check if soil moisture value is greater than threshold
    digitalWrite(LED_RAI_PIN, HIGH);  // Turn on LED
  } else {
    digitalWrite(LED_RAI_PIN, LOW);   // Turn off LED
  }
  if (lux < 400 ){
    digitalWrite(LED_PIN,HIGH);
  }else{
    digitalWrite(LED_PIN,LOW); 
  }
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    WiFiClient client;
    DynamicJsonDocument jsonDoc(1024);

    jsonDoc["temperature"] = data.temperature;
    jsonDoc["humidity"] = data.humidity;
    jsonDoc["soil"] = soilMoistureValue;
    jsonDoc["light"] = lux;


    String json;
    serializeJson(jsonDoc, json);

    http.begin(client,serverName);
    http.addHeader("Content-Type", "application/json");
    int httpResponseCode = http.POST(json);

    if (httpResponseCode > 0) {
      String response = http.getString();
      Serial.println(httpResponseCode);
      Serial.println(response);
    } else {
      Serial.print("Error code: ");
      Serial.println(httpResponseCode);
    }

    http.end();
  } else {
    Serial.println("WiFi not connected");
  }
  
  delay(10000);
}
