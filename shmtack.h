#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <assert.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#define PERMISSIONS 0666
#define KEYPATH "/tmp/key_file"

typedef int ElemT;

struct Stack {

    unsigned int maxElems;
    ElemT* elems;
    int elemCount;
    void* shm;
};

void stackContruct(struct Stack* stk, unsigned int maxELements);

void stackPush(struct Stack* stk, ElemT newElem);

int stackPop(struct Stack* stk);

void stackPrint(struct Stack* stk);

void stackDestruct(struct Stack* stk);	

/*int main() {

    struct Stack stk;
    stackContruct(&stk, MAXELEMENTS);

    for (int i = 0; i < 5; i++) {
        int new;
        scanf("%d", &new);
        stackPush(&stk, new);

    }
    stackPrint(&stk);
    printf("\n");

    printf("%d\n", stackPop(&stk));
    printf("%d", stk.elemCount);

    stackDestruct(&stk);

    return 0;
}*/

void stackCreate(struct Stack* stk, unsigned int maxELements) {
	int fd = 0;
	assert(stk);

    fd = open(KEYPATH, O_CREAT | PERMISSIONS);

    key_t key = ftok(KEYPATH, 0);
  	if (key == -1) {
  		perror("");
  		exit(-1);
  	}

    int id = shmget(key, sizeof(ElemT) * maxELements + sizeof(int), IPC_CREAT | PERMISSIONS);
    if (id == -1) {
    	perror("");
    	exit(-1);
    }
    stk->shm = shmat(id, NULL, 0);
    stk->elems = (ElemT*)(stk->shm + sizeof(int));

    memset(stk->shm, 0, sizeof(ElemT) * maxELements + sizeof(int));

    stk->maxElems = maxELements;	
    stk->elemCount = 0;
}

void stackConstruct(struct Stack* stk, unsigned int maxELements) {
	assert(stk);
    stk->maxElems = maxELements;
    key_t key = ftok(KEYPATH, 0);
    int id = shmget(key, sizeof(ElemT) * maxELements + sizeof(int), IPC_CREAT | PERMISSIONS);
    if (id == -1) {
    	perror("");
    	exit(-1);
    }

    stk->shm = shmat(id, NULL, 0);
    stk->elems = (ElemT*)(stk->shm + sizeof(int));
    stk->elemCount = *((int*) shmat(id, NULL, 0));
}

void stackPush(struct Stack* stk, ElemT newElem) {

    stk->elems[stk->elemCount++] = newElem;
    *((int*) stk->shm) = stk->elemCount;

}

int stackPop(struct Stack* stk) {

    ElemT result = stk->elems[stk->elemCount-- - 1];
    *((int*) (stk->elems - sizeof(int))) = stk->elemCount;
    return result;
}

void stackPrint(struct Stack* stk) {

    for (int i = 0; i < stk->elemCount; i++) {
        printf("%d\n", stk->elems[i]);
    }

}

void stackDestruct(struct Stack* stk) {

    shmdt(stk->elems - sizeof(int));
}