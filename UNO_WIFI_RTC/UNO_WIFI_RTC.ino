#include <Arduino.h>
#include <WiFi.h>
#include <RTC.h>
#include <NTPClient.h>
#include <WiFiUdp.h>

// Replace these with your network credentials
const char* ssid = "Verizon-MiFi8800L-B8D0";
const char* password = "3c55dc92";

WiFiUDP Udp; // A UDP instance to let us send and receive packets over UDP
NTPClient timeClient(Udp);

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
  Serial.println("Connected to Wi-Fi");
  Serial.print("Signal strength (RSSI): ");
  Serial.print(WiFi.RSSI());
  Serial.println(" dBm");
  RTC.begin();
  Serial.println("\nStarting connection to NTP server...");
  timeClient.begin();
  timeClient.update();

  // Get the current date and time from an NTP server and convert
  // it to UTC +2 by passing the time zone offset in hours.
  // You may change the time zone offset to your local one.
  auto timeZoneOffsetHours = -6;
  auto unixTime = timeClient.getEpochTime() + (timeZoneOffsetHours * 3600);
  Serial.print("Unix time = ");
  Serial.println(unixTime);
  RTCTime timeToSet = RTCTime(unixTime);
  RTC.setTime(timeToSet);

  // Retrieve the date and time from the RTC and print them
  RTCTime currentTime;
  RTC.getTime(currentTime); 
  Serial.println("The RTC was just set to: " + String(currentTime));
   
}


void loop() {

}