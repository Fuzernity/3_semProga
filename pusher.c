#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <assert.h>
#include <sys/ipc.h>
#include <unistd.h>
#include <sys/shm.h>
#include "shmtack.h"

void dopush(int i) {
	struct Stack stk;
	stackConstruct(&stk, 1000);

	stackPush(&stk, i);
}

int main() {
	for (int i = 0; i < 333; i++) {
		if (fork() == 0) {
			//printf("%d\n", i);
			dopush(i);
			exit(0);
		}
	}
	return 0;
}