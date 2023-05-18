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
const char* ssid = "Edimax";
const char* password = "lollol1919";
const char* serverName = "http://192.168.2.100:8000/api/data";
const char* parametreApi = "http://192.168.2.100:8000/api/parametre";
const char* DeviceApi = "http://192.168.2.100:8000/api/device";
float temperatureValeur;
float humidityValeur;
float soilValeur;
float lightValeur;
int season_id ;
String status;
int fanstatus = 0;
int pumpstatus = 0;
int ledstatus = 0;




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
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    WiFiClient client;
    DynamicJsonDocument jsonDoc(1024);

    http.begin(client, parametreApi);
    http.addHeader("Content-Type", "application/json");
    int httpResponseCode = http.GET();

    if (httpResponseCode > 0) {
      String response = http.getString();
      Serial.println("HTTP Response code: " + String(httpResponseCode));
      Serial.println("Response: " + response);

      DeserializationError error = deserializeJson(jsonDoc, response);

      if (error) {
        Serial.print("JSON parsing failed: ");
        Serial.println(error.c_str());
      } else {
        // Extract data from JSON
        season_id = jsonDoc["season_id"];
        temperatureValeur = jsonDoc["TemperatureValeur"];
        humidityValeur = jsonDoc["HumidityValeur"];
        soilValeur = jsonDoc["SoilValeur"];
        lightValeur = jsonDoc["LightValeur"];
   
      }
    } else {
      Serial.print("Error code: ");
      Serial.println(httpResponseCode);
    }

    http.end();
  } else {
    Serial.println("WiFi not connected");
  }

  uint16_t lux = lightMeter.readLightLevel();
  Serial.print("Light: ");
  Serial.print(lux);
  Serial.println(" lx");
  delay(1000);
  int soilMoistureValue = analogRead(soilMoisturePin);
  int perc = map(soilMoistureValue,650,0,0,100);
  Serial.print("Soil Moisture: ");
  Serial.println(perc);
  delay(1000);

  TempAndHumidity  data = dhtSensor.getTempAndHumidity();
  Serial.println("Temp: " + String(data.temperature, 2) + "°C");
  Serial.println("Humidity: " + String(data.humidity, 1) + "%");
  delay(1000);
if (data.temperature>temperatureValeur || data.humidity>humidityValeur){
  digitalWrite(FAN_PIN, HIGH); 
  fanstatus = 1;
   // Turn on LED
  } else {
    digitalWrite(FAN_PIN, LOW);   // Turn off LED
    fanstatus = 0;
  }

if (perc < soilValeur  ) {  // Check if soil moisture value is greater than threshold
    digitalWrite(LED_RAI_PIN, HIGH);  // Turn on LED
    pumpstatus = 1;
  } else {
    digitalWrite(LED_RAI_PIN, LOW);   // Turn off LED
    pumpstatus = 0;
  }
  if (lux < lightValeur ){
    digitalWrite(LED_PIN,HIGH);
    ledstatus = 1;
  }else{
    digitalWrite(LED_PIN,LOW); 
    ledstatus = 0;
  }
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    WiFiClient client;
    DynamicJsonDocument jsonDoc(1024);

    jsonDoc["season_id"] = season_id;
    jsonDoc["temperature"] = data.temperature;
    jsonDoc["humidity"] = data.humidity;
    jsonDoc["soil"] = perc;
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
    jsonDoc["ids"] = "1,2,3";
    jsonDoc["works"]= String(fanstatus)+","+String(pumpstatus)+","+String(ledstatus);
    String jsonStatus;
    serializeJson(jsonDoc, jsonStatus);

    http.begin(client,DeviceApi);
    http.addHeader("Content-Type", "application/json");
    httpResponseCode = http.POST(jsonStatus);

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
  
  delay(40000);
}
