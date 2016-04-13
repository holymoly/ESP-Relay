#include <SimpleTimer.h>
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <WiFiUdp.h>
#include <ESP8266WebServer.h>
#include "SVG.h"

//*****************************************************************************
// Server

ESP8266WebServer server(80);

//*****************************************************************************
// Defines
#define LED 0          // Led in NodeMCU at pin GPIO16 (D0).
#define pinRelay1 16
#define pinRelay2 15
#define pinRelay3 14
#define pinRelay4 13

#define udp_port 8981
#define description "Living Room"
#define maxRemoteEsps 9

//*****************************************************************************
// Array of remote ESP8266
struct REMOTE_ESP{
  char name[255] = "";
  IPAddress ip;
  unsigned int udpPort = 0;
  unsigned int current = 0;
  char relayState[5] = "0000";
};

REMOTE_ESP remote_esps[maxRemoteEsps];

//*****************************************************************************
// WiFI
const char *ssid = "blabla";
const char *password = "blabla";
IPAddress ip;

//*****************************************************************************
// UDP
WiFiUDP udp;
IPAddress broadcastIp;
char packetBuffer[255]; //buffer to hold incoming packet

//*****************************************************************************
// Task
SimpleTimer udpBroadcasTimer;

//*****************************************************************************
// State
char relayState[5] = "0000";
char current[5] = "1250";

//*****************************************************************************
// Code
void buildHtml(){
  digitalWrite(LED, 0);
  String message = "<head> <meta http-equiv='refresh' content='5;url=/' /> </head>";
  message += description; // Area
  message += "<br>";
  message += "\nState: ";
  message += relayState;
  message += "<br>";
  //check relay state
  if (relayState[0] == '0'){
    message += "<a href='/1ON'>";
    message += svg_relayOff;
    message += "</a>";
  }else{
    message += "<a href='/1Off'>";
    message += svg_relayOn;
    message += "</a>";
  }
  if (relayState[1] == '0'){
    message += "<a href='/2ON'>";
    message += svg_relayOff;
    message += "</a>";
  }else{
    message += "<a href='/2Off'>";
    message += svg_relayOn;
    message += "</a>";
  }
  if (relayState[2] == '0'){
    message += "<a href='/3ON'>";
    message += svg_relayOff;
    message += "</a>";
  }else{
    message += "<a href='/3Off'>";
    message += svg_relayOn;
    message += "</a>";
  }
  if (relayState[3] == '0'){
    message += "<a href='/4ON'>";
    message += svg_relayOff;
    message += "</a>";
  }else{
    message += "<a href='/4Off'>";
    message += svg_relayOn;
    message += "</a>";
  }
  message += "<br>";
  message += "\nCurrent: ";
  message += current;
  message += "<br>";
  message += "\nIP: ";
  message += ip[0];
  message += ".";
  message += ip[1];
  message += ".";
  message += ip[2];
  message += ".";
  message += ip[3];
  message += "<br>";
  message += "<br>";
   
  //Remote ESP-Relays
  for( int i=0; i < maxRemoteEsps; i++){
    if(strcmp("",remote_esps[i].name) != 0){
      message += remote_esps[i].name;
      message += "<br>";
      message += "\nState: ";
      message += remote_esps[i].relayState;
      message += "<br>";
      //check relay state
      // /001 -> 0 = 1ESP-Relay 0=first relay(0,1,2 or 3) 1=ON
      if (remote_esps[i].relayState[0] == '0'){
        message += "<a href='/";
        message += i;
        message += "01'>";
        message += svg_relayOff;
        message += "</a>";
      }else{
        message += "<a href='/";
        message += i;
        message += "00'>";
        message += svg_relayOn;
        message += "</a>";
      }
      if (remote_esps[i].relayState[1] == '0'){
        message += "<a href='/";
        message += i;
        message += "11'>";
        message += svg_relayOff;
        message += "</a>";
      }else{
        message += "<a href='/";
        message += i;
        message += "10'>";
        message += svg_relayOn;
        message += "</a>";
      }
      if (remote_esps[i].relayState[2] == '0'){
        message += "<a href='/";
        message += i;
        message += "21'>";
        message += svg_relayOff;
        message += "</a>";
      }else{
       message += "<a href='/";
        message += i;
        message += "20'>";
        message += svg_relayOn;
        message += "</a>";
      }
      if (remote_esps[i].relayState[3] == '0'){
        message += "<a href='/";
        message += i;
        message += "31'>";
        message += svg_relayOff;
        message += "</a>";
      }else{
        message += "<a href='/";
        message += i;
        message += "30'>";
        message += svg_relayOn;
        message += "</a>";
      }
      message += "<br>";
      message += "\nCurrent: ";
      message += remote_esps[i].current;
      message += "<br>";
      message += "\nIP: ";
      message += remote_esps[i].ip[0];
      message += ".";
      message += remote_esps[i].ip[1];
      message += ".";
      message += remote_esps[i].ip[2];
      message += ".";
      message += remote_esps[i].ip[3];
      message += "<br>";
    }
  }
  
  server.send(200, "text/html", message);
  digitalWrite(LED, 1);
}

