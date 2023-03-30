#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#include "../include/process.h"
#include "../include/client.h"

int launch_client(int client_id, struct comm_buffers* buffers, struct main_data* data){
    pid_t pid = fork();
    if(pid == -1){
        perror("pid");
        exit(1);
    }
    if(pid == 0){
        int client = execute_client(client_id, buffers, data);
        exit(client);
    }
    else{
        return pid;
    }
}

int launch_interm(int interm_id, struct comm_buffers* buffers, struct main_data* data){
    //TODO
}

int launch_enterp(int enterp_id, struct comm_buffers* buffers, struct main_data* data){
    //TODO
}

int wait_process(int process_id){
    //TODO
}