/* handlesignal.c */
/* sigaction example */
#include <stdio.h>
#include <signal.h>
#include <unistd.h>
void handler(int signo);

int main()
{
    struct sigaction act;
    int i = 0;
    act.sa_handler = handler; // SIGINT 신호를 처리할 핸들러 함수 지정
    sigfillset(&(act.sa_mask));
    // 모든 신호를 차단(mask)하여 핸들러가 실행 중에 다른 신호를 방해하지 못하도록 설정
    sigaction(SIGINT, &act, NULL);
    printf("SIGINT on\n");
    while (1)
    {
        sleep(1);
        printf("sleep for %d sec(s).\n", ++i);
    }
}

void handler(int signo)
{
    printf("handler: signo=%d\n", signo);
}