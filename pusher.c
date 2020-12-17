#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <assert.h>
#include <sys/ipc.h>
#include <unistd.h>
#include <sys/shm.h>
#include "shmtack.h"

int main() {
	struct Stack stk;
	stackConstruct(&stk, 11);

	for (int i = 0; i < 10; ++i) {
		stackPush(&stk, i);
	}

	return 0;
}