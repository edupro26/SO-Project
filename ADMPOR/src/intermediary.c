#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../include/intermediary.h"

int execute_intermediary(int interm_id, struct comm_buffers* buffers, struct main_data* data) {
    //TODO
}

void intermediary_receive_operation(struct operation* op, struct comm_buffers* buffers, struct main_data* data) {
    //TODO
}

void intermediary_process_operation(struct operation* op, int interm_id, struct main_data* data, int* counter) {
    //TODO
}

void intermediary_send_answer(struct operation* op, struct comm_buffers* buffers, struct main_data* data) {
    //TODO
}