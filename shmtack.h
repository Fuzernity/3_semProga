#pragma once

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
#include <sys/sem.h>

#define PERMISSIONS 0666
#define KEYPATH "/tmp/filekey"

typedef int ElemT;

struct Stack {
    unsigned int maxElems;
    ElemT* elems;
    int elemCount;
    void* shm;
    int semid;
};

void stackConstruct(struct Stack* stk, unsigned int maxELements);

void stackPush(struct Stack* stk, ElemT newElem);

int stackPop(struct Stack* stk);

void stackPrint(struct Stack* stk);

void stackDestruct(struct Stack* stk);	