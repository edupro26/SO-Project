#ifndef APSIGNAL_H_GUARD
#define APSIGNAL_H_GUARD

#include "main.h"

void sigalrm_handler(int sig_num);

void print_alarm(int* op_counter);

void sigint_handler(int sig_num);

void ctrlC(struct main_data* data, struct comm_buffers* buffers, struct semaphores* sems);

#endif
