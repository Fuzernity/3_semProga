#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/inotify.h>
#include <unistd.h>
#include <assert.h>


int main(int argc, char* argv[]) {
    int in, watch, ret;
    int mask = IN_CREATE | IN_DELETE | IN_MOVE_SELF;
    char buf[4096];
    struct inotify_event* event; 

    in = inotify_init();

    if (in < 0) {
        perror("init");
        return -1;
    }

    watch = inotify_add_watch(in, argv[1], mask);
    
    if (watch < 0) {
        perror("add_watch");
        return -1;
    }

    while (1) {
        ret = read(in, buf, 4096);
        if (ret > 0) {
            event = buf;
            printf("path: %s  ", event->name);

            if (event->mask & IN_CREATE)
                printf("event: create\n");
            else if (event->mask & IN_DELETE)
                printf("event: delete\n");
            else if (event->mask & IN_MOVE_SELF)
                printf("event: move self\n");
        }
        else {
            perror("reading trouble\n");
            return -1;
        }
    }
    close(in);
    return 0;
}