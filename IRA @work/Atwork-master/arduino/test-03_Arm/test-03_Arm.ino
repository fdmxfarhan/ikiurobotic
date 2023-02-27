#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>

#ifndef STASSID
#define STASSID "fdmxfarhan"
#define STAPSK  "fdmxfarhan"
#endif

#define Name "light"

const char* ssid     = STASSID;
const char* password = STAPSK;

const char* host = "192.168.43.210";
const uint16_t port = 3000;

ESP8266WiFiMulti WiFiMulti;

WiFiClient client;
char rec;
int b[8];
int c=0;
int16_t gy;
int speed = 50;
int st = 0, last_st = st;

void setup() {
  pinMode(12, INPUT);
  pinMode(13, INPUT);
  pinMode(14, INPUT);
  pinMode(5, INPUT);
  pinMode(16, OUTPUT);
  pinMode(2, OUTPUT);
  digitalWrite(2, 1);
  delay(500);
  Serial.begin(115200);
  Serial.write(0XA5);
  Serial.write(0X54);
  delay(1000);
  Serial.write(0XA5);
  Serial.write(0X51);
//  resetStep();
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
    if(rec == 'S'){
      Serial.write(0XA5);
      Serial.write(0X55);
    }
    else if(rec == 'E'){
      resetStep();
    }
    else if(rec == 'R'){
      st ++;
      st %= 4;
    }
    else if(rec == 'L'){
      st --;
      if(st<0) st = 3;
      st %= 4;
    }
    else if(rec == 'G'){
      for(int i = 0; i<10; i++){
        Serial.write(0XA5);
        Serial.write(0X51);
        while (true) {
        b[c] = Serial.read();
        if(c==0 && b[0]!=0xAA) break;
        c++;
        if(c==8)
          {
            c=0;
            if(b[0]==0xAA && b[7]==0x55)
              {
                gy=(int16_t)(b[1]<<8|b[2])/100.00;
              }
          }
        }
      }
      client.print(gy);
    }
  }
  if(last_st != st) step(st);
  else {
    pinMode(12, INPUT);
    pinMode(13, INPUT);
    pinMode(14, INPUT);
    pinMode(5, INPUT);
  }
  last_st = st;
}
void step(int s){
  pinMode(12, OUTPUT);
  pinMode(13, OUTPUT);
  pinMode(14, OUTPUT);
  pinMode(5,  OUTPUT);
  if(s == 0){
    digitalWrite(12, 0);
    digitalWrite(13, 1);
    digitalWrite(14, 1);
    digitalWrite(5, 1);
    delay(speed);
  }
  if(s == 1){
    digitalWrite(12, 1);
    digitalWrite(13, 1);
    digitalWrite(14, 0);
    digitalWrite(5, 1);
    delay(speed);
  }
  if(s == 2){
    digitalWrite(12, 1);
    digitalWrite(13, 0);
    digitalWrite(14, 1);
    digitalWrite(5, 1);
    delay(speed);
  }
  if(s == 3){
    digitalWrite(12, 1);
    digitalWrite(13, 1);
    digitalWrite(14, 1);
    digitalWrite(5, 0);
    delay(speed);
  }
}

void resetStep(){
  int a = analogRead(A0);
  while(a < 100){
    a = analogRead(A0);
    st ++;
    st %= 4;
    step(st);
  }
  pinMode(12, INPUT);
  pinMode(13, INPUT);
  pinMode(14, INPUT);
  pinMode(5, INPUT);  
}
