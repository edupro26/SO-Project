/* 

Sistemas Operativos - Projeto Fase 1 
Grupo 10
Diogo Peixoto - 53365
Eduardo Proença  - 57551
Tiago Oliveira - 54979

*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "synchronization.h"
#include "client.h"

int execute_client(int client_id, struct comm_buffers* buffers, struct main_data* data, struct semaphores* sems) {  
    int counter = 0;
    while (1) {
        if((*data->terminate)) // If the terminate flag is set, return the number of operations processed
            return counter;

        struct operation op;
        struct operation *p_op = &op;
        client_get_operation(p_op, client_id, buffers, data, sems);
        usleep(5000);

        if(p_op->id >= 0){ // Only process the operation if it is valid (!= -1)
            client_process_operation(p_op, client_id, data, &counter, sems);
            client_send_operation(p_op, buffers, data, sems);
            printf("Cliente recebeu pedido!\n");
        }
    }
}

void client_get_operation(struct operation* op, int client_id, struct comm_buffers* buffers, struct main_data* data, struct semaphores* sems) {
    if(!(*data->terminate)){
        read_main_client_buffer(buffers->main_client, client_id, data->buffers_size, op);
    }
    
    return;
}

void client_process_operation(struct operation* op, int client_id, struct main_data* data, int* counter, struct semaphores* sems) {
    op->receiving_client = client_id;
    op->status = 'C'; // Set the status to "processed by client"
    (*counter)++; // Increment the operations counter
    
    // Update the operation in the results array
    int idx = op->id % MAX_RESULTS;
    data->results[idx] = *op;
}

void client_send_operation(struct operation* op, struct comm_buffers* buffers, struct main_data* data, struct semaphores* sems) {
    write_client_interm_buffer(buffers->client_interm, data->buffers_size, op);
}