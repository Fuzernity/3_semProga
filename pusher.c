#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <assert.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include "shmtack.h"

int main() {
	struct Stack stk;
	stackCreate(&stk, 100);
	stackPush(&stk, 450);

	stackDestruct(&stk);
	return 0;
}