#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <stdlib.h>
#include <sys/stat.h>

#define first_fifo "./1.fifo"
#define second_fifo "./2.fifo"

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
        printf("Error");
        exit(-1);
    }
    return fd;
}

int main(int argc, char **argv) {
    if (argc != 3) {
        printf("wrong input\n");
        return 0;
    }

    char buffer[5000];
    int fd1, fd2;
    mkfifo(first_fifo, 0666);
    mkfifo(second_fifo, 0666);
    pid_t pid1, pid2, pid3;
    pid1 = fork();

    if (pid1 == 0) {
        int fd = open_file(argv[1]);
        read(fd, buffer, 5000);
        close(fd);
        fd1 = open(first_fifo, O_WRONLY, 0666);
        write(fd1, buffer, strlen(buffer));
        close(fd1);
    } else {
        pid2 = fork();
        if (pid2 == 0) {
            fd1 = open(first_fifo, O_RDONLY, 0666);
            fd2 = open(second_fifo, O_WRONLY, 0666);
            read(fd1, buffer, 5000);
            close(fd1);
            int digits = count_result(buffer);
            char res[300];
            sprintf(res, "%d", digits);
            write(fd2, res, strlen(res));
            close(fd2);
        } else {
            pid3 = fork();
            if (pid3 == 0) {
                char output[5000];
                fd1 = open(second_fifo, O_RDONLY, 0666);
                read(fd1, output, 5000);
                close(fd1);
                int fd = open_file(argv[2]);
                printf("answer: %s \n", output);
                write(fd, output, strlen(output));
                close(fd);
            }
        }
    }
    return 0;
}
