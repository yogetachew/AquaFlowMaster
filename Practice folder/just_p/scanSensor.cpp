#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <DHT_U.h>

#define DHTPIN 2     // Digital pin connected to the DHT sensor
#define DHTTYPE DHT11   // DHT 11

DHT_Unified dht(DHTPIN, DHTTYPE);

void setup() {
  Serial.begin(9600);
  dht.begin();
  Serial.println("DHTxx Unified Sensor Example");
}

void loop() {
  sensors_event_t event;
  dht.temperature().getEvent(&event);
  if (!isnan(event.temperature)) {
    Serial.print("Temperature: ");
    Serial.print(event.temperature);
    Serial.println(" *C");
  } else {
    Serial.println("Error reading temperature!");
  }
  
  dht.humidity().getEvent(&event);
  if (!isnan(event.relative_humidity)) {
    Serial.print("Humidity: ");
    Serial.print(event.relative_humidity);
    Serial.println(" %");
  } else {
    Serial.println("Error reading humidity!");
  }
  
  delay(2000); // Wait a few seconds between measurements.
}
