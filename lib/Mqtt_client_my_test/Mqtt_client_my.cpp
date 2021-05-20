#include <Arduino.h>
#include "PubSubClient.h"
#include "WiFiClient.h"
#include "Mqtt_client_my.h"
#include "WiFi.h"
// #include "WiFiClientSecure.h"
/*
IPAddress mqtt_server (192, 168, 1, 88);
PubSubClient mqtt_client_my;
PubSubClient mqtt_client_my_init(){
  bool sub_topic_test = false;
  bool pub_topic_test = false;
  mqtt_client_my.setServer(mqtt_server, 1883);
  // while(!mqtt_client_my.connected()){
  //   Serial.println("Not connected");
  //   vTaskDelay(1000);
  // }

  mqtt_client_my.setCallback(callback);
  Serial.println("subscribing to topic test");
  while (!sub_topic_test){
    sub_topic_test = mqtt_client_my.subscribe("/topic/test",1);
    Serial.print("here");
    vTaskDelay(1000);
  }
  Serial.println("Subscribed");
  while (!pub_topic_test)
  {
    pub_topic_test = mqtt_client_my.publish("/topic/test", "test");
  }
  return mqtt_client_my;
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
  while (!mqtt_client_my.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (mqtt_client_my.connect("ESP8266Client")) {
      Serial.println("connected");
      // Subscribe
      mqtt_client_my.subscribe("esp32/output");
    } else {
      Serial.print("failed, rc=");
      Serial.print(mqtt_client_my.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}
*/

const char* ssid = "BEENETIC_FIBRE";
const char* password = "CairZigWatt2010";


void callback(char* topic, uint8_t* message, unsigned int length) {
  Serial.print("Message arrived on topic: ");
  Serial.print(topic);
  Serial.print(". Message: ");
  String messageTemp;
  
  for (int i = 0; i < length; i++) {
    Serial.print((char)message[i]);
    messageTemp += (char)message[i];
  }
  Serial.println();

  // Feel free to add more if statements to control more GPIOs with MQTT

  // If a message is received on the topic esp32/output, you check if the message is either "on" or "off". 
  // Changes the output state according to the message
  if (String(topic) == "esp32/output") {
    Serial.print("Changing output to ");
    if(messageTemp == "on"){
      Serial.println("on");
      
    }
    else if(messageTemp == "off"){
      Serial.println("off");
      
    }
  }
}
void setup_wifi() {
  delay(10);
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}
void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (client.connect("ESP8266Client")) {
      Serial.println("connected");
      // Subscribe
      client.subscribe("esp32/output");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}
/*
void test(void *client){
  PubSubClient* local_client = (PubSubClient*)client;
  if (!local_client->connected()) {
    reconnect();
  }
  local_client->loop();
  vTaskDelete(NULL);
}
*/
void test(void * arg){  
  long lastMsg = 0;
  char msg[50];
  int value = 0;
  int temperature;
  int humidity;
  client.setCallback(callback);
  
  while (!client.connected()) {
    reconnect();
  }
  Serial.println("subscribing to esp32/temperature");
  while (!client.subscribe("esp32/temperature",1))
  {
    Serial.print(".");
    delay(2000);
  }
  Serial.println("subscribed to esp32/temperature");
  Serial.println("subscribing to esp32/humidity");
  while (!client.subscribe("esp32/humidity",1))
  {
    Serial.print(".");
    delay(2000);
  }
  Serial.println("subscribed to esp32/humidity");

  while (1)
  {
    client.loop();
    long now = millis();
    if (now - lastMsg > 5000) {
      lastMsg = now;
      
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
      client.publish("esp32/temperature", tempString);

      humidity = random(80);
      
      // Convert the value to a char array
      char humString[8];
      dtostrf(humidity, 1, 2, humString);
      Serial.print("Humidity: ");
      Serial.println(humString);
      client.publish("esp32/humidity", humString);
    }
  }
  
  //vTaskDelete(NULL);
}