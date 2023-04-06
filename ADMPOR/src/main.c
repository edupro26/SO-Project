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
    // main_client buffer
    buffers->main_client = (struct rnd_access_buffer*)create_shared_memory(STR_SHM_MAIN_CLIENT_BUFFER, sizeof(struct rnd_access_buffer));
    buffers->main_client->ptrs = (int*)create_shared_memory(STR_SHM_MAIN_CLIENT_PTR, sizeof(int) * 2);
    buffers->main_client->buffer = (struct operation*)create_dynamic_memory(sizeof(struct operation) * data->max_ops);

    // client_interm buffer
    buffers->client_interm = (struct circular_buffer*)create_shared_memory(STR_SHM_CLIENT_INTERM_BUFFER, sizeof(struct circular_buffer));
    buffers->client_interm->ptrs = (struct pointers*)create_shared_memory(STR_SHM_CLIENT_INTERM_PTR, sizeof(struct pointers));
    buffers->client_interm->ptrs->in = 0;
    buffers->client_interm->ptrs->out = 0;
    buffers->client_interm->buffer = (struct operation*)create_dynamic_memory(sizeof(struct operation) * data->max_ops);

    // interm_enterp buffer
    buffers->interm_enterp = (struct rnd_access_buffer*)create_shared_memory(STR_SHM_INTERM_ENTERP_BUFFER, sizeof(struct rnd_access_buffer));
    buffers->interm_enterp->ptrs = (int*)create_shared_memory(STR_SHM_INTERM_ENTERP_PTR, sizeof(int) * 2);
    buffers->interm_enterp->buffer = (struct operation*)create_dynamic_memory(sizeof(struct operation) * data->max_ops);

    // results array
    data->results = (struct operation*)create_shared_memory(STR_SHM_RESULTS, sizeof(struct operation) * MAX_RESULTS);

    // terminate flag
    data->terminate = (int*)create_shared_memory(STR_SHM_TERMINATE, sizeof(int));
    *data->terminate = 0;
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
    char command[10];
    int op_counter = 0;
    
    while (!(*data->terminate)) {
        printf("Introduzir ação:\n");
        scanf("%s", command);
        
        if (strcmp(command, "op") == 0) {
            create_request(&op_counter, buffers, data);
        } else if (strcmp(command, "status") == 0) {
            read_status(data);
        } else if (strcmp(command, "stop") == 0) {
            stop_execution(data, buffers);
        } else if (strcmp(command, "help") == 0) {
            printf("Ações disponíveis:\n");
            printf("op client empresa - criar uma nova operação\n");
            printf("status id - consultar o estado de uma operação\n");
            printf("stop - termina a execução do AdmPor.\n");
            printf("help - imprime informação sobre as ações disponíveis.\n");
        } else {
            printf("Ação não reconhecida, insira 'help' para assistência..\n");
        }
    }

    
}

void create_request(int* op_counter, struct comm_buffers* buffers, struct main_data* data) {
    char temp[0];
    scanf("%s", temp);
    int client_id = atoi(temp);
    scanf("%s", temp);
    int enterp_id = atoi(temp);

    buffers->main_client->buffer->id = op_counter;
    buffers->main_client->buffer->requesting_client = client_id;
    buffers->main_client->buffer->requested_enterp = enterp_id;

    printf("O pedido #%d foi criado \n", *op_counter);
    (*op_counter)++;
}

void read_status(struct main_data* data) {
    int op_id;
    scanf("%d", &op_id);
    if (op_id >= 100) {
        printf("id de pedido fornecido é inválido!\n"); 
        return;
    }

    if (op_id < 0 || data->results[op_id].id == -1) { // Need testing with ids bigger that results size
        printf("Pedido %d ainda não é válido!\n", op_id);
        return;
    }

    char status = data->results[op_id].status;
    int client_id = data->results[op_id].requesting_client;
    int enterprise_id = data->results[op_id].requested_enterp;

    printf("Pedido %d com estado %c requesitado pelo ciente %d à empresa %d\n", op_id, status, client_id, enterprise_id);

}

void stop_execution(struct main_data* data, struct comm_buffers* buffers) {
    *(data->terminate) = 1;

    printf("Terminando o AdmPor! Imprimindo estatísticas:\n");

    wait_processes(data);
    write_statistics(data);
    destroy_memory_buffers(data, buffers);

}

void wait_processes(struct main_data* data) {
    // client processes
    for (int i = 0; i < data->n_clients; i++) {
        wait_process(data->client_pids[i]);
    }
    
    // intermediary processes
    for (int i = 0; i < data->n_intermediaries; i++) {
        wait_process(data->intermediary_pids[i]);
    }
    
    // enterprise processes
    for (int i = 0; i < data->n_enterprises; i++) {
        wait_process(data->enterprise_pids[i]);
    }
}

void write_statistics(struct main_data* data) {
    for (int i = 0; i < data->n_clients; i++) {
        printf("Cliente %d processou %d pedidos!\n", i, data->client_stats[i]);
    }
    for (int i = 0; i < data->n_intermediaries; i++) {
        printf("Intermidiário %d entregou %d pedidos!\n", i, data->intermediary_stats[i]);
    }
    for (int i = 0; i < data->n_enterprises; i++) {
        printf("Empresa %d recebeu %d pedidos!\n", i, data->enterprise_stats[i]);
    }
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
    destroy_shared_memory(STR_SHM_MAIN_CLIENT_PTR, buffers->main_client->ptrs, data->buffers_size);
    destroy_shared_memory(STR_SHM_MAIN_CLIENT_BUFFER, buffers->main_client->buffer, data->buffers_size * sizeof(struct operation));
    destroy_shared_memory(STR_SHM_CLIENT_INTERM_PTR, buffers->client_interm->ptrs, sizeof(struct pointers));
    destroy_shared_memory(STR_SHM_CLIENT_INTERM_BUFFER, buffers->client_interm->buffer, data->buffers_size * sizeof(struct operation));
    destroy_shared_memory(STR_SHM_INTERM_ENTERP_PTR, buffers->interm_enterp->ptrs, sizeof(struct pointers));
    destroy_shared_memory(STR_SHM_INTERM_ENTERP_BUFFER, buffers->interm_enterp->buffer, data->buffers_size * sizeof(struct operation));
    destroy_shared_memory(STR_SHM_RESULTS, data->results, MAX_RESULTS * sizeof(struct operation));
    destroy_shared_memory(STR_SHM_TERMINATE, data->terminate, sizeof(int));

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