#include <ESP8266WiFi.h>
#include <PubSubClient.h>

int pirPin = D7;
int pirState;
int pirStatus = 0;

#define MQTT_TOPIC_PIR "orange/pir/status"
#define MQTT_TOPIC_STATE "orange/pir/state"
#define MQTT_PUBLISH_DELAY 60000
#define MQTT_CLIENT_ID "esp8266pir001"

const char *WIFI_SSID = "xxx";
const char *WIFI_PASSWORD = "xxx";

const char *MQTT_SERVER = "192.168.16.38";
const char *MQTT_USER = "xxx"; // NULL for no authentication
const char *MQTT_PASSWORD = "xxx"; // NULL for no authentication

long lastMsgTime = 0;

WiFiClient espClient;
PubSubClient mqttClient(espClient);

void setup() {
  Serial.begin(115200);
  delay(10);

  // Connect to WiFi network
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(WIFI_SSID);
 
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
 
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
 
  // Print the IP address
  Serial.print("IP ");
  Serial.println(WiFi.localIP());

  mqttClient.setServer(MQTT_SERVER, 1883);
 
}

void loop() {

  if (!mqttClient.connected()) {
    mqttReconnect();
  }

  mqttClient.loop();
  delay(2000);
  long now = millis();

  // Reading PIR sensor
  pirState = digitalRead(pirPin);
  pirStatus += ( pirState == LOW ) ? 0 : 1;

  Serial.print("PIR status: ");
  Serial.println(pirStatus);
  Serial.println(pirState);
  
  if (now - lastMsgTime > MQTT_PUBLISH_DELAY) {
    lastMsgTime = now;

    Serial.print("PIR status: ");
    Serial.println(pirStatus);
   
    // Publishing sensor status
    mqttPublish(MQTT_TOPIC_PIR, pirStatus);
    pirStatus = 0;
  }
  
}

void mqttReconnect() {
  while (!mqttClient.connected()) {
    Serial.print("Attempting MQTT connection...");

    // Attempt to connect
    if (mqttClient.connect(MQTT_CLIENT_ID, MQTT_USER, MQTT_PASSWORD, MQTT_TOPIC_STATE, 1, true, "disconnected", false)) {
      Serial.println("connected");

      // Once connected, publish an announcement...
      mqttClient.publish(MQTT_TOPIC_STATE, "connected", true);
    } else {
      Serial.print("failed, rc=");
      Serial.print(mqttClient.state());
      Serial.println(" try again in 5 seconds");
      delay(5000);
    }
  }
}

void mqttPublish(char *topic, float payload) {
  Serial.print(topic);
  Serial.print(": ");
  Serial.println(payload);

  mqttClient.publish(topic, String(payload).c_str(), true);
}
