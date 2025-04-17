#pragma once

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/msg.h>
#include <sys/stat.h>

#define MAX_TEXT_LEN 32
#define MAX_PROCESS_NAME 256

struct message {
    long type;
    char text[MAX_TEXT_LEN];
};

unsigned long simple_hash(const char *str);

key_t generate_unique_key(void);

int alloc_message_queue(void);
int get_message_queue(void);
void remove_message_queue(int msqid);
