#include <Arduino.h>
#include <WiFi.h>
#include <RTC.h>
#include <NTPClient.h>
#include <WiFiUdp.h>

// Replace these with your network credentials
char* ssid = "NU-IoT"; //Replace with the ssid name of the wifi
const char* password = "tmvmrppp"; //Replace with the password name of the wifi 
const int sensorPin = A0;

// Device ID
char* deviceID = "0001";

WiFiUDP Udp; // A UDP instance to let us send and receive packets over UDP
NTPClient timeClient(Udp);

void setup() {
  // Start serial communication
  Serial.begin(115200);
  while (!Serial) {
    ; // Wait for Serial to initialize
  }
a
  // Start Wi-Fi connection
  Serial.println("Connecting to Wi-Fi...");
  WiFi.begin(ssid, password);

  // Wait for connection
  if (WiFi.status() != WL_CONNECTED) {
    delay(10000);
    Serial.print(".");
  }
  else{
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
  }

  // Get the current date and time from an NTP server and convert
  // it to UTC +2 by passing the time zone offset in hours.
  // You may change the time zone offset to your local one.
  auto timeZoneOffsetHours = -6;
  auto unixTime = timeClient.getEpochTime() + (timeZoneOffsetHours * 3600);
 
}


void loop() {
  int sensorValue = analogRead(sensorPin);
  float Voltage = sensorValue * (5.0 / 1023.0)* 1000;

  // Read battery voltage from Analog pin A1; assign to BatterySensorValue variable
  int BatterySensorValue = analogRead(A1);

  // Convert the analog reading (which goes from 0 - 1023) to a voltage (0 - 5V) to read the battery voltage levels
  float voltage = BatterySensorValue * (5.0 / 1023.0);

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
  
  timeClient.update();

  // Get the current date and time from an NTP server and convert
  // it to UTC +2 by passing the time zone offset in hours.
  // You may change the time zone offset to your local one.
  auto timeZoneOffsetHours = -6;
  auto DateStamp = timeClient.getEpochTime() + (timeZoneOffsetHours * 3600);
  Serial.print(" \t Unix time = ");
  Serial.print(DateStamp);

// Calculate the Volumetric Water Content
  float Soil_moisture = (4.824e-10 * pow(Voltage, 3)) 
            - (2.278e-6 * pow(Voltage, 2)) 
            + (3.898e-3 * Voltage) 
            - 2.154;

  // Print the results to the Serial Monitor
  Serial.print(" \t Millivolts: ");
  Serial.print(Voltage, 1); // Print millivolts with 1 decimal place
  Serial.print(" mV\t VWC: ");
  Serial.print(Soil_moisture, 2); // Print VWC with 2 decimal places
  Serial.print("\t");
  Serial.print(" \t Device_ID: ");
  Serial.print(deviceID);
  Serial.print(" \t Battery Voltage: ");
  Serial.print(voltage);
  Serial.print(" \n");

  // Wait for 10 seconds before checking again
  delay(10000);
}