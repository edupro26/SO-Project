#ifndef APTIME_H_GUARD
#define APTIME_H_GUARD

#include <time.h>

#include "memory.h"

/* Regista o instante em que um operação é criada
*/
void register_start_time(struct operation* op);

/* Regista o instante em que o cliente processa a operação
*/
void register_client_time(struct operation* op);

/* Regista o instante em que o intermediário processa a operação 
*/
void register_intermd_time(struct operation* op);

/* Regista o instante em que a empresa processa a operação 
*/
void register_enterp_time(struct operation* op);

#endif
