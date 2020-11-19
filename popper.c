#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <assert.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include "shmtack.h"

int main() {
	int x = 0;
	struct Stack stk;
	stackConstruct(&stk, 100);

	x = stackPop(&stk);
	printf("x=%d\n", x);

	stackDestruct(&stk);
	return 0;
}