void handleRoot() {
  buildHtml();
}

void relay1On(){
  digitalWrite(pinRelay1, HIGH);
  relayState[0] = '1';
  buildHtml();
}
void relay1Off(){
  digitalWrite(pinRelay1, LOW); 
  relayState[0] = '0';
  buildHtml();
}

void relay2On(){
  digitalWrite(pinRelay2, HIGH); 
  relayState[1] = '1';
  buildHtml();
}
void relay2Off(){
  digitalWrite(pinRelay2, LOW);
  relayState[1] = '0'; 
  buildHtml();
}

void relay3On(){
  digitalWrite(pinRelay3, HIGH); 
  relayState[2] = '1'; 
  buildHtml();
}
void relay3Off(){
  digitalWrite(pinRelay3, LOW); 
  relayState[2] = '0'; 
  buildHtml();
}

void relay4On(){
  digitalWrite(pinRelay4, HIGH); 
  relayState[3] = '1'; 
  buildHtml();
}
void relay4Off(){
  digitalWrite(pinRelay4, LOW); 
  relayState[3] = '0'; 
  buildHtml();
}

void handleNotFound(){
  digitalWrite(LED, 1);
  Serial.println(server.uri());
  // which ESP-Relay
  if (isdigit(server.uri()[1])){
    int i = server.uri()[1] - '0';
    Serial.print("Set ");
    Serial.print(remote_esps[i].name);

    // which Relay
    if (isdigit(server.uri()[2])){
      int n = server.uri()[2] - '0';
      Serial.print(" Relay ");
        Serial.print( n + 1 );

        // which state
        if (isdigit(server.uri()[3])){
          int remoteState = server.uri()[3] - '0';
          Serial.print(" to ");
          Serial.println( remoteState );
          Serial.print(remote_esps[i].ip);
          Serial.print(remote_esps[i].udpPort);
          
          udp.beginPacket(remote_esps[i].ip, udp_port);
          udp.write("3;");
          udp.write(n + '0');
          udp.write(remoteState + '0');
          udp.endPacket();
        }
    }
  }
  buildHtml();
  digitalWrite(LED, 0);
}
void udpBroadcast(){
  digitalWrite(LED, 0);
  Serial.println ( "Hello Broadcast send" );
  
  // transmit broadcast package
  udp.beginPacket(broadcastIp, udp_port);
  udp.write("1");
  udp.endPacket();
  digitalWrite(LED, 1);
}

