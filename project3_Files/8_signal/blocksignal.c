#include <stdio.h>
#include <signal.h>
#include <unistd.h>

// 신호 핸들러 함수 선언
void handler1(int); // SIGUSR1 처리 핸들러
void handler2(int); // SIGUSR2 처리 핸들러

// SIGUSR1 신호를 블록하기 위한 신호 집합
sigset_t set;

int main() {
    struct sigaction act;

    // 1. SIGUSR1 신호를 블록하는 신호 집합 초기화
    sigemptyset(&set);           // 신호 집합 초기화
    sigaddset(&set, SIGUSR1);    // SIGUSR1 신호 추가

    // 2. SIGUSR1 신호 핸들러 설정
    sigemptyset(&act.sa_mask);   // 핸들러 실행 중 블록할 신호 집합 초기화
    act.sa_flags = 0;            // 기본 플래그 설정
    act.sa_handler = handler1;   // SIGUSR1에 대해 handler1 지정
    sigaction(SIGUSR1, &act, NULL); // SIGUSR1 신호에 대해 액션 등록

    // 3. SIGUSR2 신호 핸들러 설정
    act.sa_handler = handler2;   // SIGUSR2에 대해 handler2 지정
    sigaction(SIGUSR2, &act, NULL); // SIGUSR2 신호에 대해 액션 등록

    // 4. SIGUSR1 신호를 블록
    sigprocmask(SIG_BLOCK, &set, NULL);

    // 5. 무한 루프에서 신호 처리
    while (1) {
        pause();                // 신호가 올 때까지 대기
        sigprocmask(SIG_UNBLOCK, &set, NULL); // SIGUSR1 신호 언블록
    }
}

// SIGUSR1 신호를 받을 때 호출되는 핸들러
void handler1(int sig) {
    printf("Got a SIGUSR1 signal\n");
}

// SIGUSR2 신호를 받을 때 호출되는 핸들러
void handler2(int sig) {
    printf("Got a SIGUSR2 signal\n");
}
