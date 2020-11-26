#include "shmtack.h"

void stackConstruct(struct Stack* stk, unsigned int maxELements) {
	assert(stk);

    int id = 0;
    int fd = 0;
    struct sembuf semops;

    stk->maxElems = maxELements;

    fd = open(KEYPATH, O_CREAT);
    key_t key = ftok(KEYPATH, 0);
    id = shmget(key, sizeof(ElemT) * maxELements + sizeof(int), IPC_CREAT | PERMISSIONS);
    if (id == -1) {
    	perror("");
        printf("Error getting shared memory\n");
    	exit(-1);
    }
    stk->semid = semget(key, 2, IPC_CREAT | PERMISSIONS);
    if (stk->semid == -1) {
        perror("");
        printf("Error getting semaphores\n");
        exit(-1);
    }

    stk->shm = shmat(id, NULL, 0);
    stk->elems = (ElemT*)(stk->shm + sizeof(int));
    stk->maxElems = maxELements;

    if (semctl(stk->semid, 0, GETVAL) == 0) {
        semops.sem_num = 0;
        semops.sem_op = 1;
        semops.sem_flg = 0;
        if (semop(stk->semid, &semops, 1) == -1) {
            perror("");
            printf("Error opping semaphores\n");
            exit(-1);
        }
        memset(stk->shm, 0, sizeof(ElemT) * maxELements + sizeof(int));
        stk->elemCount = 0;

        if (semctl(stk->semid, 1, SETVAL, 1) == -1) {
            perror("");
            printf("Error initializing semaphore\n");
            exit(-1);
        }

    } else {
        stk->elemCount = *((int*) stk->shm);
    }
}

void stackPush(struct Stack* stk, ElemT newElem) {
    struct sembuf semops;

    semops.sem_num = 1;
    semops.sem_op = -1;
    semops.sem_flg = SEM_UNDO;
    if (semop(stk->semid, &semops, 1) == -1) {
        perror("");
        printf("Error opping semaphores\n");
        exit(-1);
    }
    if (stk->elemCount >= stk->maxElems) {
        printf("Too many elements in the stack\n");
        return ;
    }

    stk->elems[stk->elemCount++] = newElem;
    *((int*) stk->shm) = stk->elemCount;

    semops.sem_num = 1;
    semops.sem_op = 1;
    semops.sem_flg = SEM_UNDO;
    if (semop(stk->semid, &semops, 1) == -1) {
        perror("");
        printf("Error opping semaphores\n");
        exit(-1);
    }
}

int stackPop(struct Stack* stk) {
    struct sembuf semops;

    semops.sem_num = 1;
    semops.sem_op = -1;
    semops.sem_flg = SEM_UNDO;

    if (semop(stk->semid, &semops, 1) == -1) {
        perror("");
        printf("Error opping semaphores\n");
        exit(-1);
    }

    if (stk->elemCount <= 0) {
        printf("No elements in the stack\n");
        return -1;
    }

    ElemT result = stk->elems[stk->elemCount-- - 1];
    *((int*) stk->shm) = stk->elemCount;

    semops.sem_num = 1;
    semops.sem_op = 1;
    semops.sem_flg = SEM_UNDO;

    if (semop(stk->semid, &semops, 1) == -1) {
        perror("");
        printf("Error opping semaphores\n");
        exit(-1);
    }

    return result;
}

void stackPrint(struct Stack* stk) {

    for (int i = 0; i < stk->elemCount; i++) {
        printf("%d\n", stk->elems[i]);
    }

}

void stackDestruct(struct Stack* stk) {
    struct sembuf semops;

    shmdt(stk->shm);
    semops.sem_num = 0;
    semops.sem_op = -1;
    semops.sem_flg = 0;

    if (semop(stk->semid, &semops, 1) == -1) {
        perror("");
        printf("Error opping semaphores\n");
        exit(-1);
    }
    if (semctl(stk->semid, 0, IPC_RMID) == -1) {
        perror("");
        printf("Error deleting semaphores\n");
        exit(-1);
    }
}