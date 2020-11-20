#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <dirent.h>

int main(int argc, char * argv[]) {
	DIR * dir = opendir(argv[1]);
	struct dirent * curfile;
	while ((curfile = readdir(dir)) != NULL) {
		printf("%s\n", curfile->d_name);
	}
	return 0;
}