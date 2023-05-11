#include <stdio.h>

FILE *stats_file;

void write_statistics(struct statistics* stats, struct operation* ops, int num_ops, char* filename);

void write_process_statistics(struct statistics* stats);

void write_ops_statistics(struct operation* ops, int num_ops);