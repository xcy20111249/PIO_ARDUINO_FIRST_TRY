#include <Arduino.h>
#include "PubSubClient.h"
#include "WiFiClient.h"
#include "WiFi.h"

#include "config_test.h"
#include "Mqtt_client_my.h"

static bool running_flag;

void mqtt_client_my_init(){
  mqtt_client_my.setServer(mqtt_server, 1883);
  mqtt_client_my.setCallback(callback);
  topicSubscribe_all = "imeds/database/#";
  topicSubscribe_User = "imeds/database/" + Distributeur_id + "/user/#";
  topicSubscribe_Product = "imeds/database/" + Distributeur_id + "/product/#";
  topicSubscribe_Hw = "imeds/database/" + Distributeur_id + "/hw/#";
  topicSubscribe_App = "imeds/database/" + Distributeur_id + "/app/#";
  topicSubscribe_Mtime = "imeds/database/" + Distributeur_id + "/mtime/#";

  Serial.println("mqtt client initilized.");
}

// this function is called every time connecting to mqtt server
static void on_connect_subscribe(){
  Serial.printf("[%s] Subscribing to %s\n", TAG, topicSubscribe_User.c_str());
  while (!mqtt_client_my.subscribe(topicSubscribe_User.c_str(),1)){
    Serial.print(".");
    delay(500);
  }
  Serial.printf("[%s] Subscribing to %s\n", TAG, topicSubscribe_Product.c_str());
  while (!mqtt_client_my.subscribe(topicSubscribe_Product.c_str(),1)){
    Serial.print(".");
    delay(500);
  }
  Serial.printf("[%s] Subscribing to %s\n", TAG, topicSubscribe_Hw.c_str());
  while (!mqtt_client_my.subscribe(topicSubscribe_Hw.c_str(),1)){
    Serial.print(".");
    delay(500);
  }
  Serial.printf("[%s] Subscribing to %s\n", TAG, topicSubscribe_App.c_str());
  while (!mqtt_client_my.subscribe(topicSubscribe_App.c_str(),1)){
    Serial.print(".");
    delay(500);
  }
  Serial.printf("[%s] Subscribing to %s\n", TAG, topicSubscribe_Mtime.c_str());
  while (!mqtt_client_my.subscribe(topicSubscribe_Mtime.c_str(),1)){
    Serial.print(".");
    delay(500);
  }

}

static void callback(char* topic, byte* payload, unsigned int length) {
  String messageTemp;
  Serial.print("Message arrived [");
  Serial.print(topic);   // 打印主题信息
  Serial.print("] ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]); // 打印主题内容
    messageTemp += (char)payload[i];
  }
  Serial.println();
}

static void reconnect() {
  // Loop until we're reconnected
  while (!mqtt_client_my.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (mqtt_client_my.connect("ESP32Client")) {
      Serial.println("connected");
      // Subscribe
      Serial.printf("[%s] Subscribing to /topic/test\n", TAG);
      while (!mqtt_client_my.subscribe("/topic/test",1)){
        Serial.print(".");
        delay(200);
      }
      Serial.printf("[%s] Subscribing to esp32/temperature\n", TAG);
      while (!mqtt_client_my.subscribe("esp32/temperature",1))
      {
        Serial.print(".");
        delay(200);
      }
      Serial.printf("[%s] Subscribing to esp32/humidity\n", TAG);
      while (!mqtt_client_my.subscribe("esp32/humidity",1))
      {
        Serial.print(".");
        delay(200);
      }
      on_connect_subscribe();
      Serial.println("Subscription done.");
    } else {
      Serial.print("failed, rc=");
      Serial.print(mqtt_client_my.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

void mqtt_stop(){
  running_flag = false;
}

void test(void * arg){  
  long lastMsg = 0;
  int temperature;
  int humidity;
  mqtt_client_my.setCallback(callback);
  running_flag = true;
  Serial.println("starting test task...");
  
  
  // while (!mqtt_client_my.connected()) {
  reconnect();
  // }
  while (running_flag)
  {
    mqtt_client_my.loop();
    long now = millis();
    if (now - lastMsg > 5000) {
      lastMsg = now;
      mqtt_client_my.publish("/topic/test", "living...");
      delay(100);
      /*
      // Temperature in Celsius
      temperature = random(30);   
      // Uncomment the next line to set temperature in Fahrenheit 
      // (and comment the previous temperature line)
      //temperature = 1.8 * bme.readTemperature() + 32; // Temperature in Fahrenheit
      
      // Convert the value to a char array
      char tempString[8];
      dtostrf(temperature, 1, 2, tempString);
      Serial.print("Temperature: ");
      Serial.println(tempString);
      mqtt_client_my.publish("esp32/temperature", tempString);

      humidity = random(80);
      
      // Convert the value to a char array
      char humString[8];
      dtostrf(humidity, 1, 2, humString);
      Serial.print("Humidity: ");
      Serial.println(humString);
      mqtt_client_my.publish("esp32/humidity", humString);
      */
    }
    delay(500);
    Serial.print(".");
  }
  Serial.println("ending mqtt task.");
  xSemaphoreGive(task_semaphore_mqtt);
  vTaskDelete(NULL);
}
