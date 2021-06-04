#ifndef COMMUTILS_H_
#define COMMUTILS_H_

void bluetooth_print_state();
void Task_init_bluetooth();
void TaskCreate_bluetooth();
bool bluetooth_restart();
bool bluetooth_terminate();
void nvs_comm_test();

#endif