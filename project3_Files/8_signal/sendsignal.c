#include <stdio.h>
#include <sys/types.h>
#include <signal.h>

int i = 0; // 핸들러 호출 횟수

// 핸들러 선언
void p_handler(int); // 부모 핸들러
void c_handler(int); // 자식 핸들러

int main()
{
    pid_t pid, ppid;
    struct sigaction act;

    pid = fork(); // 부모-자식 프로세스 생성

    if (pid == 0)
    {                                   // 자식 프로세스
        act.sa_handler = c_handler;     // 자식 핸들러 설정
        sigaction(SIGUSR1, &act, NULL); // SIGUSR1 신호 등록
        ppid = getppid();               // 부모의 PID 가져오기

        while (1)
        {                        // 무한 루프
            sleep(1);            // 1초 대기
            kill(ppid, SIGUSR1); // 부모에게 SIGUSR1 신호 보냄
            pause();             // 부모의 응답 대기
        }
    }
    else if (pid > 0)
    {                                   // 부모 프로세스
        act.sa_handler = p_handler;     // 부모 핸들러 설정
        sigaction(SIGUSR1, &act, NULL); // SIGUSR1 신호 등록

        while (1)
        {                       // 무한 루프
            pause();            // 자식 신호 대기
            sleep(1);           // 1초 대기
            kill(pid, SIGUSR1); // 자식에게 SIGUSR1 신호 보냄
        }
    }
    else
    {
        perror("Error"); // fork 실패 시 오류 출력
    }
}

// 부모 핸들러
void p_handler(int signo)
{
    printf("Parent handler: call %d times.\n", ++i);
}

// 자식 핸들러
void c_handler(int signo)
{
    printf("Child handler: call %d times.\n", ++i);
}
