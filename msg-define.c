#include <stdio.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include <unistd.h>

#include "msg-define.h"

unsigned long simple_hash(const char *str)
{
    unsigned long hash = 5481;
    int c;
    
    while ((c = *str++)) {
        hash = ((hash << 5) + hash) + c;  // hash * 33 + c
    }

    return hash;
}

key_t generate_unique_key(void)
{
    char process_name[MAX_PROCESS_NAME];
    FILE *fp;

    // Read the process name from /proc/self/comm (Linux-specific)
    fp = fopen("/etc/hostname", "r");
    if (fp == NULL) {
        perror("Error opening /etc/hostname");
        exit(1);
    }
    
    // Get the process name
    fgets(process_name, MAX_PROCESS_NAME, fp);
    fclose(fp);

    // Remove the newline character that fgets() may add
    process_name[strcspn(process_name, "\n")] = 0;

    // printf("process_name => %s\n", process_name);

    // Generate a simple hash
    unsigned long hash_value = simple_hash(process_name);

    // printf("hash_value => %lu\n", hash_value);
    
    // Ensure the key is positive
    if (hash_value <= 0) {
        hash_value = -hash_value;  // Make it positive if negative
    }

    // Limit the key to fit within the key_t range (32-bit signed integer)
    key_t key = (key_t)(hash_value & 0x7FFFFFFF);  // Ensure it's a valid positive key
    // key_t key = (key_t)(hash_value);

    return key;
}

int alloc_message_queue(void)
{
    int msqid;
    // Generate a unique key using the process name and PID
    key_t key = generate_unique_key();

    // printf("key => %d\n", key);

    msqid = msgget(key, IPC_CREAT | S_IRUSR | S_IWUSR | IPC_EXCL | IPC_NOWAIT) ;

    if (msqid == -1) {
        perror("msgget failed: ");
        exit(EXIT_FAILURE);
    }

    // printf("Message queue created successfully!\n");

    return msqid;
}

int get_message_queue(void)
{
    int msqid;
    // Generate a unique key using the process name and PID
    key_t key = generate_unique_key();

    msqid = msgget(key, S_IRUSR | S_IWUSR) ;

    if (msqid == -1) {
        perror("msgget failed: ");
        exit(EXIT_FAILURE);
    }

    // printf("Message queue setup successfully!\n");

    return msqid;
}

void remove_message_queue(int msqid)
{
    if(msgctl(msqid, IPC_RMID, NULL) == -1) {
        perror("msgget failed: ");
        exit(EXIT_FAILURE);
    } else {
        // printf("Message queue removed successfully!\n");
    }
}
