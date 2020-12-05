/*
 * Common mistakes & tips:
 *   * Don't just connect the IR LED directly to the pin, it won't
 *     have enough current to drive the IR LED effectively.
 *   * Make sure you have the IR LED polarity correct.
 *     See: https://learn.sparkfun.com/tutorials/polarity/diode-and-led-polarity
 *   * Typical digital camera/phones can be used to see if the IR LED is flashed.
 *     Replace the IR LED with a normal LED if you don't have a digital camera
 *     when debugging.
 *   * Avoid using the following pins unless you really know what you are doing:
 *     * Pin 0/D3: Can interfere with the boot/program mode & support circuits.
 *     * Pin 1/TX/TXD0: Any serial transmissions from the ESP8266 will interfere.
 *     * Pin 3/RX/RXD0: Any serial transmissions to the ESP8266 will interfere.
 *   * ESP-01 modules are tricky. We suggest you use a module with more GPIOs
 *     for your first time. e.g. ESP-12 etc.
 *     https://github.com/abhrodeep/Arduino_projs
 */
#ifndef UNIT_TEST
#include <Arduino.h>
#endif
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <IRremoteESP8266.h>
#include <IRsend.h>
#include <WiFiClient.h>
#include <vector>
#include <iostream>
#include<sstream> 
#include<string>
using namespace std;
const char *ssid = "10012503";
const char *password = "gd10012503";
MDNSResponder mdns;

ESP8266WebServer server(80);

const uint16_t kIrLed = 4; // ESP8266 GPIO pin to use. Recommended: 4 (D2).
IRsend irsend(kIrLed);     // Set the GPIO to be used to sending the message.

void handleRoot()
{
  server.send(200, "text/html",
              "<html>"
              "<head><title>ESP8266 Demo</title></head>"
              "<meta http-equiv=\"Content-Type\" content=\"text/html;charset=utf-8\">"
              "<body>"
              "<h1>Hello from ESP8266, you can send NEC encoded IR"
              "signals from here!</h1>"
              "<p><a href=\"ir?code=551489775\">Send 20DF10EF</a></p>"
              "<p><a href=\"ir?code=16429347\">Send 0xFAB123</a></p>"
              "<p><a href=\"ir?code=16771222\">Send 0xFFE896</a></p>"
              "<p><a href=\"ir?code=16771222\">Box 40 AF 01 TurnOff</a></p>"
              "<p><a href=\"ir?code=16771222\">00 FF 40 TurnOff</a></p>"
              "<p><a href=\"ir?code=404,423,0156\">TV F10E0E0C TurnOff</a></p>"
              "</body>"
              "</html>");
}

string replaceCommaToSpace(string s) {
  int n = s.length();
  for (int i = 0; i < n; ++i){
    if (s[i] == ','){
      s[i] = ' ';
    }
  }
  return s;
}

//
void sendCode(string message, string type) {
  message = replaceCommaToSpace(message);
  istringstream is(message);
  vector<uint16_t> v;
  uint16_t i;
  while(is>>i)
  {
      v.push_back(i);
  }
  uint16_t rawData[v.size()];
  for(int i=0;i<v.size();i++) {
    rawData[i] = v[i];
  }
  
  Serial.println("start to send IR");
  irsend.sendRaw(rawData, v.size(), 38);
  //irsend.sendRaw(rawData2, 73, 38);
  //v.clear();
  //delete []rawData;
  Serial.println("end to send IR");
}
//
void handleIr()
{
  string type = "";
  string code = "";

  for (uint8_t i = 0; i < server.args(); i++)
  {
    if (server.argName(i) == "code")
    {
      code = server.arg(i).c_str();
      continue;
    }
    if(server.argName(i) == "type") {
      type = server.arg(i).c_str();
    }
  }
  
  Serial.println("start to send IR handleIr");
  sendCode(code,type);
  Serial.println("end to send IR handleIr");
  server.send(200, "text/plain", "OK");
}

void handleNotFound()
{
  String message = "File Not Found\n\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += (server.method() == HTTP_GET) ? "GET" : "POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";
  for (uint8_t i = 0; i < server.args(); i++) {
    message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
  }  
  server.send(404, "text/plain", message);
}

void setup(void)
{
  irsend.begin();

  Serial.begin(115200);
  WiFi.begin(ssid, password);
  Serial.println("");

  // Wait for connection
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  if (mdns.begin("esp8266", WiFi.localIP()))
  {
    Serial.println("MDNS responder started");
  }

  server.on("/", handleRoot);
  server.on("/ir", handleIr);
  server.on("/inline", []() {
    server.send(200, "text/plain", "this works as well");
  });
  server.onNotFound(handleNotFound);
  server.begin();
  Serial.println("HTTP server started");
}

void loop(void)
{
  server.handleClient();
}
