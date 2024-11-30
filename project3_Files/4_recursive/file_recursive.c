#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <dirent.h>

void list_directory(const char *dirname);

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        fprintf(stderr, "Usage: %s <directory>\n", argv[0]);
        exit(1);
    }

    list_directory(argv[1]); // 디렉토리 탐색 시작

    return 0;
}

void list_directory(const char *dirname) // 디렉토리 탐색 함수
{
    DIR *pdir;
    struct dirent *pde;
    struct stat statbuf;
    char path[1024];

   
    if ((pdir = opendir(dirname)) == NULL) // 디렉토리 열기
    {
        perror("opendir");
        return;
    }

    printf("\n%s/:\n", dirname); // 현재 디렉토리 이름 출력

    while ((pde = readdir(pdir)) != NULL) // 디렉토리 내 항목 탐색
    {
        if (strcmp(pde->d_name, ".") == 0 || strcmp(pde->d_name, "..") == 0)
        { // "."와 ".." 항목 무시
            continue;
        }

        snprintf(path, sizeof(path), "%s/%s", dirname, pde->d_name); // 경로 생성

        printf("%s  ", pde->d_name); // 파일 및 디렉토리 출력
    }
    printf("\n");

    // 디렉토리 내 항목 중 하위 디렉토리 탐색
    rewinddir(pdir); // 디렉토리 스트림 위치 초기화
    while ((pde = readdir(pdir)) != NULL)
    {
        if (strcmp(pde->d_name, ".") == 0 || strcmp(pde->d_name, "..") == 0)
        {
            continue;
        }

        snprintf(path, sizeof(path), "%s/%s", dirname, pde->d_name);

        // 디렉토리인 경우 재귀 호출
        if (stat(path, &statbuf) == 0 && S_ISDIR(statbuf.st_mode))
        {
            list_directory(path);
        }
    }
    closedir(pdir);
}
