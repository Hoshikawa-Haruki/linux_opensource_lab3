/* zombie 프로세스 실행 확인*/
#include <stdio.h>
#include <stdlib.h>
int main(void)
{
    pid_t pid;
    if ((pid = fork()) < 0) // 프로세스 생성
    {
        perror("fork");
        exit(1);
    }
    else if (pid == 0)
    { // 자식 프로세스는 바로 종료 (좀비상태)
        exit(0);
    }

    sleep(4); // 부모 프로세스 대기
    system("ps -o pid,ppid,state,tty,command");
    exit(0);
    //부모 프로세스가 종료되면 좀비 프로세스도 운영 체제에 의해 완전히 제거
}