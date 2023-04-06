#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "client.h"

int execute_client(int client_id, struct comm_buffers *buffers, struct main_data *data) {
    while (true) {
        int counter = 0;
        // TODO
        if (!(data->terminate)) {
            // TODO
            counter++;
        }
        else {
            return counter;
        }
    }
}

void client_get_operation(struct operation *op, int client_id, struct comm_buffers *buffers, struct main_data *data) {
    if(!(data->terminate)){
        read_main_client_buffer(buffers, client_id, data->buffers_size, op);
    }
    
    return;
}

void client_process_operation(struct operation *op, int client_id, struct main_data *data, int *counter) {
    // TODO
}

void client_send_operation(struct operation *op, struct comm_buffers *buffers, struct main_data *data) {
    // TODO
}