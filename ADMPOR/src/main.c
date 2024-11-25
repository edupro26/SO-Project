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

#include "main.h"
#include "process.h"
#include "configuration.h"
#include "apsignal.h"
#include "log.h"
#include "stats.h"


void main_args(int argc, char* argv[], struct main_data* data) {
    if(!check_args(argc, argv)){
        printf("Uso: admpor config.txt\nExemplo: ./bin/admpor config.txt\n");
        exit(1);
    }
    else{
        read_input_file(argv[1], data);
    }
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
    // main_client buffer
    buffers->main_client->ptrs = (int*)create_shared_memory(STR_SHM_MAIN_CLIENT_PTR, data->buffers_size * sizeof(int));
    buffers->main_client->buffer = (struct operation*)create_shared_memory(STR_SHM_MAIN_CLIENT_BUFFER, data->buffers_size * sizeof(struct operation));

    // client_interm buffer
    buffers->client_interm->ptrs = (struct pointers*)create_shared_memory(STR_SHM_CLIENT_INTERM_PTR, data->buffers_size * sizeof(struct pointers));
    buffers->client_interm->ptrs->in = 0;
    buffers->client_interm->ptrs->out = 0;
    buffers->client_interm->buffer = (struct operation*)create_shared_memory(STR_SHM_CLIENT_INTERM_BUFFER, data->buffers_size * sizeof(struct operation));

    // interm_enterp buffer
    buffers->interm_enterp->ptrs = (int*)create_shared_memory(STR_SHM_INTERM_ENTERP_PTR, data->buffers_size * sizeof(int));
    buffers->interm_enterp->buffer = (struct operation*)create_shared_memory(STR_SHM_INTERM_ENTERP_BUFFER, data->buffers_size * sizeof(struct operation));

    // results array
    data->results = (struct operation*)create_shared_memory(STR_SHM_RESULTS, sizeof(struct operation) * MAX_RESULTS);

    // terminate flag
    data->terminate = (int*)create_shared_memory(STR_SHM_TERMINATE, sizeof(int));
    *data->terminate = 0;
}

void launch_processes(struct comm_buffers* buffers, struct main_data* data, struct semaphores* sems) {
    for(int i = 0; i < data->n_clients; i++)
        data->client_pids[i] = launch_client(i, buffers, data, sems);

    for(int i = 0; i < data->n_intermediaries; i++)
        data->intermediary_pids[i] = launch_interm(i, buffers, data, sems);

    for(int i = 0; i < data->n_enterprises; i++)
        data->enterprise_pids[i] = launch_enterp(i, buffers, data, sems);
}

void user_interaction(struct comm_buffers* buffers, struct main_data* data, struct semaphores* sems) {
    char command[20];
    int op_counter = 0;
    set_alarm(&op_counter);
    load_opcounter_stats(&op_counter);
    log_init(data);
    print_help();

    printf("\nAlarme definido para %d segundos\n", data->alarm_time);
    while (1) {
        printf("\nIntroduzir ação:\n");
        scanf("%s", command);

        if (strcmp(command, "op") == 0) {
            create_request(&op_counter, buffers, data, sems);
            usleep(100000);
        } 
        else if (strcmp(command, "status") == 0) {
            read_status(data, sems);
        } 
        else if (strcmp(command, "stop") == 0) {
            log_append(data, "stop");
            stop_execution(data, buffers, sems);
            exit(1);
        } 
        else if (strcmp(command, "help") == 0) {
            log_append(data, "help");
            print_help();
        } 
        else {
            char temp[100];
            fgets(temp, sizeof(temp), stdin);
            printf("Ação não reconhecida, insira 'help' para assistência..\n");
        }
    }
}

void create_request(int* op_counter, struct comm_buffers* buffers, struct main_data* data, struct semaphores* sems) {
    char id1[10], id2[10];
    scanf("%s %s", id1, id2);
    
    if(check_request(id1, id2) == 1){
        printf("id de cliente ou empresa inválido!\n");
        return;
    }
    if(*op_counter >= MAX_RESULTS){
        printf("O numero maximo de operações foi atingido!\n");
        return;
    }
    int client_id = atoi(id1);
    int enterp_id = atoi(id2);

    struct operation op;
    op.id = *op_counter;
    op.requesting_client = client_id;
    op.requested_enterp = enterp_id;
    op.status = 'M';

    log_operation(data, &op);
    data->results[*op_counter] = op;
    produce_begin(sems->main_client);
    write_main_client_buffer(buffers->main_client, data->buffers_size, &op);
    produce_end(sems->main_client);
    printf("O pedido #%d foi criado \n", *op_counter);
    (*op_counter)++;
}

