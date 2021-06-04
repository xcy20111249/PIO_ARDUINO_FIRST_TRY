#ifndef MQTT_FUNC_H_
#define MQTT_FUNC_H_

void mqtt_set_server(char* add, int port);
void Task_init_mqtt();
void TaskCreate_mqtt();
bool mqtt_restart();
bool mqtt_terminate();


#endif