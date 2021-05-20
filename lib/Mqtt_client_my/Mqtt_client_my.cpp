#include <Arduino.h>
#include "PubSubClient.h"
#include "WiFiClient.h"
#include "Mqtt_client_my.h"
// #include "WiFiClientSecure.h"

#define mqtt_server "192.168.1.88"

PubSubClient mqtt_client_my_init(){
  bool sub_topic_test = false;
  bool pub_topic_test = false;

  mqtt_client.setServer(mqtt_server, 1883);
  while(!mqtt_client.connected()){
    Serial.println("Not connected");
    vTaskDelay(1000);
  }

  mqtt_client.setCallback(callback);
  Serial.println("subscribing to topic test");
  while (!sub_topic_test){
    sub_topic_test = mqtt_client.subscribe("/topic/test",1);
    Serial.print("here");
    vTaskDelay(1000);
  }
  Serial.println("Subscribed");
  while (!pub_topic_test)
  {
    pub_topic_test = mqtt_client.publish("/topic/test", "test");
  }
  return mqtt_client;
}

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);   // 打印主题信息
  Serial.print("] ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]); // 打印主题内容
  }
  Serial.println();
}

void reconnect() {
  // Loop until we're reconnected
  while (!mqtt_client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (mqtt_client.connect("ESP8266Client")) {
      Serial.println("connected");
      // Subscribe
      mqtt_client.subscribe("esp32/output");
    } else {
      Serial.print("failed, rc=");
      Serial.print(mqtt_client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}