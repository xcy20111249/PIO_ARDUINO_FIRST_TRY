#ifndef COMMUTILS_H_
#define COMMUTILS_H_

void Task_init_bluetooth();
void TaskCreate_bluetooth();
bool mqtt_restart();
bool mqtt_terminate();
bool bluetooth_restart();
bool bluetooth_terminate();

#endif