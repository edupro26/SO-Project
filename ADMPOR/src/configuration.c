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
            //save_to_main_data(buffer, line, data);
        }
    } while (keep_reading);

    line++;
}

// void save_to_main_data(char* buffer[], int line, struct main_data* data) {

// }