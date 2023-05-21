/* 

Sistemas Operativos - Projeto Fase 1 
Grupo 10
Eduardo Proença  - 57551
Tiago Oliveira - 54979

*/

#include "synchronization.h"

#include <stdio.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>

sem_t * semaphore_create(char* name, int value) {
    sem_t *semaphore;

    sem_unlink(name);

    semaphore = sem_open(name, O_CREAT, 0xFFFFFFFF, value);

    if (semaphore == SEM_FAILED) {
        perror("sem_open");
        return NULL;
    }

    return semaphore;
}

void semaphore_destroy(char* name, sem_t* semaphore) {
    if (sem_close(semaphore) == -1) {
        perror("sem_close");
    }

    if (sem_unlink(name) == -1) {
        perror("sem_unlink");
    }
}

void produce_begin(struct prodcons* pc) {
    if (!pc) {
        return;
    }

    if (sem_wait(pc->empty) == -1) {
        perror("sem_wait on empty");
    }

    if (sem_wait(pc->mutex) == -1) {
        perror("sem_wait on mutex");
    }
}

void produce_end(struct prodcons* pc) {
    if (!pc) {
        return;
    }

    if (sem_post(pc->mutex) == -1) {
        perror("sem_post on mutex");
    }

    if (sem_post(pc->full) == -1) {
        perror("sem_post on full");
    }
}

void consume_begin(struct prodcons* pc) {
    if (!pc) {
        return;
    }

    if (sem_wait(pc->full) == -1) {
        perror("sem_wait on full");
    }

    if (sem_wait(pc->mutex) == -1) {
        perror("sem_wait on mutex");
    }
}

void consume_end(struct prodcons* pc) {
    if (!pc) {
        return;
    }

    if (sem_post(pc->mutex) == -1) {
        perror("sem_post on mutex");
    }

    if (sem_post(pc->empty) == -1) {
        perror("sem_post on empty");
    }
}

void semaphore_mutex_lock(sem_t* sem) {
    if (!sem) {
        return;
    }

    if (sem_wait(sem) == -1) {
        perror("sem_wait on mutex");
    }
}

void semaphore_mutex_unlock(sem_t* sem) {
    if (!sem) {
        return;
    }

    if (sem_post(sem) == -1) {
        perror("sem_post on mutex");
    }
}

