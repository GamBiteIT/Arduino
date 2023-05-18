#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <ArduinoJson.h>

const char* ssid = "PhoneSaber";
const char* password = "lollol1919";
const char* serverName = "http://192.168.2.101:8000/api/parametre";

void setup() {
  Serial.begin(115200);
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

    http.begin(client, serverName);
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
        float temperature = jsonDoc["TemperatureValeur"];
        float humidity = jsonDoc["HumidityValeur"];
        float soil = jsonDoc["SoilValeur"];
        float light = jsonDoc["LightValeur"];


        Serial.print("Temperature: ");
        Serial.println(temperature);
         Serial.println(humidity);
          Serial.println(soil);
           Serial.println(light);
   
      }
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
