#include <WiFi.h>
#include <WebServer.h>

WebServer server(80);

const char* ssid = "Your_AP_SSID";
const char* password = "Your_AP_PASSWORD";

void handleRoot() {
  String html = "<html><body><h1>Wi-Fi Configuration</h1>";
  html += "<form action='/configure' method='POST'>";
  html += "SSID: <input type='text' name='ssid'><br>";
  html += "Password: <input type='text' name='password'><br>";
  html += "<input type='submit' value='Connect'>";
  html += "</form></body></html>";
  server.send(200, "text/html", html);
}

void handleConfigure() {
  String ssid = server.arg("ssid");
  String password = server.arg("password");

  WiFi.begin(ssid.c_str(), password.c_str());
  server.send(200, "text/html", "Connecting to Wi-Fi...");
}

void setup() {
  Serial.begin(115200);
  WiFi.softAP(ssid, password);
  server.on("/", handleRoot);
  server.on("/configure", HTTP_POST, handleConfigure);
  server.begin();
  Serial.println("Web server started");
}

void loop() {
  server.handleClient();
}
