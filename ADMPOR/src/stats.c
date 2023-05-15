/* 

Sistemas Operativos - Projeto Fase 1 
Grupo 10
Eduardo Proença  - 57551
Tiago Oliveira - 54979

*/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "stats.h";
#include "main.h";


void write_statistics_to_file(struct main_data* data, int num_ops, char* filename){
    stats_file = fopen(filename, "w");

    if(stats_file == NULL){
        perror("Unable to open stats file");
        exit(1);
    }

    write_process_statistics(data);
    write_ops_statistics(data->results, num_ops);

    fclose(stats_file);

    stats_file = NULL;

}

void write_process_statistics(struct main_data* data) {
    fprintf(stats_file, "Process Statistics:\n");

    for (int i = 0; i < data->n_clients; i++)
        fprintf(stats_file, "Client %d received %d operation(s)!\n", i, data->client_stats[i]);

    for (int i = 0; i < data->n_intermediaries; i++)
        fprintf(stats_file, "Intermidiary %d prepared %d operation(s)!\n", i, data->intermediary_stats[i]);

    for (int i = 0; i < data->n_enterprises; i++) 
        fprintf(stats_file, "Enterprise %d executed %d operation(s)!\n", i, data->enterprise_stats[i]);
}

void write_ops_statistics(struct operation* ops, int num_ops) {
    fprintf(stats_file, "\nOperation Statistics:\n");

    fprintf(stats_file, "Request Statistics:\n");

    for (int i = 0; i < num_ops; i++) {
        struct operation* op = &ops[i];

        char start_time_str[100];
        strftime(start_time_str, sizeof(start_time_str), "%Y-%m-%d %H:%M:%S", localtime(&(op->start_time.tv_sec)));

        char client_time_str[100];
        strftime(client_time_str, sizeof(client_time_str), "%Y-%m-%d %H:%M:%S", localtime(&(op->client_time.tv_sec)));

        char intermediary_time_str[100];
        strftime(intermediary_time_str, sizeof(intermediary_time_str), "%Y-%m-%d %H:%M:%S", localtime(&(op->intermed_time.tv_sec)));

        char enterprise_time_str[100];
        strftime(enterprise_time_str, sizeof(enterprise_time_str), "%Y-%m-%d %H:%M:%S", localtime(&(op->enterp_time.tv_sec)));

        double total_time = calculate_total_time(op->start_time, op->enterp_time);

        fprintf(stats_file, "Request: %d\n", op->id);
        fprintf(stats_file, "Status: %c\n", op->status);
        fprintf(stats_file, "Intermediary id: %d\n", op->receiving_interm);
        fprintf(stats_file, "Enterprise id: %d\n", op->requested_enterp);
        fprintf(stats_file, "Client id: %d\n", op->requesting_client);
        fprintf(stats_file, "Start time: %s.%03ld\n", start_time_str, op->start_time.tv_nsec / 1000000);
        fprintf(stats_file, "Client time: %s.%03ld\n", client_time_str, op->client_time.tv_nsec / 1000000);
        fprintf(stats_file, "Intermediary time: %s.%03ld\n", intermediary_time_str, op->intermed_time.tv_nsec / 1000000);
        fprintf(stats_file, "Enterprise time: %s.%03ld\n", enterprise_time_str, op->enterp_time.tv_nsec / 1000000);
        fprintf(stats_file, "Total time: %.3f\n\n", total_time);

    }
}

double calculate_total_time(struct timespec start_time, struct timespec end_time) {
    double start_time_seconds = start_time.tv_sec + start_time.tv_nsec / 1000000000.0;
    double end_time_seconds = end_time.tv_sec + end_time.tv_nsec / 1000000000.0;

    return end_time_seconds - start_time_seconds;
}