void read_status(struct main_data* data, struct semaphores* sems) {
    char id[10];
    scanf("%s", id);

    if (check_status(id) == 1) {
        printf("id de pedido fornecido é inválido!\n"); 
        return;
    }

    int op_id = atoi(id);
    semaphore_mutex_lock(sems->results_mutex);
    log_status(data, op_id);
    print_status(op_id, data);
    semaphore_mutex_unlock(sems->results_mutex);
}

void stop_execution(struct main_data* data, struct comm_buffers* buffers, struct semaphores* sems) {
    *(data->terminate) = 1;
    wakeup_processes(data, sems);
    wait_processes(data);

    printf("\nTerminando o AdmPor! Imprimindo estatísticas:\n");
    write_statistics(data);
    
    
    destroy_semaphores(sems);
    destroy_memory_buffers(data, buffers);
}

void write_statistics(struct main_data* data) {
    // int num_ops = 0; // TODO - get number of operations
    write_statistics_to_file(data);

    for (int i = 0; i < data->n_clients; i++)
        printf("Cliente %d processou %d pedidos!\n", i, data->client_stats[i]);

    for (int i = 0; i < data->n_intermediaries; i++)
        printf("Intermidiário %d entregou %d pedidos!\n", i, data->intermediary_stats[i]);

    for (int i = 0; i < data->n_enterprises; i++) 
        printf("Empresa %d recebeu %d pedidos!\n", i, data->enterprise_stats[i]);
}

void wait_processes(struct main_data* data) {
    // client processes
    for (int i = 0; i < data->n_clients; i++)
        data->client_stats[i] = wait_process(data->client_pids[i]);
    
    // intermediary processes
    for (int i = 0; i < data->n_intermediaries; i++)
        data->intermediary_stats[i] = wait_process(data->intermediary_pids[i]);
    
    // enterprise processes
    for (int i = 0; i < data->n_enterprises; i++)
        data->enterprise_stats[i] = wait_process(data->enterprise_pids[i]);
}

void destroy_memory_buffers(struct main_data* data, struct comm_buffers* buffers) {
    // Destroy dynamic memory
    destroy_dynamic_memory(data->client_pids);
    destroy_dynamic_memory(data->intermediary_pids);
    destroy_dynamic_memory(data->enterprise_pids);
    destroy_dynamic_memory(data->client_stats);
    destroy_dynamic_memory(data->intermediary_stats);
    destroy_dynamic_memory(data->enterprise_stats);

    // Destroy shared memory
    destroy_shared_memory(STR_SHM_MAIN_CLIENT_PTR, buffers->main_client->ptrs, data->buffers_size * sizeof(int));
    destroy_shared_memory(STR_SHM_MAIN_CLIENT_BUFFER, buffers->main_client->buffer, data->buffers_size * sizeof(struct operation));
    destroy_shared_memory(STR_SHM_CLIENT_INTERM_PTR, buffers->client_interm->ptrs, data->buffers_size * sizeof(struct pointers));
    destroy_shared_memory(STR_SHM_CLIENT_INTERM_BUFFER, buffers->client_interm->buffer, data->buffers_size * sizeof(struct operation));
    destroy_shared_memory(STR_SHM_INTERM_ENTERP_PTR, buffers->interm_enterp->ptrs, data->buffers_size * sizeof(int));
    destroy_shared_memory(STR_SHM_INTERM_ENTERP_BUFFER, buffers->interm_enterp->buffer, data->buffers_size * sizeof(struct operation));
    destroy_shared_memory(STR_SHM_RESULTS, data->results, MAX_RESULTS * sizeof(struct operation));
    destroy_shared_memory(STR_SHM_TERMINATE, data->terminate, sizeof(int));
}

