#include <time.h>

struct log_item {
    timespec time;
    char *operation;
    char *argument;
};

// Initialize the log file
int log_init(char *log_file_name);

// Append a new item to the log file like 2022-2-31 14:53:30.572 status 0
int log_append(struct log_item *item);

// Close the log file
int log_close();

