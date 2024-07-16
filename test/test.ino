#include <ArduinoHttpClient.h>
#include <WiFi.h>
#include <RTC.h>
#include <NTPClient.h>
#include <WiFiUDP.h>


/////// WiFi Settings ///////
char ssid[] = "NU-IoT";
char pass[] = "";
int sensorPin = A0;

WiFiUdp Udp; // A UDP instance to let us send and receive packets over UDP
NTPClient timeClient(Udp);

// Address to the server to upload data to
const char serverAddress[] = "";  // server address
int port = 80;

WiFiClient wifi;
HttpClient client = HttpClient(wifi, serverAddress, port);
int status = WL_IDLE_STATUS;

void setup() {
  Serial.begin(9600);
  while(!Serial);
  while ( status != WL_CONNECTED) {
    Serial.print("Attempting to connect to Network named: ");
    Serial.println(ssid);                   // print the network name (SSID);

    // Connect to WPA/WPA2 network:
    status = WiFi.begin(ssid, pass);

    RTC.begin();
  }

  // print the SSID of the network you're attached to:
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  // print your WiFi shield's IP address:
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);

  // Get the current date and time from an NTP server and convert
  // it to UTC +2 by passing the time zone offset in hours.
  // You may change the time zone offset to your local one.
  auto timeZoneOffsetHours = -6;
  auto unixTime = timeClient.getEpochTime() + (timeZoneOffsetHours * 3600);
 
}

void loop() {
  int sensorValue = analogRead(sensorPin);
  float Milivolts = sensorValue * (5.0 / 1023.0)* 1000;
  
  // Read battery voltage from Analog pin A1; assign to BatterySensorValue variable
  int BatterySensorValue = analogRead(A1);

  // Convert the analog reading (which goes from 0 - 1023) to a voltage (0 - 5V) to read the battery voltage levels
  float voltage = BatterySensorValue * (5.0 / 1023.0);

  // Calculate the Volumetric Water Content
  float Soil_moisture = (4.824e-10 * pow(Milivolts, 3)) 
            - (2.278e-6 * pow(Milivolts, 2)) 
            + (3.898e-3 * Milivolts) 
            - 2.154;

  // assemble the path for the POST message:
  String path = "";
  String contentType = "application/json";

  // Assemble the body of the POST message:
  // This is hardcoded for testing. This will be what we need to switch to variables
  // Device ID will be hardcoded. Don't forget to change the Device ID
  const char dev_id[] = "1002";
  int msg_timestamp = 1720815878;
  float soilmoisture = Soil_moisture;
  int voltage = Milivolts;
  float bat = voltage;
  timeClient.update();

  // Get the current date and time from an NTP server and convert
  // it to UTC +2 by passing the time zone offset in hours.
  // You may change the time zone offset to your local one.
  auto timeZoneOffsetHours = -6;
  auto DateStamp = timeClient.getEpochTime() + (timeZoneOffsetHours * 3600);
  Serial.print(" \t Unix time = ");
  Serial.print(DateStamp);


  // Construct JSON string
  String postData = "{";
  postData += "\"dev_id\":\"" + String(dev_id) + "\",";
  postData += "\"msg_timestamp\":\"" + String(msg_timestamp) + "\",";
  postData += "\"soilmoisture\":\"" + String(soilmoisture) + "\",";
  postData += "\"voltage\":\"" + String(voltage) + "\",";
  postData += "\"bat\":\"" + String(bat) + "\"";
  postData += "}";

  Serial.println("making POST request");

  // send the POST request
  client.post(path, contentType, postData);

  // Print out the JSON to the serial console. 
  Serial.println(postData);

  // read the status code and body of the response
  // Lets us know if the server got the post and responds if data was added successfully or not
  int statusCode = client.responseStatusCode();
  String response = client.responseBody();

  Serial.print("Status code: ");
  Serial.println(statusCode);
  Serial.print("Response: ");
  Serial.println(response);

  Serial.println("Wait three minutes\n");
  delay(180000);
}
