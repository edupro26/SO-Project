#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../include/process.h"
#include "../include/main.h"

void main_args(int argc, char* argv[], struct main_data* data){
    int temp[5];

    for (int i = 1; i < argc; i++){
        temp[i-1] = atoi(argv[i]);
    }

    data->max_ops = temp[0];
    data->buffers_size = temp[1];
    data->n_clients = temp[2];
    data->n_intermediaries= temp[3];
    data->n_enterprises = temp[4];
}

void create_dynamic_memory_buffers(struct main_data* data) {
    data->client_pids = create_dynamic_memory(data->n_clients * sizeof(int));
    data->intermediary_pids = create_dynamic_memory(data->n_intermediaries * sizeof(int));
    data->enterprise_pids = create_dynamic_memory(data->n_enterprises * sizeof(int));
    
    data->client_stats = create_dynamic_memory(data->n_clients * sizeof(int));
    data->intermediary_stats = create_dynamic_memory(data->n_intermediaries * sizeof(int));
    data->enterprise_stats = create_dynamic_memory(data->n_enterprises * sizeof(int));
}

void create_shared_memory_buffers(struct main_data* data, struct comm_buffers* buffers) {

    // Create shared memory for main to client buffer
    buffers->main_client->buffer = (struct operation*) create_shared_memory("main_to_client", data->buffers_size * sizeof(struct operation));
    buffers->main_client->ptrs = (int*) create_shared_memory("main_to_client_ptrs", 2 * sizeof(int));
    memset(buffers->main_client->ptrs, 0, 2 * sizeof(int));

    // Create shared memory for client to intermediary buffers
    for (int i = 0; i < data->n_clients; i++) {
        char buffer_name[20];
        char ptrs_name[20];
        sprintf(buffer_name, "client_to_interm_%d", i);
        sprintf(ptrs_name, "client_to_interm_%d_ptrs", i);
        buffers->client_interm[i].buffer = (struct operation*) create_shared_memory(buffer_name, data->buffers_size * sizeof(struct operation));
        buffers->client_interm[i].ptrs = (struct ptrs*) create_shared_memory(ptrs_name, sizeof(struct ptrs*));
        memset(buffers->client_interm[i].ptrs, 0, sizeof(struct ptrs*));
    }

    // Create shared memory for intermediary to enterprise buffers
    for (int i = 0; i < data->n_intermediaries; i++) {
        char buffer_name[20];
        char ptrs_name[20];
        sprintf(buffer_name, "interm_to_enterp_%d", i);
        sprintf(ptrs_name, "interm_to_enterp_%d_ptrs", i);
        buffers->interm_enterp[i].buffer = (struct operation*) create_shared_memory(buffer_name, data->buffers_size * sizeof(struct operation));
        buffers->interm_enterp[i].ptrs = (struct ptrs*) create_shared_memory(ptrs_name, sizeof(struct ptrs*));
        memset(buffers->interm_enterp[i].ptrs, 0, sizeof(struct ptrs*));
    }

    // Create shared memory for enterprise to intermediary buffers
    // for (int i = 0; i < data->n_enterprises; i++) {
    //     char buffer_name[20];
    //     char ptrs_name[20];
    //     sprintf(buffer_name, "enterp_to_interm_%d", i);
    //     sprintf(ptrs_name, "enterp_to_interm_%d_ptrs", i);
    //     buffers->enterp_to_interm[i].buffer = (struct operation*) create_shared_memory(buffer_name, data->buffers_size * sizeof(struct operation));
    //     buffers->enterp_to_interm[i].ptrs = (struct ptrs*) create_shared_memory(ptrs_name, sizeof(struct ptrs*));
    //     memset(buffers->enterp_to_interm[i].ptrs, 0, sizeof(struct ptrs*));
    // }

    // Create shared memory for intermediary to client buffers
    // for (int i = 0; i < data->n_intermediaries; i++) {
    //     char buffer_name[20];
    //     char ptrs_name[20];
    //     sprintf(buffer_name, "interm_to_client_%d", i);
    //     sprintf(ptrs_name, "interm_to_client_%d_ptrs", i);
    //     buffers->interm_to_client[i].buffer = (struct operation*) create_shared_memory(buffer_name, data->buffers_size * sizeof(struct operation));
    //     buffers->interm_to_client[i].ptrs = (struct ptrs*) create_shared_memory(ptrs_name, sizeof(struct ptrs*));
    //     memset(buffers->interm);
    // }
}

void launch_processes(struct comm_buffers* buffers, struct main_data* data) {
    for(int i = 0; i < data->n_clients; i++)
        data->client_pids = launch_client(i, buffers, data);

    for(int i = 0; i < data->n_intermediaries; i++)
        data->intermediary_pids = launch_interm(i, buffers, data);

    for(int i = 0; i < data->n_enterprises; i++)
        data->enterprise_pids = launch_enterp(i, buffers, data);
}

void user_interaction(struct comm_buffers* buffers, struct main_data* data) {
    //TODO
}

void create_request(int* op_counter, struct comm_buffers* buffers, struct main_data* data) {
    //TODO
}

