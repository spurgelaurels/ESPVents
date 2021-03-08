#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <Servo.h>

#ifndef STASSID
#define STASSID "SSID"
#define STAPSK  "PSK"
#endif

const char* ssid = STASSID;
const char* password = STAPSK;

ESP8266WebServer server(80);

const int led = 13;

Servo servo;
int angle = 10;               // Initial angle can be set
boolean vent_status = true;   // true = open 


void handleRoot() {
  digitalWrite(led, 1);
  server.send(200, "text/html", "<form action=\"/VENT\" method=\"POST\"><input type=\"submit\" value=\"Toggle Vent\"></form>");
  digitalWrite(led, 0);
}

void handleNotFound() {
  digitalWrite(led, 1);
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
  digitalWrite(led, 0);
}



void setup(void) {
  pinMode(led, OUTPUT);
  digitalWrite(led, 0);
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.println("");

  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  if (MDNS.begin("esp8266")) {
    Serial.println("MDNS responder started");
  }

  server.on("/", handleRoot);

  server.on("/inline", []() {
    server.send(200, "text/html", "this works as well");
  });

  server.onNotFound(handleNotFound);

  server.begin();
  Serial.println("HTTP server started");
  servo_setup();
  server.on("/", HTTP_GET, handleRoot);     // Call the 'handleRoot' function when a client requests URI "/"
  server.on("/VENT", HTTP_POST, handleVent);  // Call the 'handleVent' function when a POST request is made to URI "/VENT"
  server.onNotFound(handleNotFound);        // When a client requests an unknown URI (i.e. something other than "/"), call f
}

void servo_setup() {
  servo.attach(2);      // GPIO Pin 2
  servo.write(angle);
}

void handleVent() {
  Serial.print(vent_status);
  if ( vent_status ){
    servo_open();
    vent_status = !vent_status;
  }
  else {
    servo_closed();
    vent_status = !vent_status;
  }
  
  server.sendHeader("Location","/");        // Add a header to respond with a new location for the browser to go to the home page again
  server.send(303);  
}

void servo_closed() {
   // scan from 0 to 180 degrees
  for(angle = 10; angle < 180; angle++)  
  {                                  
    servo.write(angle);               
    delay(15);                   
  } 
}

void servo_open() {
  // now scan back from 180 to 0 degrees
  for(angle = 180; angle > 10; angle--)    
  {                                
    servo.write(angle);           
    delay(15);       
  } 
}


void loop(void) {
  server.handleClient();
  MDNS.update();

}
