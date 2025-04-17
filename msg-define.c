#include <errno.h>
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
        hash = ((hash << 5) + hash) + c;
    }

    return hash;
}

key_t get_msq_key(void)
{
    char str[MAX_TEXT_LEN];
    FILE *fp;

    // Can also read the process name from /proc/self/comm if used in only one process
    fp = fopen("/etc/hostname", "r");
    if (fp == NULL) {
        perror("Error opening /etc/hostname");
        exit(1);
    }
    
    // Get system hostname
    fgets(str, MAX_TEXT_LEN, fp);
    fclose(fp);

    // Remove the newline character that fgets() may add
    str[strcspn(str, "\n")] = 0;

    // Generate a simple hash
    unsigned long hash_value = simple_hash(str);

    // Ensure the key is positive
    if (hash_value <= 0) {
        hash_value = -hash_value;
    }

    // Limit the key to fit within the key_t range == 32-bit signed integer
    key_t key = (key_t)(hash_value); //( & 0x7FFFFFFF);

    return key;
}

int alloc_message_queue(void)
{
    int msqid;
    key_t key = get_msq_key();

    msqid = msgget(key, IPC_CREAT | S_IRUSR | S_IWUSR | IPC_EXCL | IPC_NOWAIT) ;

    if (msqid == -1) {
        perror("msgget failed: ");
        exit(EXIT_FAILURE);
    }

    return msqid;
}

int get_message_queue(void)
{
    int msqid;
    key_t key = get_msq_key();

    msqid = msgget(key, IPC_CREAT | IPC_EXCL | IPC_NOWAIT | S_IRUSR | S_IWUSR);

    if (msqid == -1) {
        if (errno == EEXIST) {
            // Message queue already exists so just retrieve the ID
            msqid = msgget(key, S_IRUSR | S_IWUSR);
            if (msqid == -1) {
                perror("msgget failed: ");
                exit(EXIT_FAILURE);
            }
        } else {
            perror("msgget failed: ");
            exit(EXIT_FAILURE);
        }
    }

    return msqid;
}

void remove_message_queue(int msqid)
{
    if (msgctl(msqid, IPC_RMID, NULL) == -1) {
        perror("msgget failed: ");
        exit(EXIT_FAILURE);
    }
}
