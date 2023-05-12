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
#include "intermediary.h"

int execute_intermediary(int interm_id, struct comm_buffers* buffers, struct main_data* data, struct semaphores* sems) {
    int counter = 0;
    while (1) {
        if ((*data->terminate)) // If the terminate flag is set, return the number of operations processed
            return counter;

        struct operation op;
        struct operation* p_op = &op;
        
        intermediary_receive_operation(p_op, buffers, data, sems);

        if (p_op->id >= 0) { // Only process the operation if it is valid (!= -1)
            printf("Intermediário recebeu pedido!\n");
            intermediary_process_operation(p_op, interm_id, data, &counter, sems);
            intermediary_send_answer(p_op, buffers, data, sems);
        }
    }
}

void intermediary_receive_operation(struct operation* op, struct comm_buffers* buffers, struct main_data* data, struct semaphores* sems) {
    if(!(*data->terminate)){
        consume_begin(sems->client_interm);
        read_client_interm_buffer(buffers->client_interm, data->buffers_size, op);
        consume_end(sems->client_interm);
    }
    return;
}

void intermediary_process_operation(struct operation* op, int interm_id, struct main_data* data, int* counter, struct semaphores* sems) {
    semaphore_mutex_lock(sems->results_mutex);
    op->receiving_interm = interm_id;
    op->status = 'I'; // Set the status to "processed by intermediary"
    (*counter)++; // Increment the operations counter

    // Update the operation in the results array
    int idx = op->id % MAX_RESULTS;
    data->results[idx] = *op;
    semaphore_mutex_unlock(sems->results_mutex);
}

void intermediary_send_answer(struct operation* op, struct comm_buffers* buffers, struct main_data* data, struct semaphores* sems) {
    produce_begin(sems->interm_enterp);
    write_interm_enterp_buffer(buffers->interm_enterp, data->buffers_size, op);
}