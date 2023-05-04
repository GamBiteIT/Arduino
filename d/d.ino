#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <ArduinoJson.h>

const char* ssid = "D-Link0909";
const char* password = "ghaz1234";
const char* serverName = "http://192.168.1.4:8000/api/data";

void setup() {
  Serial.begin(9600);
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

    jsonDoc["temperature"] = 25.0;
    jsonDoc["humidity"] = 50.0;
    jsonDoc["soil"] = 250.0;
    jsonDoc["light"] = 500.0;


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

  delay(5000);
}
