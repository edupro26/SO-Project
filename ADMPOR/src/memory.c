/* 

Sistemas Operativos - Projeto Fase 1 
Grupo 10
Eduardo Proença  - 57551
Tiago Oliveira - 54979

*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#include "memory.h"

void* create_shared_memory(char* name, int size) {
    int fd = shm_open(name, O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);
    if (fd == -1) {
        perror("shm_open");
        exit(1);
    }
    if (ftruncate(fd, size) == -1) {
        perror("ftruncate");
        exit(1);
    }
    void* ptr = mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (ptr == MAP_FAILED) {
        perror("mmap");
        exit(1);
    }
    memset(ptr, 0, size);
    return ptr;
}

void* create_dynamic_memory(int size){
    void *ptr = malloc(size);

    if(ptr == NULL){
        perror("malloc");
        exit(1);
    }

    memset(ptr, 0, size);

    return ptr;
}

void destroy_shared_memory(char* name, void* ptr, int size){
    if (munmap(ptr, size) == -1) {
        perror("munmap");
        exit(1);
    }
    if (shm_unlink(name) == -1) {
        perror("shm_unlink");
        exit(1);
    }
}

void destroy_dynamic_memory(void* ptr){
    free(ptr);
    ptr = NULL;
}

void write_main_client_buffer(struct rnd_access_buffer* buffer, int buffer_size, struct operation* op){
    int slot = -1;
    for (int i = 0; i < buffer_size; i++) {
        if (buffer->ptrs[i] == 0) {
            slot = i;
            break;
        }
    }
    
    if (slot >= 0) {
        buffer->buffer[slot] = *op;
        buffer->ptrs[slot] = 1;
    }
}

void write_client_interm_buffer(struct circular_buffer* buffer, int buffer_size, struct operation* op){
    int next_write = (buffer->ptrs->in + 1) % buffer_size;
    
    if (next_write == buffer->ptrs->out) {
        return;
    }
   
    buffer->buffer[buffer->ptrs->in] = *op;
    buffer->ptrs->in = next_write;
}

void write_interm_enterp_buffer(struct rnd_access_buffer* buffer, int buffer_size, struct operation* op){
    int slot = -1;
    for (int i = 0; i < buffer_size; i++) {
        if (buffer->ptrs[i] == 0) {
            slot = i;
            break;
        }
    }
    
    if (slot >= 0) {
        buffer->buffer[slot] = *op;
        buffer->ptrs[slot] = 1;
    }
}

void read_main_client_buffer(struct rnd_access_buffer* buffer, int client_id, int buffer_size, struct operation* op){
    for(int i = 0; i < buffer_size; i++){
        if(buffer->ptrs[i] == 1){
            struct operation temp = buffer->buffer[i];
            if(temp.id != -1 && temp.requesting_client == client_id){
                *op = temp;
                buffer->ptrs[i] = 0;
                return;
            }
        }
    }

    op->id = -1;
}

void read_client_interm_buffer(struct circular_buffer* buffer, int buffer_size, struct operation* op){
    int in = buffer->ptrs->in; 
    int out = buffer->ptrs->out;

    if (in == out) {
        op->id = -1;
        return;
    }

    *op = buffer->buffer[out];
    buffer->ptrs->out = (out + 1) % buffer_size;
}

void read_interm_enterp_buffer(struct rnd_access_buffer* buffer, int enterp_id, int buffer_size, struct operation* op){
    for(int i = 0; i < buffer_size; i++){
        if(buffer->ptrs[i] == 1){
            struct operation temp = buffer->buffer[i];
            if(temp.id != -1 && temp.requested_enterp == enterp_id){
                *op = temp;
                buffer->ptrs[i] = 0;
                return;
            }
        }
    }
    op->id = -1;
}