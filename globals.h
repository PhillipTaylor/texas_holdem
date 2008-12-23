
#ifndef GLOBALS_H
#define GLOBALS_H

//zero based index for message queues
//but unique msg queue id can't be zero
//based because it conflicts with designated
//values.
#define MSG_QUEUE_OFFSET 2

int msg_queue;
key_t key;

int *player_count;
int *table_count;
char **table_names;

#endif
