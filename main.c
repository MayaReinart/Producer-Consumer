#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <time.h>
#include <stdlib.h>
#include <sys/wait.h>

#define PATH "./queue"

void producer(int *fd) {
    close(fd[0]);
    char text[3];
    int counter = 0;
    srand(time(NULL));
    while (counter < 10) {
        sprintf(text, "%d", rand()%100);
        if (write(fd[1], text, sizeof(text)) == -1) {
            printf("Sending failed\n");
            return;
        }
        printf("Produced %s\n", text);
        ++counter;
    }
    close(fd[1]);
}

void consumer(int *fd) {
    close(fd[1]);
    char text[3];
    int counter = 0;
    while (counter < 10) {
        if (read(fd[0], text, sizeof(text)) == -1) {
            printf("Reading failed\n");
            return;
        }
        printf("Consumed %s\n", text);
        ++counter;
    }
    close(fd[0]);
}

int main(void)
{
    int fd[2];
    srand(time(NULL));
    if (pipe(fd) == -1) {
        printf("Unable to create pipe\n");
        return -1;
    }
    if (fork()) {
        producer(fd);
        wait(0);
    } else {
        consumer(fd);
    }
    return 0;
}
