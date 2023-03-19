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
        printf("Error while opening file %s\n", file_name);
        exit(-1);
    }
    return fd;
}

int main(int argc, char **argv) {
    if (argc != 3) {
        printf("wrong input\n");
        return 0;
    }

    int fd1_write, fd1_read, fd2_write, fd2_read, fd1, fd2;
    mkfifo(first_fifo, 0666);
    mkfifo(second_fifo, 0666);
    
    pid_t pid1, pid2;
    
    pid1 = fork();

    if (pid1 == 0) {
        char  buffer[5000];
        int fd = open_file(argv[1]);
        read(fd, buffer, 5000);
        close(fd);
        fd1 = open(first_fifo, O_WRONLY, 0666);
        write(fd1, buffer, strlen(buffer));
        close(fd1);
        char buffer3[5000];
        fd2 = open(second_fifo, O_RDONLY, 0666);
        read(fd2, buffer3, 5000);
        close(fd2);
        int fd_out = open_file(argv[2]);
        printf("answer: %s is written in file %s\n", buffer3, argv[2]);
        write(fd_out, buffer3, strlen(buffer3));
        close(fd_out);
        exit(0);
    }
    pid2 = fork();
    if (pid2 == 0) {
        close(fd1);
        close(fd2);
        char buffer2[5000];
        fd1 = open(first_fifo, O_RDONLY, 0666);
        read(fd1, buffer2, strlen(buffer2));
        int ans = count_result(buffer2);
        char res[300];
        sprintf(res, "%d", ans);
        fd2 = open(second_fifo, O_WRONLY, 0666);
        write(fd2, res, strlen(res));
        exit(0);
    }
}


