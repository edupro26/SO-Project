#include "synchronization.h"

#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>

sem_t * semaphore_create(char* name, int value) {
    sem_t *semaphore;

    // Make sure the semaphore is not already created
    sem_unlink(name);

    // Create a new semaphore 
    semaphore = sem_open(name, O_CREAT, 0xFFFFFFFF, value);

    // Make sure the semaphore was created
    if (semaphore == SEM_FAILED) {
        perror("sem_open");
        return NULL;
    }

    return semaphore;
}

void semaphore_destroy(char* name, sem_t* semaphore) {
    // Shut down the semaphore
    if (sem_close(semaphore) == -1) {
        perror("sem_close");
    }

    // Remove the semaphore by name
    if (sem_unlink(name) == -1) {
        perror("sem_unlink");
    }
}

void produce_begin(struct prodcons* pc) {
    if (!pc) {
        return;
    }

    // Wait on the empty semaphore to ensure there is space to produce
    if (sem_wait(pc->empty) == -1) {
        perror("sem_wait on empty");
    }

    // Wait on the mutex semaphore to ensure mutual exclusion
    if (sem_wait(pc->mutex) == -1) {
        perror("sem_wait on mutex");
    }
}

void produce_end(struct prodcons* pc) {
    if (!pc) {
        return;
    }

    // Release the mutex semaphore to allow other producers or consumers to access the buffer
    if (sem_post(pc->mutex) == -1) {
        perror("sem_post on mutex");
    }

    // Signal the full semaphore to notify consumers that data has been produced
    if (sem_post(pc->full) == -1) {
        perror("sem_post on full");
    }
}

void consume_begin(struct prodcons* pc) {
    if (!pc) {
        return;
    }

    // Wait on the full semaphore to ensure there is data to consume
    if (sem_wait(pc->full) == -1) {
        perror("sem_wait on full");
    }

    // Wait on the mutex semaphore to ensure mutual exclusion during consumption
    if (sem_wait(pc->mutex) == -1) {
        perror("sem_wait on mutex");
    }
}

void consume_end(struct prodcons* pc) {
    if (!pc) {
        return;
    }

    // Release the mutex semaphore to allow other producers or consumers to access the buffer
    if (sem_post(pc->mutex) == -1) {
        perror("sem_post on mutex");
    }

    // Signal the empty semaphore to notify producers that space is available
    if (sem_post(pc->empty) == -1) {
        perror("sem_post on empty");
    }
}

void semaphore_mutex_lock(sem_t* sem) {
    if (!sem) {
        return;
    }

    // Wait on the semaphore to lock the mutex
    if (sem_wait(sem) == -1) {
        perror("sem_wait on mutex");
    }
}

void semaphore_mutex_unlock(sem_t* sem) {
    if (!sem) {
        return;
    }

    // Release the semaphore to unlock the mutex
    if (sem_post(sem) == -1) {
        perror("sem_post on mutex");
    }
}

