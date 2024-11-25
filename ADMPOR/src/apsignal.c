/* 

Sistemas Operativos - Projeto Fase 1 
Grupo 10
Eduardo Proença  - 57551
Tiago Oliveira - 54979

*/

#include <signal.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

#include "apsignal.h"

struct main_data *data_pointer;
struct comm_buffers *buffers_pointer;
struct semaphores *sems_pointer;
int *op_counter_sinal_pointer;
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

void sigalrm_handler(int sig_num) {
    if(sig_num == SIGALRM) {
        print_alarm();
        alarm(data_pointer->alarm_time);
        signal(SIGALRM, sigalrm_handler);
    }
}

void set_alarm(int* op_counter) {
    op_counter_sinal_pointer = op_counter;
    signal(SIGALRM, sigalrm_handler);
    alarm(data_pointer->alarm_time);
}

void print_alarm() {
    printf("\nOPERAÇÕES:\n");
    for(int i = 0; i < *op_counter_sinal_pointer; i++){
        struct operation op = data_pointer->results[i];
        switch (op.status) {
            case 'M':
                printf("op:%d status:%c start_time:%ld\n", op.id, op.status, op.start_time.tv_sec);
                break;
            case 'C':
                printf("op:%d status:%c start_time:%ld ", op.id, op.status, op.start_time.tv_sec);
                printf("client:%d client_time:%ld\n", op.receiving_client, op.client_time.tv_sec);
                break;
            case 'I':
                printf("op:%d status:%c start_time:%ld ", op.id, op.status, op.start_time.tv_sec);
                printf("client:%d client_time:%ld ", op.receiving_client, op.client_time.tv_sec);
                printf("intermediary:%d intermediary_time:%ld\n", op.receiving_interm, op.intermed_time.tv_sec);
                break;
            default:
                printf("op:%d status:%c start_time:%ld ", op.id, op.status, op.start_time.tv_sec);
                printf("client:%d client_time:%ld ", op.receiving_client, op.client_time.tv_sec);
                printf("intermediary:%d intermediary_time:%ld ", op.receiving_interm, op.intermed_time.tv_sec);
                printf("enterprise:%d enterprise_time:%ld\n", op.receiving_enterp, op.enterp_time.tv_sec);
                break;
        }
    }
    printf("Próximo alarme em %d segundos\n", data_pointer->alarm_time);
    printf("\nIntroduzir ação:\n");
}