void checkUdpData(){
  // if there's data available, read a packet
  int packetSize = udp.parsePacket();
  if (packetSize) {
    digitalWrite(LED, 0);
    Serial.print("Received UDP packet of size ");
    Serial.println(packetSize);
    Serial.print("From ");
    IPAddress remoteIp = udp.remoteIP();
    Serial.print(remoteIp);
    Serial.print(", port ");
    Serial.println(udp.remotePort());

    // read the packet into packetBufffer
    int len = udp.read(packetBuffer, 255);
    if (len > 0) {
      packetBuffer[len] = 0;
    }
    Serial.print("Contents:");
    Serial.println(packetBuffer);
    
    // Reply to broadcast
    if ( packetBuffer[0] == '1' ){
      Serial.println("Sending UDP response");
      // send a reply, to the IP address and port that sent us the packet we received
      udp.beginPacket(udp.remoteIP(), udp.remotePort());
      udp.write("2;");
      udp.write(description);
      udp.write(";");
      udp.write(relayState);
      udp.write(";");
      udp.write(current);
      // Add relay data and current
      udp.endPacket();
    } 

    // Reply to broadcast
    if ( packetBuffer[0] == '3' ){
      Serial.println("Remote Control");
      Serial.println(packetBuffer[2]);
      switch ( packetBuffer[2] - '0' ) {
        case 0:
          Serial.println(" Set Relay 1");
          if(packetBuffer[3] == '0'){
            relay1Off();
          } else {
            relay1On();
          }
          break;
        case 1:
         Serial.println(" Set Relay 2");
          if(packetBuffer[3] == '0'){
            relay2Off();
          } else {
            relay2On();
          }
          break;
        case 2:
          Serial.println(" Set Relay 3");
          if(packetBuffer[3] == '0'){
            relay3Off();
          } else {
            relay3On();
          }
          break;
        case 3:
         Serial.println(" Set Relay 4");
          if(packetBuffer[3] == '0'){
            relay4Off();
          } else {
            relay4On();
          }
          break;
        default: 
          // if nothing else matches, do the default
          // default is optional
        break;
      }
    } 

    // Receiving broadcast respons
    if ( packetBuffer[0] == '2' ){
      Serial.println("Receiving UDP response");
      // parse data and add/update remote_esps
      int field = 0;
      int offset = 2; // 2 becauss packet starts with "2,"
             
      char nameBuffer[255];
      char relayBuffer[5];
      char currentBuffer[5];
      
      for( int i=2; i < packetSize; i++){
        Serial.print(packetBuffer[i]);
        
        if( packetBuffer[i] == ';' ){
          offset = i + 1;
          field = field + 1;
        }
        
        switch ( field ) {
          case 0:
            nameBuffer[i-offset] = packetBuffer[i];
            nameBuffer[i-offset+1] = '\0';
            break;
          case 1:
            relayBuffer[i-offset] = packetBuffer[i];
            relayBuffer[i-offset+1] = '\0';
            break;
          case 2:
            currentBuffer[i-offset] = packetBuffer[i];
            currentBuffer[i-offset+1] = '\0';
            break;
          default: 
            // if nothing else matches, do the default
            // default is optional
          break;
        }
      }
      
      Serial.println("");
      Serial.println(nameBuffer);
      Serial.println(relayBuffer);
      Serial.println(currentBuffer);
      
      int updateEntry = -1;
      for( int i=0; i < maxRemoteEsps; i++){
        // if name exists in array
        if(strcmp(nameBuffer,remote_esps[i].name) == 0){
          updateEntry = i;
          break;
        }
        if(strcmp("",remote_esps[i].name) == 0 && updateEntry == -1){
          updateEntry = i;
        }
      }
      
      Serial.print("Update entry: ");
      Serial.println(updateEntry);
      
      strcpy(remote_esps[updateEntry].name, nameBuffer);
      remote_esps[updateEntry].ip = udp.remoteIP();
      remote_esps[updateEntry].udpPort = udp.remotePort();
      strcpy(remote_esps[updateEntry].relayState, relayBuffer);
      remote_esps[updateEntry].current = atoi(currentBuffer);
      
      // Print updated data
      Serial.println(remote_esps[updateEntry].name);
      Serial.println(remote_esps[updateEntry].ip);
      Serial.println(remote_esps[updateEntry].udpPort);
      Serial.println(remote_esps[updateEntry].current);
    } 
  }
  digitalWrite(LED, 1);
}

void setup ( void ) {
	pinMode ( LED, OUTPUT );
	digitalWrite ( LED, HIGH );
  pinMode ( pinRelay1, OUTPUT );
  digitalWrite ( pinRelay1, LOW );
  pinMode ( pinRelay2, OUTPUT );
  digitalWrite ( pinRelay2, LOW );
  pinMode ( pinRelay3, OUTPUT );
  digitalWrite ( pinRelay3, LOW );
  pinMode ( pinRelay4, OUTPUT );
  digitalWrite ( pinRelay4, LOW );
  // Start serial
	Serial.begin ( 115200 );

  // Start and connect to WiFI
	WiFi.begin ( ssid, password );
	Serial.println ( "" );

	// Wait for connection
	while ( WiFi.status() != WL_CONNECTED ) {
		delay ( 200 );
		Serial.print ( "." );
	}

	Serial.println ( "" );
	Serial.print ( "Connected to " );
	Serial.println ( ssid );
	Serial.print ( "IP address: " );
	Serial.println ( WiFi.localIP() );
  ip = WiFi.localIP();

  // Start UDP Server
  udp.begin(udp_port);
  broadcastIp = WiFi.localIP();
  broadcastIp[3] = 255;
  
  Serial.println ( "" );
  Serial.print ( "UDP Server started on Port: " );
  Serial.println ( udp_port );
  Serial.print ( "UDP broadcast IP: " );
  Serial.println ( broadcastIp );

  Serial.println("Start WebServer");
  server.on("/", handleRoot);
  server.on("/1ON", relay1On);
  server.on("/1Off", relay1Off);
  server.on("/2ON", relay2On);
  server.on("/2Off", relay2Off);
  server.on("/3ON", relay3On);
  server.on("/3Off", relay3Off);
  server.on("/4ON", relay4On);
  server.on("/4Off", relay4Off);
  
  server.onNotFound(handleNotFound);
  server.begin();
  
  digitalWrite ( LED, LOW );
  delay(500);
  digitalWrite ( LED, HIGH );
  
  udpBroadcasTimer.setInterval(5000,udpBroadcast);
}

void loop ( void ) {
  udpBroadcasTimer.run();
  checkUdpData();
  server.handleClient();
}

