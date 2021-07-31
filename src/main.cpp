#include <Arduino.h>
#include <LED_Bar.h>
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include <ESP8266mDNS.h>
#include <OSCMessage.h>
#include <ArduinoOTA.h>
#include <ssid.h>

#define RESOLUTION 8191
#define NUM_LEDS 60

const char* ssid = SSID;
const char* password = PASSWORD;
const int udpPort = PORT;

const int packetSize = 255;
char packetBuffer[packetSize];

char *strings[10];
char *ptr = NULL;

WiFiUDP Udp;
LED_Bar bar(D3, D2, 1, NUM_LEDS, LED_NEW_DEVICE); // Clock pin, Data pin, Orientation

void reset() {
  for(int j = 1; j <= NUM_LEDS; j++) bar.setLed(j, 0,0,0);
}

void startWiFiUdp() {
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println();
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  Serial.println("Starting UDP");
  Udp.begin(PORT);
}


void effect() {
  for(int j = 1; j <= NUM_LEDS; j++) {
    for(int i = 0; i <= 16; i++) bar.setLed(j, i,i,i), bar.show(), delay(1); //For runnin from 0 to 16
  }
  for(int j = 1; j <= NUM_LEDS; j++) {
    for(int i = 16; i >= 0; i--) bar.setLed(j, i,i,i), bar.show(), delay(1);
  }
}

void processLED (OSCMessage &msg) {
  //unsigned long int t_i = millis();
  float rgb[3];
  for(int a = 0; a < msg.size(); a = a+3) {
    for (int i = 0; i < 3; i++){
      if(msg.isFloat(a+i)) rgb[i] = msg.getFloat(a+i)*RESOLUTION;
      else if(msg.isInt(a+i)) rgb[i] = float(msg.getInt(a+i)==1)*RESOLUTION;
      else rgb[i] = 0;
    }
    //Serial.println(String(a/3) + " " + String(rgb[0]) + " " + String(rgb[1]) + " " + String(rgb[2]));
    bar.setLed((a/3)+1, rgb[0], rgb[1], rgb[2]);
  }
  bar.show();
  //Serial.println("Tempo: " + String(millis() - t_i));
}

void receiveMsg(){
  OSCMessage msg;
  int size = Udp.parsePacket();
  if (size > 0) {
    while (size--) msg.fill(Udp.read());
    if (!msg.hasError()) msg.dispatch("/LEDS", processLED);
    else Serial.println("Error in msg");
  }
}


void setup() {
    // nothing to initialize
    Serial.begin(115200);
    startWiFiUdp();
    ArduinoOTA.begin();
    bar.begin();
    reset();
    Serial.println("Done");
}

void loop() {
  ArduinoOTA.handle();
  receiveMsg();
}