void create_semaphores(struct main_data* data, struct semaphores* sems) {
    sems->main_client->full = semaphore_create(STR_SEM_MAIN_CLIENT_FULL, 0);
    sems->client_interm->full = semaphore_create(STR_SEM_CLIENT_INTERM_FULL, 0);
    sems->interm_enterp->full = semaphore_create(STR_SEM_INTERM_ENTERP_FULL, 0);

    sems->main_client->empty = semaphore_create(STR_SEM_MAIN_CLIENT_EMPTY, data->buffers_size);
    sems->client_interm->empty = semaphore_create(STR_SEM_CLIENT_INTERM_EMPTY, data->buffers_size);
    sems->interm_enterp->empty = semaphore_create(STR_SEM_INTERM_ENTERP_EMPTY, data->buffers_size);

    sems->main_client->mutex = semaphore_create(STR_SEM_MAIN_CLIENT_MUTEX, 1);
    sems->client_interm->mutex = semaphore_create(STR_SEM_CLIENT_INTERM_MUTEX, 1);
    sems->interm_enterp->mutex = semaphore_create(STR_SEM_INTERM_ENTERP_MUTEX, 1);

    sems->results_mutex = semaphore_create(STR_SEM_RESULTS_MUTEX, 1);
}

void wakeup_processes(struct main_data* data, struct semaphores* sems) {
    for (int i = 0; i < data->n_clients; i++)
        produce_end(sems->main_client);

    for (int i = 0; i < data->n_intermediaries; i++)
        produce_end(sems->client_interm);
    
    for (int i = 0; i < data->n_enterprises; i++)
        produce_end(sems->interm_enterp);
}

void destroy_semaphores(struct semaphores* sems) {
    semaphore_destroy(STR_SEM_MAIN_CLIENT_FULL, sems->main_client->full);
    semaphore_destroy(STR_SEM_CLIENT_INTERM_FULL, sems->client_interm->full);
    semaphore_destroy(STR_SEM_INTERM_ENTERP_FULL, sems->interm_enterp->full);

    semaphore_destroy(STR_SEM_MAIN_CLIENT_EMPTY, sems->main_client->empty);
    semaphore_destroy(STR_SEM_CLIENT_INTERM_EMPTY, sems->client_interm->empty);
    semaphore_destroy(STR_SEM_INTERM_ENTERP_EMPTY, sems->interm_enterp->empty);

    semaphore_destroy(STR_SEM_MAIN_CLIENT_MUTEX, sems->main_client->mutex);
    semaphore_destroy(STR_SEM_CLIENT_INTERM_MUTEX, sems->client_interm->mutex);
    semaphore_destroy(STR_SEM_INTERM_ENTERP_MUTEX, sems->interm_enterp->mutex);

    semaphore_destroy(STR_SEM_RESULTS_MUTEX, sems->results_mutex);
}

int main(int argc, char *argv[]) {
    //init data structures
    struct main_data* data = create_dynamic_memory(sizeof(struct main_data));
    struct comm_buffers* buffers = create_dynamic_memory(sizeof(struct comm_buffers));
    buffers->main_client = create_dynamic_memory(sizeof(struct rnd_access_buffer));
    buffers->client_interm = create_dynamic_memory(sizeof(struct circular_buffer));
    buffers-> interm_enterp = create_dynamic_memory(sizeof(struct rnd_access_buffer));

    // init semaphore data structure
    struct semaphores* sems = create_dynamic_memory(sizeof(struct semaphores));
    sems->main_client = create_dynamic_memory(sizeof(struct prodcons));
    sems->client_interm = create_dynamic_memory(sizeof(struct prodcons));
    sems->interm_enterp = create_dynamic_memory(sizeof(struct prodcons));

    //execute main code
    ctrlC(data, buffers, sems);
    main_args(argc, argv, data);
    create_dynamic_memory_buffers(data);
    create_shared_memory_buffers(data, buffers);
    create_semaphores(data, sems);
    launch_processes(buffers, data, sems);
    user_interaction(buffers, data, sems);

    //release memory before terminating
    destroy_dynamic_memory(data);
    destroy_dynamic_memory(buffers->main_client);
    destroy_dynamic_memory(buffers->client_interm);
    destroy_dynamic_memory(buffers->interm_enterp);
    destroy_dynamic_memory(buffers);
    destroy_dynamic_memory(sems->main_client);
    destroy_dynamic_memory(sems->client_interm);
    destroy_dynamic_memory(sems->interm_enterp);
    destroy_dynamic_memory(sems);

    return 0;
}