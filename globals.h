
#include <sys/ipc.h>
#include <sys/types.h>
#include <sys/sem.h>

#ifndef GLOBALS_H
#define GLOBALS_H

key_t semaphore_key;

int semaphore_id;

union semun {
    int val;              /* used for SETVAL only */
    struct semid_ds *buf; /* for IPC_STAT and IPC_SET */
    ushort *array;        /* used for GETALL and SETALL */
};

union semun sem_un;

int *table_count;
char **table_names;

#endif