/* Função que lê um id de operação do utilizador e verifica se a mesma é valida.
* Em caso afirmativo imprime informação da mesma, nomeadamente o seu estado, o 
* id do cliente que fez o pedido, o id da empresa requisitada, e os ids do cliente,
* intermediário, e empresa que a receberam e processaram.
*/
void read_status(struct main_data* data) {
   
}

void stop_execution(struct main_data* data, struct comm_buffers* buffers) {
    //TODO
}

void wait_processes(struct main_data* data) {
    //TODO
}

void write_statistics(struct main_data* data) {
    //TODO
}

void destroy_memory_buffers(struct main_data* data, struct comm_buffers* buffers) {
    // Destroy shared memory for main to client buffer
    destroy_shared_memory("main_to_client", buffers->main_to_client.buffer, data->buffers_size * sizeof(struct operation));
    destroy_shared_memory("main_to_client_ptrs", buffers->main_to_client.ptrs, 2 * sizeof(int));

    // Destroy shared memory for client to intermediary buffers
    for (int i = 0; i < data->n_clients; i++) {
        char buffer_name[20];
        char ptrs_name[20];
        sprintf(buffer_name, "client_to_interm_%d", i);
        sprintf(ptrs_name, "client_to_interm_%d_ptrs", i);
        destroy_shared_memory(buffer_name, buffers->client_to_interm[i].buffer, data->buffers_size * sizeof(struct operation));
        destroy_shared_memory(ptrs_name, buffers->client_to_interm[i].ptrs, sizeof(struct ptrs));
    }

    // Destroy shared memory for intermediary to enterprise buffers
    for (int i = 0; i < data->n_intermediaries; i++) {
        char buffer_name[20];
        char ptrs_name[20];
        sprintf(buffer_name, "interm_to_enterp_%d", i);
        sprintf(ptrs_name, "interm_to_enterp_%d_ptrs", i);
        destroy_shared_memory(buffer_name, buffers->interm_to_enterp[i].buffer, data->buffers_size * sizeof(struct operation));
        destroy_shared_memory(ptrs_name, buffers->interm_to_enterp[i].ptrs, sizeof(struct ptrs));
    }

    // Destroy shared memory for enterprise to intermediary buffers
    for (int i = 0; i < data->n_enterprises; i++) {
        char buffer_name[20];
        char ptrs_name[20];
        sprintf(buffer_name, "enterp_to_interm_%d", i);
        sprintf(ptrs_name, "enterp_to_interm_%d_ptrs", i);
        destroy_shared_memory(buffer_name, buffers->enterp_to_interm[i].buffer, data->buffers_size * sizeof(struct operation));
        destroy_shared_memory(ptrs_name, buffers->enterp_to_interm[i].ptrs, sizeof(struct ptrs));
    }

    // Destroy shared memory for intermediary to client buffers
    for (int i = 0; i < data->n_intermediaries; i++) {
        char buffer_name[20];
        char ptrs_name[20];
        sprintf(buffer_name, "interm_to_client_%d", i);
        sprintf(ptrs_name, "interm_to_client_%d_ptrs", i);
        destroy_shared_memory(buffer_name, buffers->interm_to_client[i].buffer, data->buffers_size * sizeof(struct operation));
        destroy_shared_memory(ptrs_name, buffers->interm_to_client[i].ptrs, sizeof(struct ptrs));
    }

    // Destroy shared memory for results array and terminate flag
    destroy_shared_memory("results", data->results, MAX_RESULTS * sizeof(struct operation));
    destroy_shared_memory("terminate", data->terminate, sizeof(int));

    // Free dynamic memory for pid and stats arrays
    destroy_dynamic_memory(data->client_pids);
    destroy_dynamic_memory(data->intermediary_pids);
    destroy_dynamic_memory(data->enterprise_pids);
    destroy_dynamic_memory(data->client_stats);
    destroy_dynamic_memory(data->intermediary_stats);
    destroy_dynamic_memory(data->enterprise_stats);
}

int main(int argc, char *argv[]) {
    // init data structures
    struct main_data *data = create_dynamic_memory(sizeof(struct main_data));
    struct comm_buffers *buffers = create_dynamic_memory(sizeof(struct comm_buffers));
    buffers->main_client = create_dynamic_memory(sizeof(struct rnd_access_buffer));
    buffers->client_interm = create_dynamic_memory(sizeof(struct circular_buffer));
    buffers->interm_enterp = create_dynamic_memory(sizeof(struct rnd_access_buffer));

    // execute main code
    main_args(argc, argv, data);
    create_dynamic_memory_buffers(data);
    create_shared_memory_buffers(data, buffers);
    launch_processes(buffers, data);
    user_interaction(buffers, data);

    // release memory before terminating
    destroy_dynamic_memory(data);
    destroy_dynamic_memory(buffers->main_client);
    destroy_dynamic_memory(buffers->client_interm);
    destroy_dynamic_memory(buffers->interm_enterp);
    destroy_dynamic_memory(buffers);

    return 0;
}