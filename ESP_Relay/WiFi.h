#include <WiFiClient.h>
#include <ESP8266WiFi.h>

#define retryMax 90

//*****************************************************************************
// WiFI
IPAddress ip;
IPAddress broadcastIp;

char ssid[100] = "\0";
char password[100] = "\0";

IPAddress getIpWiFi(){
  return WiFi.localIP();  
}

IPAddress getApIpWiFi(){
  return WiFi.softAPIP();
}

void startAP(){
  Serial.println("Set WiFi to AP!");
  WiFi.mode(WIFI_OFF);
  delay(200);
  WiFi.softAP("ESPRelay", "password1234");
  
}

void beginWiFi(){
  int retry = 0; // retry counter

  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  delay(200);

  EEPROM.begin(512); 
  EEPROM.get(0, ssid);
  EEPROM.get(100, password);
  EEPROM.end(); 

  Serial.println(ssid);
  if (password != '\0'){
    WiFi.begin ( ssid, password );
  }else{
    WiFi.begin ( ssid);
  }
  delay(200);
  
  while ( WiFi.status() != WL_CONNECTED ) {
    delay ( 200 );
    Serial.print ( "." );
    retry += 1;
    if (retry == retryMax){
      startAP();
      break;
    }
  }
  
  if(retry < retryMax){
    ip =  getIpWiFi();
  } else {
    ip =   getApIpWiFi();
  }
  /*
  struct ip_info ipInfo;
  wifi_get_ip_info(STATION_IF, &ipInfo);
  Serial.println( IPAddress(ipInfo.netmask.addr));
  */
}

