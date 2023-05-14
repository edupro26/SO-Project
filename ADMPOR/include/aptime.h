#ifndef APTIME_H_GUARD
#define APTIME_H_GUARD

#include <time.h>

#include "memory.h"

void register_start_time(struct operation* op);

void register_client_time(struct operation* op);

void register_intermd_time(struct operation* op);

void register_enterp_time(struct operation* op);

void get_local_time(struct tm* local_time);

#endif
