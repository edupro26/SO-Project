#ifndef CLIENT_H_GUARD
#define CLIENT_H_GUARD

#define MAX_LINE 1024

#include "main.h"

void read_input_file(char* filename, struct main_data* data);

void read_file(FILE *input, struct main_data* data);

//void save_to_main_data(char buffer, int line, struct main_data* data);

#endif
