#include <ArduinoHttpClient.h>
#include <WiFi.h>


/////// WiFi Settings ///////
char ssid[] = "NU-IoT";
char pass[] = "";

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
  }

  signal = 
  // print the SSID of the network you're attached to:
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  // print your WiFi shield's IP address:
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);
}

void loop() {
  // assemble the path for the POST message:
  String path = "";
  String contentType = "application/json";

  // Assemble the body of the POST message:
  // This is hardcoded for testing. This will be what we need to switch to variables
  // Device ID will be hardcoded. Don't forget to change the Device ID
  const char dev_id[] = "1002";
  int msg_timestamp = 1720815878;
  float soilmoisture = 32.4;
  int voltage = 1372;
  float bat = 4.76;

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
