#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>

#include "../include/memory.h"

void* create_shared_memory(char* name, int size) {
    int fd = shm_open(name, O_CREAT | O_EXCL | O_RDWR, S_IRUSR | S_IWUSR);
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
    void *ptr = malloc(size * sizeof(int));

    if(ptr == NULL){
        perror("malloc");
        exit(1);
    }

    memset(ptr, 0, size * sizeof(int));

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
    int in = buffer->ptrs[0]; 
    int out = buffer->ptrs[1]; 
    int next_in = (in + 1) % buffer_size;
    if (next_in == out) {
        return; 
    }

    buffer->buffer[in] = *op;

    buffer->ptrs[0] = next_in;
}

void read_main_client_buffer(struct rnd_access_buffer* buffer, int client_id, int buffer_size, struct operation* op){
    int in = buffer->ptrs[0];  
    int out = buffer->ptrs[1]; 

    for (int i = 0; i < buffer_size; i++) {
        int index = (out + i) % buffer_size; 
        if (buffer->buffer[index].id != -1 && 
            buffer->buffer[index].requesting_client == client_id) { 
            *op = buffer->buffer[index];
            buffer->ptrs[1] = (index + 1) % buffer_size;
            return; 
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

    return;
}

void read_interm_enterp_buffer(struct rnd_access_buffer* buffer, int enterp_id, int buffer_size, struct operation* op){
   int in = buffer->ptrs[0]; 
    int out = buffer->ptrs[1]; 

    for (int i = 0; i < buffer_size; i++) {
        int index = (out + i) % buffer_size; 

        if (buffer->buffer[index].id != -1 && 
            buffer->buffer[index].requested_enterp == enterp_id) { 

            *op = buffer->buffer[index];
   
            buffer->ptrs[1] = (index + 1) % buffer_size;

            return;
        }
    }
    op->id = -1;
}