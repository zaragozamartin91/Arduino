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
#include <ESP8266mDNS.h>
#include "MyMacros.h"
#include <string.h>
#include <stdio.h>


// According to the Arduino blink sketch (and this blog https://www.best-microcontroller-projects.com/esp-01-vs-esp-01s.html)... 
// the true led builtin is pin #2
#define __ESP_LED_BUILTIN 2

// wifi creds
#include "__SSID__.h"

const char* ssid = STASSID;
const char* password = STAPSK;

// Create an instance of the server
// specify the port to listen on as an argument
WiFiServer server(80);


/* On esp8266, low voltage on gpio2 turns the led on
https://forum.arduino.cc/t/want-to-blink-blue-led-on-esp8266-01/486496/4 
Read more here: https://www.best-microcontroller-projects.com/esp-01-vs-esp-01s.html
*/
#define __ESP_LED_ON 0
#define __ESP_LED_OFF 1
int ledValue = __ESP_LED_OFF;

void setup() {
  Serial.begin(9600);
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
  Serial.print("Local ip: ");Serial.println(WiFi.localIP());

  // More on MDNS: https://circuits4you.com/2017/12/31/esp8266-mdns/
  MDNS.begin("espled");
  Serial.println("Local network address: espled.local");
}

#define __LOOP_DELAY 200
void loop() {
  MDNS.update();

  if (!server.hasClient()) {
    // looping with delay to update MDNS server frequently
    delay(__LOOP_DELAY);
    return;
  }
  WiFiClient client = server.accept();

  debugPrint(F("new client"));
  client.setTimeout(5000);  // default is 1000

  String clientRequest;
  for (int availableBytes = client.available() ; availableBytes > 0 ; availableBytes = client.available()) {
    char buffer[availableBytes];
    client.readBytes(buffer, availableBytes);
    clientRequest.concat(buffer, availableBytes);
  }

  debugPrintArgs("Request: \n", clientRequest);

  // Match the request
  if (clientRequest.indexOf(F("/gpio/0")) != -1) {
    ledValue = __ESP_LED_OFF;
  } else if (clientRequest.indexOf(F("/gpio/1")) != -1) {
    ledValue = __ESP_LED_ON;
  } else if (clientRequest.indexOf(F("/ledlight/")) != -1) {
    int sidx = clientRequest.indexOf(F("/ledlight/"));
    String ledlightRequest = clientRequest.substring(sidx);
    ledlightRequest.replace("/ledlight/", "");
    debugPrintArgs("ledlightRequest: ", ledlightRequest);
    
  } else {
    debugPrint(F("invalid request"));
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
  debugPrint(F("Disconnecting from client"));
}
