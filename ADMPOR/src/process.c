/* 

Sistemas Operativos - Projeto Fase 1 
Grupo 10
Diogo Peixoto - 53365
Eduardo Proença  - 57551
Tiago Oliveira - 54979

*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#include "process.h"
#include "client.h"
#include "intermediary.h"
#include "enterprise.h"

int launch_client(int client_id, struct comm_buffers* buffers, struct main_data* data, struct semaphores* sems) {
    pid_t pid = fork();
    if(pid == -1){
        perror("launch error");
        exit(1);
    }
    if(pid == 0){
        int client = execute_client(client_id, buffers, data, sems);
        exit(client);
    }
    else{
        return pid;
    }
}

int launch_interm(int interm_id, struct comm_buffers* buffers, struct main_data* data, struct semaphores* sems) {
    pid_t pid = fork();
    if(pid == -1){
        perror("launch error");
        exit(1);
    }
    if(pid == 0){
        int interm = execute_intermediary(interm_id, buffers, data, sems);
        exit(interm);
    }
    else{
        return pid;
    }
}

int launch_enterp(int enterp_id, struct comm_buffers* buffers, struct main_data* data, struct semaphores* sems) {
    pid_t pid = fork();
    if(pid == -1){
        perror("launch error");
        exit(1);
    }
    if(pid == 0){
        int enterp = execute_enterprise(enterp_id, buffers, data, sems);
        exit(enterp);
    }
    else{
        return pid;
    }
}

int wait_process(int process_id){
    int status;
    pid_t w_pid = waitpid(process_id, &status, 0);

    if(w_pid == -1){
        perror("waitpid");
        exit(1);
    }

    if(WIFEXITED(status)){
        int exit_status = WEXITSTATUS(status);
        return exit_status;
    }

    return -1;
}