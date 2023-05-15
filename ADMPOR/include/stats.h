#ifndef STATS_H_GUARD
#define STATS_H_GUARD

#include "main.h"

void write_statistics_to_file(struct main_data* data, int num_ops);

void write_process_statistics(FILE *stats_file, struct main_data* data);

void write_ops_statistics(FILE *stats_file, struct operation* ops, int num_ops);

double calculate_total_time(struct timespec start_time, struct timespec end_time);

#endif