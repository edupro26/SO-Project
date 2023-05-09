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
#include <ctype.h>

#include "process.h"
#include "main-private.h"
#include "main.h"

int isNumber(char n[]) {
    for (int i = 0; n[i] != 0; i++) {
        if (!isdigit(n[i]))
            return 0;
    }

    return 1;
}

int check_args(int argc, char* argv[]){
    if (argc != 6) return 1;

    for (int i = 1; i < argc; i++) {
        if(!isNumber(argv[i]))
            return 1;

        if(atoi(argv[i]) < 1) 
            return 1;
    }

    return 0;
}

void main_args(int argc, char* argv[], struct main_data* data) {
    if (check_args(argc, argv) == 0) {
        int temp[5];
        for (int i = 1; i < argc; i++)
            temp[i-1] = atoi(argv[i]);

        data->max_ops = temp[0];
        data->buffers_size = temp[1];
        data->n_clients = temp[2];
        data->n_intermediaries= temp[3];
        data->n_enterprises = temp[4];
    }
    else {
        printf("Uso: admpor max_ops buffers_size n_clients n_intermediaries n_enterprises\nExemplo: ./bin/admpor 10 10 1 1 1\n");
        exit(1);
    }
}

void create_dynamic_memory_buffers(struct main_data* data) {
    data->client_pids = create_dynamic_memory(sizeof(int));
    data->intermediary_pids = create_dynamic_memory(sizeof(int));
    data->enterprise_pids = create_dynamic_memory(sizeof(int));
    
    data->client_stats = create_dynamic_memory(sizeof(int));
    data->intermediary_stats = create_dynamic_memory(sizeof(int));
    data->enterprise_stats = create_dynamic_memory(sizeof(int));
}

void create_shared_memory_buffers(struct main_data* data, struct comm_buffers* buffers) {
    // main_client buffer
    buffers->main_client->ptrs = (int*)create_shared_memory(STR_SHM_MAIN_CLIENT_PTR, sizeof(int));
    buffers->main_client->buffer = (struct operation*)create_shared_memory(STR_SHM_MAIN_CLIENT_BUFFER, sizeof(struct operation));

    // client_interm buffer
    buffers->client_interm->ptrs = (struct pointers*)create_shared_memory(STR_SHM_CLIENT_INTERM_PTR, sizeof(struct pointers));
    buffers->client_interm->ptrs->in = 0;
    buffers->client_interm->ptrs->out = 0;
    buffers->client_interm->buffer = (struct operation*)create_shared_memory(STR_SHM_CLIENT_INTERM_BUFFER, sizeof(struct operation));

    // interm_enterp buffer
    buffers->interm_enterp->ptrs = (int*)create_shared_memory(STR_SHM_INTERM_ENTERP_PTR, sizeof(int));
    buffers->interm_enterp->buffer = (struct operation*)create_shared_memory(STR_SHM_INTERM_ENTERP_BUFFER, sizeof(struct operation));

    // results array
    data->results = (struct operation*)create_shared_memory(STR_SHM_RESULTS, sizeof(struct operation) * MAX_RESULTS);

    // terminate flag
    data->terminate = (int*)create_shared_memory(STR_SHM_TERMINATE, sizeof(int));
    *data->terminate = 0;
}

void launch_processes(struct comm_buffers* buffers, struct main_data* data, struct semaphores* sems) {
    for(int i = 0; i < data->n_clients; i++)
        data->client_pids[i] = launch_client(i, buffers, data);

    for(int i = 0; i < data->n_intermediaries; i++)
        data->intermediary_pids[i] = launch_interm(i, buffers, data);

    for(int i = 0; i < data->n_enterprises; i++)
        data->enterprise_pids[i] = launch_enterp(i, buffers, data);
}

void print_help() {
    printf("Ações disponíveis:\n");
    printf("    op client empresa - criar uma nova operação\n");
    printf("    status id - consultar o estado de uma operação\n");
    printf("    stop - termina a execução do AdmPor.\n");
    printf("    help - imprime informação sobre as ações disponíveis.\n");
}

void user_interaction(struct comm_buffers* buffers, struct main_data* data, struct semaphores* sems) {
    char command[10];
    int op_counter = 0;
    print_help();

    while (1) {
        printf("Introduzir ação:\n");
        scanf("%s", command);

        if (strcmp(command, "op") == 0) {
            create_request(&op_counter, buffers, data);
            usleep(100000);
        } 
        else if (strcmp(command, "status") == 0) {
            read_status(data);
        } 
        else if (strcmp(command, "stop") == 0) {
            stop_execution(data, buffers);
            exit(1);
        } 
        else if (strcmp(command, "help") == 0) {
            print_help();
        } 
        else {
            printf("Ação não reconhecida, insira 'help' para assistência..\n");
        }
    }
}

int check_request(char id1[], char id2[]) {
    if(!isNumber(id1) || !isNumber(id2))
        return 1;
    
    int client_id = atoi(id1);
    int enterp_id = atoi(id2);
    if(client_id < 0 || enterp_id < 0)
        return 1;

    return 0;
}

