#include <stdio.h>
#include <stdlib.h>
// was <sched.h>, but wouldn't compile on Amazon Linux
#include <linux/sched.h>
// for umount2()
#include <sys/mount.h>
#include <sys/wait.h>
#include <errno.h>

#define STACKSIZE (1024*1024)
static char child_stack[STACKSIZE];

struct clone_args {
        char **argv;
};

static int child_exec(void *stuff) {
        struct clone_args *args = (struct clone_args *)stuff;

        // the fprintf()s crash. Not sure why.

        // changed from umount(), lazy umount succeeds
       

        if (execvp(args->argv[0], args->argv) != 0) {
                fprintf(stderr, "failed to execvp arguments: %s\n", strerror(errno));
                exit(-1);
        }

        // unreachable
        exit(EXIT_FAILURE);
}

int main(int argc, char **argv) {
        struct clone_args args;
        args.argv = &argv[1];

        int clone_flags = CLONE_NEWUSER | SIGCHLD;
        pid_t pid = clone(child_exec, child_stack + STACKSIZE, clone_flags, &args);

        if (pid < 0) {
                fprintf(stderr, "clone failed: %s\n", strerror(errno));
                exit(EXIT_FAILURE);
        }

        if (waitpid(pid, NULL, 0) == -1) {
                fprintf(stderr, "failed to wait pid %d\n", pid);
                exit(EXIT_FAILURE);
        }

        exit(EXIT_SUCCESS);
}
