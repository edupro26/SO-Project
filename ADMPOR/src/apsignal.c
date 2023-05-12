#include <signal.h>
#include "main.h"
#include "apsignal.h"

struct main_data *data_pointer;
struct comm_buffers *buffers_pointer;
struct semaphores *sems_pointer;

void sigint_handler(int sig_num) {
    // Invocar stop_execution quando SIGINT é recebido
    if (sig_num == SIGINT) {
        stop_execution(data_pointer, buffers_pointer, sems_pointer);
    }
}

void ctrlC(struct main_data* data, struct comm_buffers* buffers, struct semaphores* sems) {
    // Guardar os ponteiros para as estruturas de dados globais
    data_pointer = data;
    buffers_pointer = buffers;
    sems_pointer = sems;


    // Configurar o sinal SIGINT para ser tratado pela função sigint_handler
    signal(SIGINT, sigint_handler);
}