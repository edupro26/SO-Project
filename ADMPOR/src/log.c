/* 

Sistemas Operativos - Projeto Fase 1 
Grupo 10
Eduardo Proença  - 57551
Tiago Oliveira - 54979

*/

#include <stdio.h>
#include <stdlib.h>

#include "log.h"

void log_init(struct main_data* data) {
    FILE *log;

    log = fopen(data->log_file_name, "w");
    if (log == NULL) {
        perror("Unable to open log file");
        exit(1);
    }

    fclose(log);
}

void log_operation(struct main_data* data, struct operation* op) {
    FILE *log;

    log = fopen(data->log_file_name, "a");
    if(log == NULL){
        perror("Unable to open log file");
        exit(1);
    }

    register_start_time(op);
    char buffer[50];
    strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", localtime(&(op->start_time.tv_sec)));
    fprintf(log, "%s.%03ld %s %d %d\n", buffer, op->start_time.tv_nsec / 1000000, "op", op->requesting_client, op->requested_enterp);
    fclose(log);
}

void log_status(struct main_data* data, int op_id) {
    FILE *log;

    log = fopen(data->log_file_name, "a");
    if(log == NULL){
        perror("Unable to open log file");
        exit(1);
    }

    struct timespec t;
    if (clock_gettime(CLOCK_REALTIME, &t) == -1) {
        perror("clock gettime");
        exit(1);
    }

    char buffer[50];
    strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", localtime(&t.tv_sec));
    fprintf(log, "%s.%03ld %s %d\n", buffer, t.tv_nsec / 1000000, "status", op_id);
    fclose(log);
}

void log_append(struct main_data* data, const char* command) {
    FILE *log;

    log = fopen(data->log_file_name, "a");
    if(log == NULL){
        perror("Unable to open log file");
        exit(1);
    }

    struct timespec t;
    if (clock_gettime(CLOCK_REALTIME, &t) == -1) {
        perror("clock gettime");
        exit(1);
    }

    char buffer[50];
    strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", localtime(&t.tv_sec));
    fprintf(log, "%s.%03ld %s\n", buffer, t.tv_nsec / 1000000, command);
    fclose(log);
}