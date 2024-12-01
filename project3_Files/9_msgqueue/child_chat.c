#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <unistd.h>

#define MSG_SIZE 256

struct msg_buffer {
    long msg_type;
    char msg_text[MSG_SIZE];
};

void send_message(int msgid, long msg_type) {
    struct msg_buffer message;
    message.msg_type = msg_type;

    while (1) {
        printf("[Child]: ");
        fgets(message.msg_text, MSG_SIZE, stdin);
        message.msg_text[strcspn(message.msg_text, "\n")] = '\0';

        msgsnd(msgid, &message, sizeof(message.msg_text), 0);

        if (strcmp(message.msg_text, "exit") == 0) {
            break;
        }
    }
}

void receive_message(int msgid, long msg_type) {
    struct msg_buffer message;

    while (1) {
        msgrcv(msgid, &message, sizeof(message.msg_text), msg_type, 0);

        if (strcmp(message.msg_text, "exit") == 0) {
            printf("[Child]: 종료\n");
            break;
        }

        printf("[Parent]: %s\n", message.msg_text);
    }
}

int main() {
    key_t key;
    int msgid;

    // 키 생성
    key = ftok("chat", 65);
    if (key == -1) {
        perror("ftok");
        exit(1);
    }

    // 메시지 큐 가져오기
    msgid = msgget(key, 0666);
    if (msgid == -1) {
        perror("msgget");
        exit(1);
    }

    printf("Child Chat 시작\n");

    receive_message(msgid, 1);
    send_message(msgid, 2);

    return 0;
}
