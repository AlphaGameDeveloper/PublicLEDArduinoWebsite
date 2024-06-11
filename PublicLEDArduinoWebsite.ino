#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>

#include "html_main.h"
#include "led_html_states.h"
#include "htmx.h"

#ifndef STASSID
#define STASSID "5PineViewLane"
#define STAPSK "castillo"
#endif

const char* ssid = STASSID;
const char* password = STAPSK;

ESP8266WebServer server(80); // start server on port 80

// Stuff for static IP address - https://randomnerdtutorials.com/esp8266-nodemcu-static-fixed-ip-address-arduino/
IPAddress local_IP(192, 168, 0, 101); // 192.168.0.101 - on subnet and unused
IPAddress gateway(192, 168, 0, 1);

IPAddress subnet(255, 255, 255, 0);
IPAddress primaryDNS(8, 8, 8, 8);   //optional
IPAddress secondaryDNS(8, 8, 4, 4); //optional

// Define LED numbers
const int led = 13;
const int led_error = D7;
const int led_request = D1;
const int led_1 = D7;
const int led_2 = D6;
const int led_3 = D5;

int led_1_status = LOW;
int led_2_status = LOW;
int led_3_status = LOW;

void startRequest() { digitalWrite(led_request, HIGH); }
void endRequest() { digitalWrite(led_request, LOW); }

void handleRoot() {
  digitalWrite(led, 1);
  server.send(200, "text/html", HTML_INDEX);
  digitalWrite(led, 0);
}

void handleNotFound() {
  startRequest();
  String message = "404 Not Found\n\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += (server.method() == HTTP_GET) ? "GET" : "POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";
  for (uint8_t i = 0; i < server.args(); i++) { message += " " + server.argName(i) + ": " + server.arg(i) + "\n"; }
  server.send(404, "text/plain", message);
  endRequest();
}

int contraryState(int in) {
  if (in == HIGH) {
    return LOW;
  }
  else {
    return HIGH;
  }
}

int getLedPin(int led) {
  int ledpin;
  if (led == 1) {
    ledpin = D7;
  }
  else if (led == 2) {
    ledpin = D6;
  }
  else if (led == 3) {
    ledpin = D5;
  }
  else {
    Serial.print("getLedPin was requested to change nonexistant nonled #");
    Serial.println(led);
    return 0;
  }
  return ledpin;
}
void handleLedStatusChange(int led) {
  int ledpin = getLedPin(led);

  digitalWrite(ledpin, contraryState(digitalRead(ledpin)));

  server.send(200, "text/html", "OK!"); // we send text/html as our mimetype (telling the browser what kind of data we are sending)
}

const char* getStatusHTMLString(int led) {
    int state = digitalRead(getLedPin(led));
    const char* data;
    if (led == 1) {
        if (state == HIGH) {
            server.send(200, "text/html", LED_1_ON);
        } else if (state == LOW) {
            server.send(200, "text/html", LED_1_OFF);
        }
    } else if (led == 2) {
        if (state == HIGH) {
            server.send(200, "text/html", LED_2_ON);
        } else if (state == LOW) {
            server.send(200, "text/html", LED_2_OFF);
        }
    } else if (led == 3) {
        if (state == HIGH) {
            server.send(200, "text/html", LED_3_ON);
        } else if (state == LOW) {
            server.send(200, "text/html", LED_3_OFF);
        }
    }
    else {
      server.send(500, "text/plain", "Internal Server Error: invalid led requested internally");
      return "500 internal server error";
    }
    return "200 OK";
}

void returnVariable(String v, String m) {
	Server.send(200, m, v);
	Serial.println("Static file served!");
}

void setup(void) {
  pinMode(led, OUTPUT);
  pinMode(led_1, OUTPUT);
  pinMode(led_2, OUTPUT);
  pinMode(led_3, OUTPUT);
  pinMode(led_request, OUTPUT);
  digitalWrite(led, 0);
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.println("Connecting to WiFi");
  Serial.print("WiFi Network SSID: ");
  Serial.println(ssid);

  if (!WiFi.config(local_IP, gateway, subnet, primaryDNS, secondaryDNS)) {
    Serial.println("STA Failed to configure");
  }

  // Wait for wifi connection
  Serial.print("Connecting to WiFi network")
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  // MDNS was in the template. I don't know WHY it is needed,
  // but I will keep it just in case.
  if (MDNS.begin("esp8266")) { 
  	Serial.println("MDNS responder started"); 
  }

  server.on("/", handleRoot);

  server.on("/led/1", []() { getStatusHTMLString(1); });
  server.on("/led/2", []() { getStatusHTMLString(2); });
  server.on("/led/3", []() { getStatusHTMLString(3); });

  server.on("/led/1/toggle", []() { handleLedStatusChange(1); }); 
  server.on("/led/2/toggle", []() { handleLedStatusChange(2); }); 
  server.on("/led/3/toggle", []() { handleLedStatusChange(3); }); 

  // htmx
  server.on("/htmx.js", []() { returnVariable(HTMX_JS, "text/javascript") })
    
  server.onNotFound(handleNotFound);

  server.begin();
  Serial.println("HTTP server started");
}

void loop(void) {
  server.handleClient();
  MDNS.update();
}
