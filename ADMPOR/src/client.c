#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "client.h"

int execute_client(int client_id, struct comm_buffers *buffers, struct main_data *data) {  
    int counter = 0;
    while (1) {
        struct operation op;
        struct operation *p_op = &op;
        client_get_operation(p_op, client_id, buffers, data);

        if((*data->terminate))
            return counter;

        if(p_op->id >= 0){
            client_process_operation(p_op, client_id, data, &counter);
            client_send_operation(p_op, buffers, data);
        }
    }
}

void client_get_operation(struct operation *op, int client_id, struct comm_buffers *buffers, struct main_data *data) {
    if(!(*data->terminate)){
        read_main_client_buffer(buffers->main_client, client_id, data->buffers_size, op);
    }
    
    return;
}

void client_process_operation(struct operation *op, int client_id, struct main_data *data, int *counter) {
    op->receiving_client = client_id;
    op->status = 'C'; // Set the status to "processed by client"
    (*counter)++; // Increment the operations counter
    
    // Update the operation in the results array
    int idx = op->id % MAX_RESULTS;
    data->results[idx] = *op;

    // Increment the number of operations processed by this client
    data->client_stats[client_id]++;
}

void client_send_operation(struct operation *op, struct comm_buffers *buffers, struct main_data *data) {
    write_client_interm_buffer(buffers->client_interm, data->buffers_size, op);
}