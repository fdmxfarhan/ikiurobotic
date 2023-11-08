#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>

#ifndef STASSID
#define STASSID "FDMX"
#define STAPSK  "123454321"
#endif

// #define Name "MOV"
#define Name "MOV"

const char* ssid     = STASSID;
const char* password = STAPSK;

const char* host = "192.168.124.148";
const uint16_t port = 3000;

ESP8266WiFiMulti WiFiMulti;

WiFiClient client;
char rec;
bool led = false;;
int distance=0;

void setup() {
  pinMode(12, OUTPUT);
  pinMode(13, OUTPUT);
  pinMode(14, OUTPUT);
  pinMode(16, OUTPUT);
  pinMode(2, OUTPUT);
  digitalWrite(2, 1);
  Serial.begin(115200);
  
  WiFi.mode(WIFI_STA);
  WiFiMulti.addAP(ssid, password);
  while (WiFiMulti.run() != WL_CONNECTED) delay(500);
  digitalWrite(2, 0);
  while (!client.connect(host, port)) delay(100);
}


void loop() {
  
  while (!client.connected()){
    digitalWrite(2,1);
    delay(100);
    client.connect(host, port);
  }
  digitalWrite(2,0);
  if(client.available()){
    rec = client.read();
    if(rec == '?'){
      client.print(Name);
    }else Serial.print(rec);
  }
  if(Serial.available() > 0){
    rec = Serial.read();
    client.print(rec);
  }
  
}

int srf(int pin){
  pinMode(pin,OUTPUT);
  digitalWrite(pin,1);
  delayMicroseconds(10);
  digitalWrite(pin,0);
  pinMode(pin,INPUT);
  return(pulseIn(pin, 1)*0.034/2);
}