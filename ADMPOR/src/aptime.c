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

#include "aptime.h"

void register_start_time(struct operation* op) {
    if (clock_gettime(CLOCK_REALTIME, &op->start_time) == -1) {
        perror("clock gettime");
        exit(1);
    }
}

void register_client_time(struct operation* op) {
    if (clock_gettime(CLOCK_REALTIME, &op->client_time) == -1) {
        perror("clock gettime");
        exit(1);
    }
}

void register_intermd_time(struct operation* op) {
    if (clock_gettime(CLOCK_REALTIME, &op->intermed_time) == -1) {
        perror("clock gettime");
        exit(1);
    }
}

void register_enterp_time(struct operation* op) {
    if (clock_gettime(CLOCK_REALTIME, &op->enterp_time) == -1) {
        perror("clock gettime");
        exit(1);
    }
}