#pragma once

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/msg.h>
#include <sys/stat.h>

#define MAX_TEXT_LEN 32

struct message {
    long type;
    char text[MAX_TEXT_LEN];
};

int alloc_message_queue(void);
int get_message_queue(void);
void remove_message_queue(int msqid);
