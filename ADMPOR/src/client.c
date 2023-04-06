#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "client.h"

int execute_client(int client_id, struct comm_buffers *buffers, struct main_data *data) {
    int counter = 0;
    while (1) {
        struct operation* op;
        client_get_operation(op, client_id, buffers, data);
        if (!(data->terminate) && op->id != -1) {
            client_process_operation(op, client_id, data, &counter);
            // TODO
        }
        else {
            return counter;
        }
    }
}

void client_get_operation(struct operation *op, int client_id, struct comm_buffers *buffers, struct main_data *data) {
    if(!(data->terminate)){
        read_main_client_buffer(buffers->main_client, client_id, data->buffers_size, op);
    }
    
    return;
}

void client_process_operation(struct operation *op, int client_id, struct main_data *data, int *counter) {
    op->receiving_client = client_id;
    op->status = 'C';
    (*counter)++;
    // TODO data operations
}

void client_send_operation(struct operation *op, struct comm_buffers *buffers, struct main_data *data) {
    // TODO
}