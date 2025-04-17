#include <errno.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/msg.h>
#include <sys/stat.h>

#include "msg-define.h"

int main(void) {
    int res = 0;
    int msqid = alloc_message_queue();
    struct message msg;

    msg.type = 1;
    
    for (int i = 0; i < 100; ++i) {
        snprintf(msg.text, sizeof(msg.text), "%d", i+1);
        errno = 0;
        res = msgsnd(msqid, (void *)&msg, sizeof(msg.text), IPC_NOWAIT);
        if (res == -1) {
            perror("msgsnd failed:");
            exit(EXIT_FAILURE);
        }
    }

    // sleep(20);    

    return 0;
}
