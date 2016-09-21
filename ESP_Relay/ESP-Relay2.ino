#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <WebSocketsServer.h>
#include <ESP8266mDNS.h>
#include <FS.h>

#include "WiFi.h"

const char* host = "esp8266fs";

ESP8266WebServer server(80);
WebSocketsServer webSocket = WebSocketsServer(81);

// Pins connected to relays
const int relayPinsL[] = {16, 4}; // Pins of relays switching L
const int relayPinsN[] = {5, 0}; // Pins of relays switching N

// State
char relayStates[sizeof(relayPinsL)/sizeof(int)+1]; // State of each relay
char current[5] = "1250";
//*****************************************************************************

#define description "Kitchen"

//holds the current upload
File fsUploadFile;

//format bytes
String formatBytes(size_t bytes){
  if (bytes < 1024){
    return String(bytes)+"B";
  } else if(bytes < (1024 * 1024)){
    return String(bytes/1024.0)+"KB";
  } else if(bytes < (1024 * 1024 * 1024)){
    return String(bytes/1024.0/1024.0)+"MB";
  } else {
    return String(bytes/1024.0/1024.0/1024.0)+"GB";
  }
}

String getContentType(String filename){
  if(server.hasArg("download")) return "application/octet-stream";
  else if(filename.endsWith(".htm")) return "text/html";
  else if(filename.endsWith(".html")) return "text/html";
  else if(filename.endsWith(".css")) return "text/css";
  else if(filename.endsWith(".js")) return "application/javascript";
  else if(filename.endsWith(".png")) return "image/png";
  else if(filename.endsWith(".gif")) return "image/gif";
  else if(filename.endsWith(".jpg")) return "image/jpeg";
  else if(filename.endsWith(".ico")) return "image/x-icon";
  else if(filename.endsWith(".xml")) return "text/xml";
  else if(filename.endsWith(".pdf")) return "application/x-pdf";
  else if(filename.endsWith(".zip")) return "application/x-zip";
  else if(filename.endsWith(".ttf")) return "font/ttf";
  else if(filename.endsWith(".eot")) return "application/octet-stream";
  else if(filename.endsWith(".woff")) return "font/woff";
  else if(filename.endsWith(".woff2")) return "font/woff2";
  else if(filename.endsWith(".gz")) return "application/x-gzip";
  return "text/plain";
}

bool handleFileRead(String path){
  Serial.println("handleFileRead: " + path);
  if(path.endsWith("/")) path += "index.htm";
  String contentType = getContentType(path);
  String pathWithGz = path + ".gz";
  if(SPIFFS.exists(pathWithGz) || SPIFFS.exists(path)){
    if(SPIFFS.exists(pathWithGz))
      path += ".gz";
    File file = SPIFFS.open(path, "r");
    size_t sent = server.streamFile(file, contentType);
    file.close();
    return true;
  }
  return false;
}

String initJson(){
  String data = "{\"msg\":\"initial\",";
  data += "\"Relays\":[";
  for(int i=0; i < (sizeof(relayPinsL)/sizeof(int)-1); i++){
    data += "{\"State\":" + String(relayStates[i]) +"},";
  }
  data += "{\"State\":" + String(relayStates[sizeof(relayPinsL)/sizeof(int)-1]) +"}],";
  data += "\"Name\":\"" + String(description) + "\",";
  data += "\"Current\":" + String(current) + "}";
  return data;
}

void webSocketEvent(uint8_t num, WStype_t type, uint8_t * payload, size_t lenght) {
  switch(type) {
    case WStype_DISCONNECTED:
      Serial.printf("[%u] Disconnected!\n", num);
      break;
    case WStype_CONNECTED:
      {
        IPAddress rIp = webSocket.remoteIP(num);
        Serial.printf("[%u] Connected from %d.%d.%d.%d url: %s\n", num, rIp[0], rIp[1], rIp[2], rIp[3], payload);

        // send message to client
        webSocket.sendTXT(num, initJson().c_str());
      }
      break;
    case WStype_TEXT:
      char *data = (char *)payload;
      char *messageType;
      char *relayNumber;
      char *relayState;
      char seperator[] = ";";
      
      Serial.println(data);
      
      messageType = strtok(data, seperator);
      Serial.print("MSG Type:" );
      Serial.println(messageType);
      
      // Type of message 0 = switch
      if(strcmp(messageType,"0") == 0){
        int iRelayNumber;
        int iRelayState;
        
        relayNumber = strtok(NULL, seperator);
        Serial.print("Relay:   ");
        iRelayNumber = strtol(relayNumber, NULL, 10);
        Serial.println(iRelayNumber);
        
        relayState = strtok(NULL, seperator);
        Serial.print("State:   ");
        Serial.println(relayState);
        iRelayState = strtol(relayState, NULL, 10);
        
        //Save State
        relayStates[iRelayNumber] = relayState[0];

        //Set relay
         digitalWrite(relayPinsL[iRelayNumber], iRelayState);
         digitalWrite(relayPinsN[iRelayNumber], iRelayState);
      }
      
      
      if(payload[0] == '#') {
          uint32_t rgb = (uint32_t) strtol((const char *) &payload[1], NULL, 16);
      }
      break;
  }
}

void setupWebSocket(){
  // start webSocket server
  webSocket.begin();
  webSocket.onEvent(webSocketEvent);
}

void setupState(){
  // Set Initial stat to off for all relays
  for(int i=0; i < (sizeof(relayPinsL)/sizeof(int)); i++){
    relayStates[i] = '0';
  }
  relayStates[sizeof(relayPinsL)/sizeof(int)] = '\0';
}

void setupRelays(){

  for(int i=0; i < (sizeof(relayPinsL)/sizeof(int)); i++){
    pinMode(relayPinsL[i], OUTPUT);
    digitalWrite(relayPinsL[i], LOW);
  }

  for(int i=0; i < (sizeof(relayPinsN)/sizeof(int)); i++){
    pinMode(relayPinsN[i], OUTPUT);
    digitalWrite(relayPinsN[i], LOW);
  }
}

void setup(void){
  Serial.begin(115200);
  Serial.print("\n");
  //Serial.setDebugOutput(true);
  SPIFFS.begin();
  {
    Dir dir = SPIFFS.openDir("/");
    while (dir.next()) {
      String fileName = dir.fileName();
      size_t fileSize = dir.fileSize();
      Serial.printf("FS File: %s, size: %s\n", fileName.c_str(), formatBytes(fileSize).c_str());
    }
    Serial.printf("\n");
  }

  //WIFI INIT
  Serial.printf("Connecting to %s\n", ssid);

  beginWiFi();

  Serial.println("");
  Serial.print("Connected! IP address: ");
  Serial.println(WiFi.localIP());

  MDNS.begin(host);
  Serial.print("Open http://");
  Serial.print(host);
  Serial.println(".local/edit to see the file browser");

  //called when the url is not defined here
  //use it to load content from SPIFFS
  server.onNotFound([](){
    if(!handleFileRead(server.uri()))
      server.send(404, "text/plain", "FileNotFound");
  });

  server.begin();
  Serial.println("HTTP server started");

  setupState(); // Initialize state variable

  //Start websocket
  setupWebSocket();
}

void loop(void){
  server.handleClient();
  delay(100);
  webSocket.loop();
  delay(100);
}
