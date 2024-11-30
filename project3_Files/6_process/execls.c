/* execls.c */
/* execl 로 ls 실행 */
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

int main()
{
    printf("Executing execl.\n");

    execl("/bin/ls", "ls", "-l", (char *)0);
    /* 만약execl이 복귀하면, 호출은실패.*/
    perror("execl failed to run ls");
    exit(1);
}