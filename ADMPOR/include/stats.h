#ifndef STATS_H_GUARD
#define STATS_H_GUARD

#include "main.h"

/* Guarda o valor do op_counter num apontador global
*/
void load_opcounter_stats(int *op_counter);

/* Escreve as estatísticas finais num ficheiro de texto com o nome definido na estrutura main_data
*/
void write_statistics_to_file(struct main_data* data);

/* Escreve as estatísticas finais dos clientes, intermediários e empresas
*/
void write_process_statistics(FILE *stats_file, struct main_data* data);

/* Escreve as estatísticas finais de todas as operações
*/
void write_ops_statistics(FILE *stats_file, struct operation* ops);

/* Calcula o tempo total entre o instante em que uma
* operação é criada até que a mesma é processada pela empresa
*/
double calculate_total_time(struct timespec start_time, struct timespec end_time);

#endif