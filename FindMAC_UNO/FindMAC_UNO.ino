#include <WiFiS3.h>

void setup() {

  // Start Serial communication for debug output
  Serial.begin(9600);

  // Wait a bit to ensure that the Serial Monitor is properly initialized
  delay(1000);

  // Retrieve and print the MAC address
  byte mac[6];
  WiFi.macAddress(mac);
  Serial.print("MAC address: ");

  for (int i = 0; i < 6; ++i) {
    if (mac[i] < 16) Serial.print("0");
    Serial.print(mac[i], HEX);
    if (i < 5) Serial.print(":");
  }

  Serial.println();
}

void loop() {
  // No actions are needed in the loop
}