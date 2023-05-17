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
#include <ctype.h>

#include "configuration.h"

void read_input_file(char* filename, struct main_data* data) {
    FILE *input;

    input = fopen(filename, "r");

    if(input == NULL){
        perror("Unable to open config file");
        exit(1);
    }

    if(ferror(input) != 0) {
        perror("Error reading config file");
        exit(1);
    }

    read_file(input, data);
    fclose(input);
}

void read_file(FILE *input, struct main_data* data) {
    char buffer[MAX_LINE];

    int line = 0;
    int keep_reading = 1;

    do {
        if(feof(input)){
            keep_reading = 0;
        }
        else {
            fgets(buffer, MAX_LINE, input);
            save_to_main_data(buffer, line, data);
        }
        line++;

    } while (keep_reading);
}

void save_to_main_data(char* buffer, int line, struct main_data* data) {
    int temp = atoi(buffer);
    switch (line) {
        case 0:
            data->max_ops = temp;
            break;
        case 1:
            data->buffers_size = temp;
            break;
        case 2:
            data->n_clients = temp;
            break;
        case 3:
            data->n_intermediaries = temp;
            break;
        case 4:
            data->n_enterprises = temp;
            break;
        case 5:
            buffer[strcspn(buffer, "\n")] = 0;
            strcpy(data->log_file_name, buffer);
            break;
        case 6:
            buffer[strcspn(buffer, "\n")] = 0;
            strcpy(data->stats_file_name, buffer);
            break;
        case 7:
            data->alarm_time = temp;
            break;
    }
}

int check_args(int argc, char* argv[]) {
    if(argc != 2) 
        return 0;

    if(strcmp(argv[1], "config.txt"))
        return 0;

    return 1;
}

int isNumber(char* n) {
    for (int i = 0; n[i] != 0; i++) {
        if (!isdigit(n[i]))
            return 0;
    }

    return 1;
}

int check_request(char* id1, char* id2) {
    if(!isNumber(id1) || !isNumber(id2))
        return 1;
    
    int client_id = atoi(id1);
    int enterp_id = atoi(id2);
    if(client_id < 0 || enterp_id < 0)
        return 1;

    return 0;
}

int check_status(char* id) {
    if(!isNumber(id))
        return 1;
    
    int op_id = atoi(id);
    if(op_id < 0 || op_id >= MAX_RESULTS)
        return 1;

    return 0;
}

void print_help() {
    printf("Ações disponíveis:\n");
    printf("    op client empresa - criar uma nova operação\n");
    printf("    status id - consultar o estado de uma operação\n");
    printf("    stop - termina a execução do AdmPor.\n");
    printf("    help - imprime informação sobre as ações disponíveis.\n");
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