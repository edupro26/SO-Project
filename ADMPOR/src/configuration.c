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

void save_to_main_data(char buffer[], int line, struct main_data* data) {
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
            strcpy(data->log_file_name, buffer);
            break;
        case 6:
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