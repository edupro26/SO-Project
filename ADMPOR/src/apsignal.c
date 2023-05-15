/* 

Sistemas Operativos - Projeto Fase 1 
Grupo 10
Eduardo Proença  - 57551
Tiago Oliveira - 54979

*/

#include <signal.h>
#include "main.h"
#include "apsignal.h"
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

struct main_data *data_pointer;
struct comm_buffers *buffers_pointer;
struct semaphores *sems_pointer;
int father_pid;

void sigint_handler(int sig_num) {
 
    // Só o pai deve invocar stop_execution
    if (getpid() != father_pid) {
        return;
    }

    // Invocar stop_execution quando SIGINT é recebido
    if (sig_num == SIGINT) {
        stop_execution(data_pointer, buffers_pointer, sems_pointer);
        exit(0);
    }
}

void ctrlC(struct main_data* data, struct comm_buffers* buffers, struct semaphores* sems) {
    // Guardar os ponteiros para as estruturas de dados globais
    data_pointer = data;
    buffers_pointer = buffers;
    sems_pointer = sems;

    father_pid = getpid();

    // Configurar o sinal SIGINT para ser tratado pela função sigint_handler
    signal(SIGINT, sigint_handler);
}