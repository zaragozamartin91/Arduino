/*
    This sketch demonstrates how to set up a simple HTTP-like server.
    The server will set a GPIO pin depending on the request
      http://server_ip/gpio/0 will set the GPIO2 low,
      http://server_ip/gpio/1 will set the GPIO2 high
    server_ip is the IP address of the ESP8266 module, will be
    printed to Serial when the module is connected.
*/

#include <Arduino.h>
#include <ESP8266WiFi.h>

// According to the Arduino blink sketch... the true led builtin is pin #2
#define __ESP_LED_BUILTIN 2

#ifndef STASSID
#define STASSID "ZaraGalli"
#define STAPSK "Zaragoza123*"
#endif

const char* ssid = STASSID;
const char* password = STAPSK;

// Create an instance of the server
// specify the port to listen on as an argument
WiFiServer server(80);

/* On esp8266, low voltage on gpio2 turns the led on
https://forum.arduino.cc/t/want-to-blink-blue-led-on-esp8266-01/486496/4 */
#define __ESP_LED_ON 0
#define __ESP_LED_OFF 1
int ledValue = __ESP_LED_OFF;

void setup() {
  Serial.begin(115200);
  while(!Serial) {};

  // prepare LED
  pinMode(__ESP_LED_BUILTIN, OUTPUT);
  digitalWrite(__ESP_LED_BUILTIN, __ESP_LED_OFF);

  // Connect to WiFi network
  Serial.println();
  Serial.println();
  Serial.print(F("Connecting to "));
  Serial.println(ssid);

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(F("."));
  }
  Serial.println();
  Serial.println(F("WiFi connected"));

  // Start the server
  server.begin();
  Serial.println(F("Server started"));

  // Print the IP address
  Serial.println(WiFi.localIP());
}

void loop() {
  // Check if a client has connected
  WiFiClient client = server.accept();
  if (!client) { return; }

  Serial.println(F("new client"));
  client.setTimeout(5000);  // default is 1000

  // Read the first line of the request
  String req = client.readStringUntil('\r');
  Serial.println(F("request: "));
  Serial.println(req);

  // Match the request
  if (req.indexOf(F("/gpio/0")) != -1) {
    ledValue = __ESP_LED_OFF;
  } else if (req.indexOf(F("/gpio/1")) != -1) {
    ledValue = __ESP_LED_ON;
  } else {
    Serial.println(F("invalid request"));
  }

  // Set LED according to the request
  digitalWrite(__ESP_LED_BUILTIN, ledValue);

  // read/ignore the rest of the request
  // do not client.flush(): it is for output only, see below
  while (client.available()) {
    // byte by byte is not very efficient
    client.read();
  }

  // Send the response to the client
  // it is OK for multiple small client.print/write,
  // because nagle algorithm will group them into one single packet
  client.print(F("HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n<!DOCTYPE HTML>\r\n<html>\r\nGPIO is now "));
  client.print((ledValue) ?  F("low") : F("high"));
  client.print(F("<br><br>Click <a href='http://"));
  client.print(WiFi.localIP());
  client.print(F("/gpio/1'>here</a> to switch LED GPIO on, or <a href='http://"));
  client.print(WiFi.localIP());
  client.print(F("/gpio/0'>here</a> to switch LED GPIO off.</html>"));

  // The client will actually be *flushed* then disconnected
  // when the function returns and 'client' object is destroyed (out-of-scope)
  // flush = ensure written data are received by the other side
  Serial.println(F("Disconnecting from client"));
}