void create_request(int* op_counter, struct comm_buffers* buffers, struct main_data* data, struct semaphores* sems) {
    char id1[10], id2[10];
    scanf("%s", id1);
    scanf("%s", id2);
    
    if(check_request(id1, id2) == 1){
        printf("id de cliente ou empresa inválido!\n");
        return;
    }

    int client_id = atoi(id1);
    int enterp_id = atoi(id2);

    struct operation op;
    op.id = *op_counter;
    op.requesting_client = client_id;
    op.requested_enterp = enterp_id;
    op.status = 'M';

    data->results[*op_counter] = op;
    struct operation *p_op = &op;
    write_main_client_buffer(buffers->main_client, data->buffers_size, p_op);

    printf("O pedido #%d foi criado \n", *op_counter);
    (*op_counter)++;
}

void print_status(int id, struct main_data* data) {
    int client_id = data->results[id].requesting_client;
    int enterprise_id = data->results[id].requested_enterp;
    int receiving_client = data->results[id].receiving_client;
    int receiving_interm = data->results[id].receiving_interm;
    int receiving_enterp = data->results[id].receiving_enterp;

    char status = data->results[id].status;
    switch (status) {
    case 'M':
        printf("Pedido %d com estado M requesitado pelo ciente %d à empresa %d\n", id, client_id, enterprise_id);
        break;

    case 'C':
        printf("Pedido %d com estado C requesitado pelo cliente %d à empresa %d, foi recebido pelo cliente %d\n", id, client_id, enterprise_id, receiving_client);
        break;

    case 'I':
        printf("Pedido %d com estado I requisitado pelo cliente %d à empresa %d, foi recebido pelo cliente %d e processado pelo intermediário %d!\n", id, client_id, enterprise_id, receiving_client, receiving_interm);
        break;

    case 'A':
        printf("Pedido %d com estado A requisitado pelo cliente %d à empresa %d, foi recebido pelo cliente %d, processado pelo intermediário %d, e tratado pela empresa %d!\n", id, client_id, enterprise_id, receiving_client, receiving_interm, receiving_enterp);
        break;

    case 'E':
        printf("Pedido %d com estado E requisitado pelo cliente %d à empresa %d, foi recebido pelo cliente %d, processado pelo intermediário %d, e tratado pela empresa %d!\n", id, client_id, enterprise_id, receiving_client, receiving_interm, receiving_enterp);
        break;

    default:
        printf("Pedido %d ainda não é válido!\n", id);
        break;
    }
}

int check_status(char id[]) {
    if(!isNumber(id))
        return 1;
    
    int op_id = atoi(id);
    if(op_id < 0 || op_id >= MAX_RESULTS)
        return 1;

    return 0;
}

void read_status(struct main_data* data, struct semaphores* sems) {
    char id[10];
    scanf("%s", id);

    if (check_status(id) == 1) {
        printf("id de pedido fornecido é inválido!\n"); 
        return;
    }

    int op_id = atoi(id);
    print_status(op_id, data);
}

void stop_execution(struct main_data* data, struct comm_buffers* buffers, struct semaphores* sems) {
    *(data->terminate) = 1;
    printf("Terminando o AdmPor! Imprimindo estatísticas:\n");
    wait_processes(data);
    write_statistics(data);
    destroy_memory_buffers(data, buffers);
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

void write_statistics(struct main_data* data) {
    for (int i = 0; i < data->n_clients; i++)
        printf("Cliente %d processou %d pedidos!\n", i, data->client_stats[i]);

    for (int i = 0; i < data->n_intermediaries; i++)
        printf("Intermidiário %d entregou %d pedidos!\n", i, data->intermediary_stats[i]);

    for (int i = 0; i < data->n_enterprises; i++) 
        printf("Empresa %d recebeu %d pedidos!\n", i, data->enterprise_stats[i]);
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
    destroy_shared_memory(STR_SHM_MAIN_CLIENT_PTR, buffers->main_client->ptrs, sizeof(int));
    destroy_shared_memory(STR_SHM_MAIN_CLIENT_BUFFER, buffers->main_client->buffer, sizeof(struct operation));
    destroy_shared_memory(STR_SHM_CLIENT_INTERM_PTR, buffers->client_interm->ptrs, sizeof(struct pointers));
    destroy_shared_memory(STR_SHM_CLIENT_INTERM_BUFFER, buffers->client_interm->buffer, sizeof(struct operation));
    destroy_shared_memory(STR_SHM_INTERM_ENTERP_PTR, buffers->interm_enterp->ptrs, sizeof(int));
    destroy_shared_memory(STR_SHM_INTERM_ENTERP_BUFFER, buffers->interm_enterp->buffer, sizeof(struct operation));
    destroy_shared_memory(STR_SHM_RESULTS, data->results, MAX_RESULTS * sizeof(struct operation));
    destroy_shared_memory(STR_SHM_TERMINATE, data->terminate, sizeof(int));
}

void create_semaphores(struct main_data* data, struct semaphores* sems) {
    //TODO
}

void wakeup_processes(struct main_data* data, struct semaphores* sems) {
    //TODO
}

void destroy_semaphores(struct semaphores* sems) {
    //TODO
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