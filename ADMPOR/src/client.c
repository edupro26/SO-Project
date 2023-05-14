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
#include "aptime.h"
#include "client.h"

int execute_client(int client_id, struct comm_buffers* buffers, struct main_data* data, struct semaphores* sems) {  
    int counter = 0;
    while (1) {
        if((*data->terminate)) // If the terminate flag is set, return the number of operations processed
            return counter;

        struct operation op;
        produce_end(sems->main_client);
        client_get_operation(&op, client_id, buffers, data, sems);

        if(op.id >= 0){ // Only process the operation if it is valid (!= -1)
            register_client_time(&op);
            printf("Cliente recebeu pedido!\n");
            client_process_operation(&op, client_id, data, &counter, sems);
            client_send_operation(&op, buffers, data, sems);
        }
    }
}

void client_get_operation(struct operation* op, int client_id, struct comm_buffers* buffers, struct main_data* data, struct semaphores* sems) {
    if(!(*data->terminate)){
        consume_begin(sems->main_client);
        read_main_client_buffer(buffers->main_client, client_id, data->buffers_size, op);
        consume_end(sems->main_client);
    }
    return;
}

void client_process_operation(struct operation* op, int client_id, struct main_data* data, int* counter, struct semaphores* sems) {
    semaphore_mutex_lock(sems->results_mutex);
    op->receiving_client = client_id;
    op->status = 'C'; // Set the status to "processed by client"
    (*counter)++; // Increment the operations counter
    
    // Update the operation in the results array
    int idx = op->id % MAX_RESULTS;
    data->results[idx] = *op;
    semaphore_mutex_unlock(sems->results_mutex);
}

void client_send_operation(struct operation* op, struct comm_buffers* buffers, struct main_data* data, struct semaphores* sems) {
    produce_begin(sems->client_interm);
    write_client_interm_buffer(buffers->client_interm, data->buffers_size, op);
    produce_end(sems->client_interm);
}