#include <ArduinoJson.h>
#include <DHT.h>
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>

#define DHT_TYPE DHT22
#define DHT_PIN 2

DHT dht(DHT_PIN, DHT_TYPE);

const char* ssid = "Orange-6A41";
const char* password = "A93239A9";

ESP8266WebServer server(80);

const int led = 13;
const char* host = "esp";

void handleRoot() {
  digitalWrite(led, 1);

  StaticJsonBuffer<100> jsonBuffer;
  
  JsonObject& root = jsonBuffer.createObject();
  root["status"] = "OK";
  
  char outputBuffer[256];
  root.printTo(outputBuffer, sizeof(outputBuffer));

  server.send(200, "application/json", outputBuffer);
  
  digitalWrite(led, 0);
}

void handleNotFound(){
  digitalWrite(led, 1);
  server.send(404, "application/json", "{\"status\": \"ERROR\", \"message\":\"NOT_FOUND\"}");
  digitalWrite(led, 0);
}

void setup(void){
  pinMode(led, OUTPUT);
  digitalWrite(led, 0);
  Serial.begin(115200);

  dht.begin();
  
  WiFi.begin(ssid, password);
  Serial.println("");
  Serial.print("Connecting to ");
  Serial.print(ssid);

  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("Connected!");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  if (MDNS.begin(host)) {
    Serial.print("MDNS responder started: ");
    Serial.println(host);
  }

  server.on("/", handleRoot);

  server.onNotFound(handleNotFound);

  server.begin();
  Serial.println("HTTP server started");
}

void loop(void){
  server.handleClient();
}
