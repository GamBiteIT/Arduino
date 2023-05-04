#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>
#include <ESP8266WiFi.h>

const char* ssid = "D-Link0909";
const char* password = "ghaz1234";
const char* api_url = "http://192.168.1.4:8000/api/data";

void setup() {
  Serial.begin(9600);
  WiFi.begin(ssid, password);

  Serial.print("Connecting to Wi-Fi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }
  Serial.println();
}

void loop() {
  if (WiFi.status() == WL_CONNECTED) {
    WiFiClient client;
    HTTPClient http;

    // Build JSON data
    String json_data = "{\"temperature\": 25.6, \"humidity\": 60.2,\"\",}";

    // Send HTTP POST request
    http.begin(client, api_url);
    http.addHeader("Content-Type", "application/json");
    int httpCode = http.POST(json_data);
    
    // Check response
    if (httpCode > 0) {
      String response = http.getString();
      Serial.println(response);
    } else {
      Serial.printf("[HTTP] POST request failed, error: %s\n", http.errorToString(httpCode).c_str());
    }
    
    http.end();
  }
  
  delay(5000);
}
