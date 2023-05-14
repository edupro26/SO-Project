#include <signal.h>
#include "main.h"
#include "apsignal.h"
#include <stdio.h>
#include <unistd.h>

struct main_data *data_pointer;
struct comm_buffers *buffers_pointer;
struct semaphores *sems_pointer;
int father_pid;

void sigint_handler(int sig_num) {
    // Make sure this is the father process
    // Get father pid

    if (getpid() != father_pid) {
        return;
    }

    // Invocar stop_execution quando SIGINT é recebido
    if (sig_num == SIGINT) {
        int pid = getpid();
        // Print pid
        printf("PID: %d\n", pid);
        printf("SIGINT received\n");
        //stop_execution(data_pointer, buffers_pointer, sems_pointer);
    }
}

void ctrlC(struct main_data* data, struct comm_buffers* buffers, struct semaphores* sems) {
    // Guardar os ponteiros para as estruturas de dados globais
    data_pointer = data;
    buffers_pointer = buffers;
    sems_pointer = sems;

    int pid = getpid();
    // Print pid
    printf("PID on register: %d\n", pid);

    father_pid = getpid();

    // Configurar o sinal SIGINT para ser tratado pela função sigint_handler
    signal(SIGINT, sigint_handler);
}