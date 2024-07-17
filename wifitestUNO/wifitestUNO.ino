#include <Arduino.h>
#include <WiFi.h>


// Replace these with your network credentials
const char* ssid = "NU-IoT";
const char* password = "mstvddmh";


void setup() {
  // Start serial communication
  Serial.begin(115200);
  while (!Serial) {
    ; // Wait for Serial to initialize
  }


  // Start Wi-Fi connection
  Serial.println("Connecting to Wi-Fi...");
  WiFi.begin(ssid, password);

  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }

  // Once connected, print some information
  Serial.println("");
  Serial.println("Wi-Fi connected.");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());

}


void loop() {
  if(WiFi.status() == WL_CONNECTED) {
    // Print the connection status and signal strength
    Serial.println("Connected to Wi-Fi");
    Serial.print("Signal strength (RSSI): ");
    Serial.print(WiFi.RSSI());
    Serial.println(" dBm");
  } 
  else {
    Serial.println("Disconnected from Wi-Fi");
  }

  // Wait for 10 seconds before checking again
  delay(10000);
}