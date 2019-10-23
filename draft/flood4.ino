#include <ESP8266WiFi.h>
#include <WiFiClient.h>

#ifndef STASSID
#define STASSID "xxx-xxx"
#define STAPSK  "xxx"
#define REPORTSERVER "192.168.xxx.xxx"
#define REPORTSERVERPORT 80
#endif

#define RTCMEMORYSTART 65
//#define RTCMEMORYLEN 127
#define RTCMEMORYLEN 2

extern "C" {
#include "user_interface.h" // this is for the RTC memory read/write functions
}

typedef struct {
  int battery;
  int other;
} rtcStore;

rtcStore rtcMem;
rtcStore rtcMemr;

int i;
int buckets;
bool toggleFlag;

const char* ssid = STASSID;
const char* password = STAPSK;
WiFiClient client;

const char* report_server = REPORTSERVER;
const int report_server_port = REPORTSERVERPORT;
const int idx = 37;
 
const int SensorPin = 12;
const unsigned sleepTime = 5 * 60;
const int drySensorState = HIGH;
const int wetWaterSensorState = LOW;
int currentSensorState = HIGH;
boolean wetDetected = false;
boolean forceSendStatus = false;

void setup(void) {

  Serial.begin(115200);
  Serial.println();
  Serial.println("Start");

  buckets = (sizeof(rtcMem) / 4);
  if (buckets == 0) buckets = 1;
  Serial.print("Buckets ");
  Serial.println(buckets);
  system_rtc_mem_read(64, &toggleFlag, 4);
  Serial.print("toggle Flag ");
  Serial.println(toggleFlag);

  if (toggleFlag) {
    Serial.println("Start Writing");
    for (i = 0; i < RTCMEMORYLEN / buckets; i++) {
      int rtcPos = RTCMEMORYSTART + i * buckets;
      system_rtc_mem_read(rtcPos, &rtcMemr, sizeof(rtcMemr));
      rtcMem.battery = i;
      //rtcMem.other = i * 11;
      if ( rtcMemr.other > 143 ) {
        rtcMem.other = 0;
        forceSendStatus = true;
      } else {
        rtcMem.other = rtcMemr.other + 1;
      }
      
      system_rtc_mem_write(rtcPos, &rtcMem, buckets * 4);
      toggleFlag = false;
      system_rtc_mem_write(64, &toggleFlag, 4);

      Serial.print("i: ");
      Serial.print(i);
      Serial.print(" Position: ");
      Serial.print(rtcPos);
      Serial.print(", battery: ");
      Serial.print(rtcMem.battery);
      Serial.print(", other: ");
      Serial.println(rtcMem.other);
      yield();
    }
    Serial.println("Writing done");
  }
  else {
    Serial.println("Start reading");
    for (i = 0; i < RTCMEMORYLEN / buckets; i++) {
      int rtcPos = RTCMEMORYSTART + i * buckets;
      system_rtc_mem_read(rtcPos, &rtcMem, sizeof(rtcMem));
      toggleFlag = true;
      system_rtc_mem_write(64, &toggleFlag, 4);

      Serial.print("i: ");
      Serial.print(i);
      Serial.print(" Position ");
      Serial.print(rtcPos);
      Serial.print(", battery: ");
      Serial.print(rtcMem.battery);
      Serial.print(", other: ");
      Serial.println(rtcMem.other);
      yield();
    }
    Serial.println("reading done");
    /*
    for (i = 0; i < RTCMEMORYLEN / buckets; i++) {
      rtcMem.battery = 0;
      rtcMem.other = 0;
      int rtcPos = RTCMEMORYSTART + i * buckets;
      system_rtc_mem_write(rtcPos, &rtcMem, buckets * 4);
    }
    */
  }

  pinMode(SensorPin, INPUT);
  Serial.print("Current Sensor State ");
  
  while ( digitalRead(SensorPin) == wetWaterSensorState ) {
    wetDetected = true;
    Serial.println(wetWaterSensorState);
    if ( wifiConnect() ) {
      sendSensorState(wetWaterSensorState);
    }
  }
  if ( wetDetected || forceSendStatus ) {
      if ( wifiConnect() ) {
      sendSensorState(drySensorState);
    } 
  }
  // dryWaterSensorState
  WiFi.disconnect();    
  Serial.println("So far, so good...");
  Serial.println("before sleep");
  ESP.deepSleep(sleepTime * 1000000, WAKE_RFCAL);
}

boolean wifiConnect(void) {

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.println("");
  int attempt = 0;
  int max_attempt = 50;
  
  while (attempt < max_attempt && WiFi.status() != WL_CONNECTED) {
    delay(500);
    attempt++;
    Serial.print(".");
  }
  
  if ( attempt == max_attempt ) {
    Serial.println("");
    Serial.print("Failed to connected to ");
    Serial.println(ssid);
    return false;
  }
  
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  return true;
}

void sendSensorState(int state) {
  
  if (client.connect(report_server,report_server_port)) {
    Serial.println("Sending Values to server...:");
    client.print("GET /json.htm?type=command&param=udevice&idx=");
    client.print(idx);
    client.print("&nvalue=");
    client.print(state);
    client.println(" HTTP/1.1");
    client.print("Host: ");
    client.print(report_server);
    client.print(":");
    client.println(report_server_port);
    client.println("User-Agent: Arduino-ethernet");
    client.println("Connection: close");
    client.println();
    client.stop();
    Serial.println(state);
  } else {
    Serial.println("Can not connect to server...:");
    Serial.println(report_server);
  }
}

void loop(void) {}
