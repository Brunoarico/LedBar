#include <Arduino.h>
#include <LED_Bar.h>
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include <ESP8266mDNS.h>
#include <OSCMessage.h>
#include <ArduinoOTA.h>
#include <ssid.h>

#define RESOLUTION 16
#define NUM_LEDS 60

const char* ssid     = SSID;
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

/*void processLED (OSCMessage &msg) {
  for(int a = 0, b = 0; a < NUM_LEDS*3; a=a+3, b++)
    if (msg.isInt(a)||msg.isInt(a+1)||msg.isInt(a+2)) bar.setLed(b, msg.getInt(a),msg.getInt(a+1),msg.getInt(a+2));
}*/

void processLED (OSCMessage &msg) {


  if(msg.size() < NUM_LEDS*3) {
    int part = (NUM_LEDS*3) / (msg.size()/3);
    int msg_count = 0;
    int segment = part;
    Serial.println(part);
    Serial.println(msg.size()/3);
    for(int i = 0; i < NUM_LEDS*3; i = i+3) {

      Serial.println(String((i/3)+1) + " " + String(msg.getFloat(msg_count)) + " " + String(msg.getFloat(msg_count+1)) + " " + String(msg.getFloat(msg_count+2)));
      bar.setLed((i/3)+1, msg.getFloat(msg_count)*RESOLUTION, msg.getFloat(msg_count+1)*RESOLUTION, msg.getFloat(msg_count+2)*RESOLUTION);

      if(i >= segment) segment += part, msg_count+=3;
    }
  }
  else {
    for(int a = 0; a < msg.size(); a = a+3) {
      if(msg.isFloat(a) || msg.isFloat(a+1) || msg.isFloat(a+2)) {
          Serial.println(String(a/3) + " " + String(msg.getFloat(a)) + " " + String(msg.getFloat(a+1)) + " " + String(msg.getFloat(a+2)));
          bar.setLed((a/3)+1, msg.getFloat(a)*RESOLUTION, msg.getFloat(a+1)*RESOLUTION, msg.getFloat(a+2)*RESOLUTION);
      }
    }
  }
  bar.show();
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
