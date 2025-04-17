#include <errno.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/msg.h>
#include <sys/stat.h>

#include "msg-define.h"

int main(void) {
    ssize_t msg_size = 0;
    int msqid = get_message_queue();
    struct message msg;

    // Read all messages inthe message queue untill it's empty
    while (1) {
        errno = 0;
        msg_size = msgrcv(msqid, (void *)&msg, sizeof(msg.text), 0, MSG_NOERROR | IPC_NOWAIT);
        if (msg_size == -1) {
            if (errno == ENOMSG) {
                // No more messages in the queue
                break;
            } else {
                perror("msgrcv failed:");
                exit(EXIT_FAILURE);
            }
        }

        printf("Received message: %s \n", msg.text);
    }

    // Clean up the message queue (delete it)
    remove_message_queue(msqid);

    return 0;
}
