#include <Arduino.h>
#include <LED_Bar.h>
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include <ssid.h>

#define RESOLUTION 16

const char* ssid     = SSID;
const char* password = PASSWORD;
const int udpPort = 2807;

const int packetSize = 255;
char packetBuffer[packetSize];

char *strings[10];
char *ptr = NULL;

WiFiUDP udp;
LED_Bar bar(D3, D2, 1, LED_NEW_DEVICE); // Clock pin, Data pin, Orientation

void reset() {
  for(int j = 1; j <= 30; j++) bar.setLed(j, 0,0,0);
}

void setup() {
    // nothing to initialize
    Serial.begin(115200);
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
      delay(100);
      Serial.print(".");
    }
    Serial.println(WiFi.localIP());
    udp.begin(udpPort);
    bar.begin();
    reset();
}

void effect() {
  for(int j = 1; j <= 30; j++) {
    for(int i = 0; i <= 16; i++) bar.setLed(j, i,i,i), delay(1); //For runnin from 0 to 16
  }
  for(int j = 1; j <= 30; j++) {
    for(int i = 16; i >= 0; i--) bar.setLed(j, 0,0,i), delay(1);
  }
  for(int j = 1; j <= 30; j++) {
    for(int i = 0; i <= 16; i++) bar.setLed(j, 0,i,0), delay(1);
  }
  for(int j = 1; j <= 30; j++) {
    for(int i = 16; i >= 0; i--) bar.setLed(j, 0,i,0), delay(1);
  }
  for(int j = 1; j <= 30; j++) {
    for(int i = 0; i <= 16; i++) bar.setLed(j, i,0,0), delay(1);
  }
  for(int j = 1; j <= 30; j++) {
    for(int i = 16; i >= 0; i--) bar.setLed(j, i,0,0), delay(1);
  }
}

void interp(char* msg) {
  byte index = 0;
  ptr = strtok(msg, " ");  // takes a list of delimiters
  while(ptr != NULL) {
        strings[index] = ptr;
        index++;
        ptr = strtok(NULL, " ");  // takes a list of delimiters
  }
  int led_num = String(strings[0]).toInt();
  int r = String(strings[1]).toInt();
  int g = String(strings[2]).toInt();
  int b = String(strings[3]).toInt();
  if(led_num >= 1 && led_num <= 30) bar.setLed(led_num, r,g,b);
  if(led_num == 255) effect();
}

void handleUDPServer() {
  int size = udp.parsePacket();
  if (size) {
    udp.read(packetBuffer, packetSize);
    int len = udp.read(packetBuffer, 255);
    if (len > 0) packetBuffer[len] = '\0';
    Serial.println(packetBuffer);
    interp(packetBuffer);
  }
}

void loop() {
    handleUDPServer();
}
