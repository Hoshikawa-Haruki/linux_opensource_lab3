/*
 * 7장 파일처리
 * 파일 이름: file_size.c
 */
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <stdlib.h>  // exit 함수 사용을 위해 추가

int main(int argc, char *argv[]) 
{
    int fd;          /* file descriptor */
    off_t size;

    // 인자가 부족할 경우 사용법 출력
    if (argc < 2) {
        fprintf(stderr, "Usage: file_size filename\n");
        exit(1);
    }

    // 파일을 읽기 전용으로 열기
    if ((fd = open(argv[1], O_RDONLY)) == -1) {
        perror("open"); // errno에 대응하는 메시지 출력됨
        exit(1);
    }

    // 파일의 크기 구하기
    size = lseek(fd, 0, SEEK_END);
    if (size < 0) {
        perror("lseek");
        close(fd); // 파일 디스크립터 닫기
        exit(1);
    }

    close(fd); // 파일 디스크립터 닫기
    printf("%s's size = %ld\n", argv[1], size);
    // 파일 크기 출력

    return 0;
}
