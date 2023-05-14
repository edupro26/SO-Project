#include <stdio.h>
#include <stdlib.h>
#include <time.h>

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

    struct tm localtime;
    get_local_time(&localtime);
    char buffer[50];
    strftime(buffer, 50, "%Y-%m-%d %H:%M:%S", &localtime);

    fprintf(log, "%s %s %d %d\n", buffer, "op", op->requesting_client, op->requested_enterp);

    fclose(log);
}

void log_status(struct main_data* data, int op_id) {
    FILE *log;

    log = fopen(data->log_file_name, "a");
    if(log == NULL){
        perror("Unable to open log file");
        exit(1);
    }

    struct tm localtime;
    get_local_time(&localtime);
    char buffer[50];
    strftime(buffer, 50, "%Y-%m-%d %H:%M:%S", &localtime);

    fprintf(log, "%s %s %d\n", buffer, "status", op_id);

    fclose(log);
}

void log_append(struct main_data* data, const char* command) {
    FILE *log;

    log = fopen(data->log_file_name, "a");
    if(log == NULL){
        perror("Unable to open log file");
        exit(1);
    }

    struct tm localtime;
    get_local_time(&localtime);
    char buffer[50];
    strftime(buffer, 50, "%Y-%m-%d %H:%M:%S", &localtime);

    fprintf(log, "%s %s\n", buffer, command);

    fclose(log);
}