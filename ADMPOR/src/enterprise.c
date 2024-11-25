/* 

Sistemas Operativos - Projeto Fase 1 
Grupo 10
Eduardo Proença  - 57551
Tiago Oliveira - 54979

*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "synchronization.h"
#include "aptime.h"
#include "enterprise.h"

int execute_enterprise(int enterp_id, struct comm_buffers* buffers, struct main_data* data, struct semaphores* sems) {
    int counter = 0;
    while (1) {
        if ((*data->terminate)) // If the terminate flag is set, return the number of operations processed
            return counter;

        struct operation op;
        enterprise_receive_operation(&op, enterp_id, buffers, data, sems);

        if (op.id >= 0) { // Only process the operation if it is valid (!= -1)
            consume_begin(sems->interm_enterp);
            printf("Empresa recebeu pedido!\n");
            enterprise_process_operation(&op, enterp_id, data, &counter, sems);
            consume_end(sems->interm_enterp);
        }
    }
}

void enterprise_receive_operation(struct operation* op, int enterp_id, struct comm_buffers* buffers, struct main_data* data, struct semaphores* sems) {
    if (!(*data->terminate)) {
        semaphore_mutex_lock(sems->interm_enterp->mutex);
        read_interm_enterp_buffer(buffers->interm_enterp, enterp_id, data->buffers_size, op);
        semaphore_mutex_unlock(sems->interm_enterp->mutex);
    }
    return;
}

void enterprise_process_operation(struct operation* op, int enterp_id, struct main_data* data, int* counter, struct semaphores* sems) {
    semaphore_mutex_lock(sems->results_mutex);
    op->receiving_enterp = enterp_id;
    if (op->id < data->max_ops) {
        op->status = 'E'; // Set status to "executed by enterprise" if the max number of operations has not been reached
    } else {
        op->status = 'A'; // Set status to "scheduled by enterprise" if the max number of operations has been reached
    }
    (*counter)++;
    register_enterp_time(op);
    int idx = op->id % MAX_RESULTS;
    data->results[idx] = *op;
    semaphore_mutex_unlock(sems->results_mutex);
}