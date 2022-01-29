#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <errno.h>

int main(int argc, char *argv[])
{
    pid_t pid;

    pid = fork();

    printf("CHECK 1");

    if (pid == -1)
    {
        printf("error");
        fprintf(stderr, "fork failed, error");
        exit(EXIT_FAILURE);
    }
    else if (pid == 0)
    {
        for (int i = 0; i < 10; i++)
        {
            printf("child : %d\n", i);
            sleep(1);
        }
        _exit(0);
    }
    else if (pid > 0)
    {
        for (int i = 0; i < 10; i++)
        {
            printf("parent : %d\n", i);
            sleep(1);
        }
        _exit(0);
    }
}