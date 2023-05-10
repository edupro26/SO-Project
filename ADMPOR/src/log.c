#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <log.h>

FILE *log_file;

int log_init(char *log_file_name) {
    
    log_file = fopen(log_file_name, "a"); // append mode to create the file if it doesn't exist
    if (log_file == NULL) {
        perror("Unable to open log file");
        return -1;
    }
    return 0;
}

int log_append(struct log_item *item) {
    if (!log_file || !item) {
        return -1;
    }

    char time_str[100];
    strftime(time_str, sizeof(time_str), "%Y-%m-%d %H:%M:%S", localtime(&(item->time.tv_sec)));

    if (fprintf(log_file, "%s.%ld %s %s\n", time_str, item->time.tv_nsec, item->operation, item->argument) < 0) {
        perror("Error writing to log file");
        return -1;
    }

    return 0;
}

int log_close() {
    if (!log_file) {
        return -1;
    }

    if (fclose(log_file) != 0) {
        perror("Error closing log file");
        return -1;
    }

    log_file = NULL;
    return 0;
}