#include <Arduino.h>
#include <WiFiNINA.h>
#include <RTC.h>
#include <NTPClient.h>
#include <WiFiUdp.h>
#include <ArduinoHttpClient.h>

// Replace these with your network credentials
const char* ssid = "Verizon-MiFi8800L-B8D0";
const char* password = "3c55dc92";
const int sensorPin = A0;

WiFiUDP Udp; // A UDP instance to let us send and receive packets over UDP
NTPClient timeClient(Udp, "pool.ntp.org", 0, 60000);

WiFiClient wifiClient;
const char* serverAddress = "http://127.0.0.1"; // Your server domain or IP
const int serverPort = 5000; // Standard HTTP port
HttpClient client = HttpClient(wifiClient, serverAddress, serverPort);

String deviceID;

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

  // Retrieve the MAC address
  byte mac[6];
  WiFi.macAddress(mac);
  deviceID = String(mac[0], HEX) + ":" + String(mac[1], HEX) + ":" + String(mac[2], HEX) + ":" + 
             String(mac[3], HEX) + ":" + String(mac[4], HEX) + ":" + String(mac[5], HEX);

  Serial.print("Device ID (MAC Address): ");
  Serial.println(deviceID);

  Serial.println("\nStarting connection to NTP server...");
  timeClient.begin();
  timeClient.update();
}

void loop() {
  int sensorValue = analogRead(sensorPin);
  float millivolts = sensorValue * (5.0 / 1023.0) * 1000;

  float VWC = (4.824e-10 * pow(millivolts, 3)) 
            - (2.278e-6 * pow(millivolts, 2)) 
            + (3.898e-3 * millivolts) 
            - 2.154;

  // Print the results to the Serial Monitor
  Serial.print(" \t Millivolts: ");
  Serial.print(millivolts, 1); // Print millivolts with 1 decimal place
  Serial.print(" mV\t VWC: ");
  Serial.print(VWC, 2); // Print VWC with 2 decimal places
  Serial.print("\t");

  // Update the time from the NTP server
  timeClient.update();
  auto timeZoneOffsetHours = -6; // Adjust for your time zone
  auto unixTime = timeClient.getEpochTime() + (timeZoneOffsetHours * 3600);
  Serial.print(" \t Unix time = ");
  Serial.print(unixTime);

  // Prepare the data as a JSON object
  String jsonData = "{\"dev_id\":\"" + deviceID + "\",\"msg_timestamp\":" + String(unixTime) + ",\"soilmoisture\":" + String(millivolts, 1) + ",\"voltage\":" + String(VWC, 2) + ",\"bat\":100}";

  // Send the data to the server
  if (WiFi.status() == WL_CONNECTED) {
    client.beginRequest();
    client.post("/data_receiver.php"); // Path to your PHP script
    client.sendHeader("Content-Type", "application/json");
    client.sendHeader("Content-Length", jsonData.length());
    client.beginBody();
    client.print(jsonData);
    client.endRequest();

    int statusCode = client.responseStatusCode();
    String response = client.responseBody();

    Serial.print("HTTP Response code: ");
    Serial.println(statusCode);
    Serial.print("Response: ");
    Serial.println(response);
  } else {
    Serial.println("Wi-Fi not connected");
  }

  // Wait for 10 seconds before checking again
  delay(10000);
}
