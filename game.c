// game.c: 처리 담당(엔진)
// shell에서 받은 명령 -> game -> 처리
// 명령어를 받아서 실제 동작 수행

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>

int main() {
    char cmd[100];

    while (1) {
        // shell에서 전달받는 입력 
        fgets(cmd, sizeof(cmd), stdin);

        // 개행 제거
        cmd[strcspn(cmd, "\n")] = 0;

        // 종료 조건
        if (strcmp(cmd, "exit") == 0){
            printf("Game Exit\n");
            break;
        }

        // 명령어 처리
        if (strcmp(cmd, "start") == 0){
            printf("[GAME] Start\n");
        }
        else if (strcmp(cmd, "left") == 0) {
            printf("[GAME] Move Left\n");
        }
        else if (strcmp(cmd, "right") == 0) {
            printf("[GAME] Move Right\n");
        }
        else if (strcmp(cmd, "down") == 0) {
            printf("[GAME] Move Down\n");
        }
        else if (strcmp(cmd, "rotate") == 0) {
            printf("[GAME] Rotate Block\n");
        }
        else if (strcmp(cmd, "status") == 0) {
            printf("[GAME] Status OK\n");
        }
        else {
            printf("[GAME] Unknown command\n");
        }
    }
    return 0;
}