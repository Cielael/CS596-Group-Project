#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266HTTPClient.h>
#include <ESP8266mDNS.h>
#include <Arduino_JSON.h>

#ifndef STASSID
#define STASSID "NETGEAR78"
#define STAPSK  "suttonFam"
#endif


JSONVar myObject; 


const char* ssid = STASSID;
const char* password = STAPSK;

ESP8266WebServer server(80);


String httpGETRequest(const char* serverName) {
  HTTPClient http;
    
  // Your IP address with path or Domain name with URL path 
  http.begin(serverName);
  
  // Send HTTP POST request
  int httpResponseCode = http.GET();
  
  String payload = "{}"; 
  
  if (httpResponseCode>0) {
    Serial.print("HTTP Response code: ");
    Serial.println(httpResponseCode);
    payload = http.getString();
  }
  else {
    Serial.print("Error code: ");
    Serial.println(httpResponseCode);
  }
  // Free resources
  http.end();

  return payload;
}


void handleNotFound() {
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

void setup(void) {
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

  server.on("/", [](){
    server.send(200, "text/plain", "home page");
    
    //when ever this endpoint is hit, send a get to backend server and update times list
    //TODO change ip to server
    Serial.print("esp server hit");
    String timeData = httpGETRequest("http://192.168.0.17:3000/state");
    myObject = JSON.parse(timeData);
    Serial.println(myObject);
   
  });
  




  server.onNotFound(handleNotFound);

  server.begin();
  Serial.println("HTTP server started");

  //Get state from server on startup
  String state = httpGETRequest("http://192.168.0.17:3000/state");
  myObject = JSON.parse(state);
}

void loop(void) {
  server.handleClient();
  MDNS.update();

  for(int i = 0; i < myObject.length(); i++){
     JSONVar keys = myObject[i].keys();
     JSONVar state = myObject[i][keys[1]];
 
     
     Serial.print(state);
//     Serial.print(" ");
//     Serial.println(minute);

   

    
     
  }

  
}
