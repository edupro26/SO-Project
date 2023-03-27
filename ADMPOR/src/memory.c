#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>

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
    //TODO
}

void destroy_dynamic_memory(void* ptr){
    free(ptr);
    ptr = NULL;
}

void write_main_client_buffer(struct rnd_access_buffer* buffer, int buffer_size, struct operation* op){
    //TODO
}

void write_client_interm_buffer(struct circular_buffer* buffer, int buffer_size, struct operation* op){
    //TODO
}

void write_interm_enterp_buffer(struct rnd_access_buffer* buffer, int buffer_size, struct operation* op){
    //TODO
}

void read_main_client_buffer(struct rnd_access_buffer* buffer, int client_id, int buffer_size, struct operation* op){
    //TODO
}

void read_client_interm_buffer(struct circular_buffer* buffer, int buffer_size, struct operation* op){
    //TODO
}

void read_interm_enterp_buffer(struct rnd_access_buffer* buffer, int enterp_id, int buffer_size, struct operation* op){
    //TODO
}