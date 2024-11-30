#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
    

    pid_t pid;

    pid = fork(); // 자식 프로세스(자식 쉘) 생성

    if (pid == 0) // 자식 프로세스가 명령 실행
    {
        execvp(argv[1], &argv[1]);
        // 인자를 여러개 받을 수 있는 execvp로 명령 수행
        perror("명령어 실행 오류.");
    }
    else if (pid > 0) // 부모 프로세스(쉘) wait(동기화)
    {
        // 명령어 수행을 기다림
        wait((int *)0);
        exit(0);
    }
    else // fork 실패
    {
        perror("fork 실패");
    }
}