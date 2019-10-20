#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>

#ifndef STASSID
#define STASSID "your-ssid"
#define STAPSK  "your-password"
#define REPORTSERVER "your-server"
#define REPORTSERVERPORT 80
#endif

const char* ssid = STASSID;
const char* password = STAPSK;
WiFiClient client;

const char* report_server = REPORTSERVER;
const int report_server_port = REPORTSERVERPORT;
const int idx = 37;
 
const int SensorPin = 2;
const unsigned sleepTime = 1 * 60;

int LastWaterSensorState = HIGH;
int CurrentWaterSensorState = HIGH;

boolean debounce1(boolean last)
{
  boolean current = digitalRead(SensorPin);
  if (last != current)
  {
    delay(5);
    current = digitalRead(SensorPin);
  }
  return current;
}

void setup(void) {

  pinMode(SensorPin, INPUT);
  WiFi.mode(WIFI_STA);
  Serial.begin(115200);
}

boolean wifiConnect(void) {
  
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
  }
}

void loop(void) {
  CurrentWaterSensorState = debounce1(LastWaterSensorState);
  if ( wifiConnect() ) {
    sendSensorState(CurrentWaterSensorState);
    WiFi.disconnect();
  } 
  
  if (LastWaterSensorState != CurrentWaterSensorState) {
    Serial.print(" Sensor State changed: ");
    Serial.println(CurrentWaterSensorState);
    LastWaterSensorState = CurrentWaterSensorState;
  } else {
    Serial.print(" Sensor State NOT changed: ");
    Serial.println(CurrentWaterSensorState);
  }
  
  Serial.println("ESP8266 in sleep mode");
  ESP.deepSleep(sleepTime * 1000000);
}
