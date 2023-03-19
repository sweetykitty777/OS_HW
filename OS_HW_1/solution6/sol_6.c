#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <stdlib.h>

int count_result(char str[]) {
    int i = 0;
    int counter = 0;
    while (str[i] != '\0') {
        if((str[i] >= 'A' && str[i] <= 'Z') || (str[i] >= 'a' && str[i] <= 'z') || (str[i] <= '9' && str[i] >= '0')) {
            counter++;
        }
        ++i;
    }
    return counter;
}

int open_file(char file_name[]) {
    int fd;
    if((fd = open(file_name, O_RDWR | O_CREAT, 0666)) < 0) {
        printf("Error\n");
        exit(-1);
    }
    return fd;
}

int main(int argc, char **argv) {
    if (argc != 3) {
        printf("wrong input\n");
        return 0;
    }

    int fd1[2], fd2[2];
    pid_t pid1, pid2;
    pipe(fd1);
    pipe(fd2);
    if (fork() == 0) {
        char  buffer[5000];
        close(fd1[0]);
        int fd = open_file(argv[1]);
        read(fd, buffer, 5000);
        close(fd);
        write(fd1[1], buffer, strlen(buffer));
        close(fd2[1]);
        char  buffer3[5000];
        read(fd2[0], buffer3, 5000);
        int fd_out = open_file(argv[2]);
        printf("answer: %s\n", buffer3);
        write(fd_out, buffer3, strlen(buffer3));
        close(fd_out);
        exit(0);
    }
    pid2 = fork();
    if (pid2 == 0) {
        close(fd1[1]);
        close(fd2[0]);
        char buffer2[5000];
        read(fd1[0], buffer2, 5000);
        int ans = count_result(buffer2);
        char res[300];
        sprintf(res, "%d", ans);
        write(fd2[1], res, strlen(res));
        exit(0);
    }

    return 0;
}

