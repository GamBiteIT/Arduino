#include <DHT.h>

#define DHTPIN D1  // DHT11 data pin connected to GPIO4 (D4)
#define DHTTYPE DHT11  // DHT11 sensor type

DHT dht(DHTPIN, DHTTYPE);

const int FAN_PIN = D2;  // GPIO5 (D1) connected to the fan

void setup() {
  Serial.begin(9600);
  pinMode(FAN_PIN, OUTPUT);
  dht.begin();
}

void loop() {
  float temperature = dht.readTemperature();
  float humidity = dht.readHumidity();

  if (isnan(temperature) || isnan(humidity)) {
    Serial.println("Failed to read data from DHT sensor");
    return;
  }

  Serial.print("Temperature: ");
  Serial.print(temperature);
  Serial.print(" °C, Humidity: ");
  Serial.print(humidity);
  Serial.println("%");

  if (temperature > 26 && humidity > 40) {
    digitalWrite(FAN_PIN, HIGH);  // turn on the fan
    Serial.println("Fan turned on");
  } else {
    digitalWrite(FAN_PIN, LOW);  // turn off the fan
    Serial.println("Fan turned off");
  }

  delay(20000);
}
