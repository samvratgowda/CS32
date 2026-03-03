#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>

int main(int argc, char **argv) {
    if (argc != 2) {
        fprintf(stderr, "Usage: parentcreates <iterations>\n");
        exit(1);
    }

    int iterations = strtol(argv[1], NULL, 10);

    for (int i = 0; i < iterations; i++) {
        int n = fork();
        if (n < 0) {
            perror("fork");
            exit(1);
        }
        printf("ppid = %d, pid = %d, i = %d\n", getppid(), getpid(), i);
        if (n == 0) {
            break;  // child exits loop, only parent keeps forking
        }
    }

    for (int i = 0; i < iterations; i++) {
        wait(NULL);
    }

    return 0;
}
