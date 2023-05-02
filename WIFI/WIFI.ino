#include <ESP8266WiFi.h>

const char* ssid = "D-Link";
const char* password = "3007200019";

void setup() {
  Serial.begin(9600);
  WiFi.begin(ssid, password);
  Serial.println("Connecting to Wi-Fi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting...");
  }
  Serial.println("Connected to Wi-Fi");
}

void loop() {
  // Your code here
}
