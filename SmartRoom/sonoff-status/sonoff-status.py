import time
import paho.mqtt.client as paho
broker="broker.hivemq.com"
broker="iot.eclipse.org"
broker="192.168.1.157"
#define callback
def on_message(client, userdata, message):
   rm=str(message.payload.decode("utf-8"))
   print("topic ",message.topic, "  message =",str(message.payload.decode("utf-8")))


client= paho.Client("client-001")  #create client object client1.on_publish = on_publish                          #assign function to callback client1.connect(broker,port)                                 #establish connection client1.publish("house/bulb1","on")  
######
client.on_message=on_message
#####
print("connecting to broker ",broker)
client.connect(broker)#connect
client.loop_start() #start loop to process received messages
print("subscribing ")
client.subscribe("sonoff-light/#")#subscribe
time.sleep(2)
print("publishing ")
pub_base="sonoff-light/cmnd/"
commands=["ButtonRetain","ButtonTopic","FullTopic","GroupTopic",\
          "MQTTclient","MqttFingerprint","MQTTHost","MQTTPassword","MQTTUser"]
topic=topic=pub_base+"MqttFingerprint"
client.publish(topic,"04 A7 19 79 A2 B4 0A 30 5B 79 BD D6 7E E4 48 FC \
28 D4 D1 E9")#publish
for c in commands:
   topic=pub_base+c
   client.publish(topic,"")#publish
   time.sleep(2)

client.disconnect() #disconnect
client.loop_stop() #stop loop
