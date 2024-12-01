#include <stdio.h>
#include <stdlib.h>

#define MSGSIZE 16

int main() {
    char buf[MSGSIZE];
    int p[2], i;
    int pid;

    if (pipe(p) == -1) {
        perror("pipe call failed");
        exit(1);
    }

    pid = fork();

    if (pid == 0) { // 자식 프로세스
        close(p[0]); // 읽기 끝 닫기
        char buf1[MSGSIZE], buf2[MSGSIZE];

        sprintf(buf1, "Hello, world #1");
        sprintf(buf2, "Hello, world #2");

        write(p[1], buf1, MSGSIZE);
        write(p[1], buf2, MSGSIZE);

        close(p[1]); // 쓰기 끝 닫기
    } else if (pid > 0) { // 부모 프로세스
        close(p[1]); // 쓰기 끝 닫기

        for (i = 0; i < 2; i++) {
            if (read(p[0], buf, MSGSIZE) > 0) {
                printf("%s\n", buf);
            } else {
                perror("read failed");
            }
        }

        close(p[0]); // 읽기 끝 닫기
    } else {
        perror("fork failed");
    }

    return 0;
}
