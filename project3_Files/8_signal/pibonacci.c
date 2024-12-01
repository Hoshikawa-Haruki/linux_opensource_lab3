#include <stdio.h>
#include <signal.h>
#include <setjmp.h>
void start(int signo);
sigjmp_buf jbuf;
int main()
{
    struct sigaction act;
    int cur_i, past_i, tmp_i;
    if (sigsetjmp(jbuf, 1) == 0) // 현재 위치를 저장
    {
        act.sa_handler = start; // start를 핸들러로 등록
        // 시그널 호출시 핸들러 호출 -> 함수 실행
        sigaction(SIGINT, &act, NULL); // SIGINT에 대해 핸들러 등록
    }
    cur_i = past_i = 1;
    while (1)
    {
        printf("%d\n", cur_i);
        tmp_i = cur_i;
        cur_i += past_i;
        past_i = tmp_i;
        sleep(1);
    }
}
void start(int signo)
{
    fprintf(stderr, "Interrupted\n");
    siglongjmp(jbuf, 1); // sigsetjmp로 저장된 상태로 복귀
                         // siglongjmp 호출 시 반환값은 1로 설정
}