#include "Arduino_LED_Matrix.h"
#include <ArduinoHttpClient.h>
#include <WiFi.h>
#include <RTC.h>
#include <NTPClient.h>
#include <WiFiUdp.h>   // Include the LED_Matrix library

ArduinoLEDMatrix matrix; // Create an instance of the ArduinoLEDMatrix class

const int sensorPin = A0;

void setup() {
  Serial.begin(115200); // Initialize serial communication at a baud rate of 115200
  matrix.begin();       // Initialize the LED matrix
}

void loop() {
  int sensorValue = analogRead(sensorPin);
  float multivolts = sensorValue * (5.0 / 1023.0) * 1000;

  // Read battery voltage from Analog pin A3; assign to BatterySensorValue variable
  int BatterySensorValue = analogRead(A3);

  // Convert the analog reading (which goes from 0 - 1023) to a voltage (0 - 5V) to read the battery voltage levels
  float Voltage = BatterySensorValue * (5.0 / 1023.0);

  // Calculate the Volumetric Water Content
  float Soil_moisture = ((4.824e-10 * pow(multivolts, 3))
                         - (2.278e-6 * pow(multivolts, 2))
                         + (3.898e-3 * multivolts)
                         - 2.154) * 100;

  if (Soil_moisture > 67) {
    matrix.loadFrame(LEDMATRIX_DANGER);
  } else if (Soil_moisture < 20) {
    matrix.loadFrame(LEDMATRIX_EMOJI_SAD);
  } else {
    matrix.loadFrame(LEDMATRIX_EMOJI_HAPPY);
  }

  // Print the results to the Serial Monitor
  Serial.print("Millivolts: ");
  Serial.print(multivolts, 1); // Print millivolts with 1 decimal place
  Serial.print(" mV\t VWC: ");
  Serial.print(Soil_moisture, 2); // Print VWC with 2 decimal places
  Serial.print("\t Voltage: ");
  Serial.print(Voltage, 2); // Print voltage with 2 decimal places
  Serial.print("\n");

  delay(5000);
}
