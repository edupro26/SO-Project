#ifndef APSIGNAL_H_GUARD
#define APSIGNAL_H_GUARD

#include "main.h"

/* Processa um sinal SIGINT
*/
void sigint_handler(int sig_num);

/* Verifica de o utilizador executou um CTRL-C. 
* Guarda as estruturas passadas com argumento em apontadores globais
*/
void ctrlC(struct main_data* data, struct comm_buffers* buffers, struct semaphores* sems);

/* Processa um sinal SIGALRM
*/
void sigalrm_handler(int sig_num);

/* Inicia um alarme de acordo com os segundos que constam na estrutura main_data.
* Guarda o valor do op_counter num apontador global
*/
void set_alarm(int* op_counter);

/* Imprime na consola as informações sobre todas as operações executadas pelo utilizador
*/
void print_alarm();

#endif
