#ifndef LOG_H_GUARD
#define LOG_H_GUARD

#include "main.h"
#include "aptime.h"

/* Initialize the log file
*/
void log_init(struct main_data* data);

/* Writes an operation in the log file
*/
void log_operation(struct main_data* data, struct operation* op);

/* Writes a status command in the log file
*/
void log_status(struct main_data* data, int op_id);

/* Writes a string in the log file
*/
void log_append(struct main_data* data, const char* command);

#endif
