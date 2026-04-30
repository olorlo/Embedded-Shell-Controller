// shell.c: 입력 담당(컨트롤러)
// 사용자 -> shell -> 명령 전달 
// 다른 프로그램으로 보내는 역할

#include <stdio.h>
#include <string.h>

void print_prompt() {
    printf("> ");
}

void execute_command(char *cmd) {

    if (strcmp(cmd, "start") == 0) {
        printf("Game Start\n");
    }
    else if (strcmp(cmd, "left") == 0) {
        printf("Move Left\n");
    }
    else if (strcmp(cmd, "right") == 0) {
        printf("Move Right\n");
    }
    else if (strcmp(cmd, "down") == 0) {
        printf("Move Down\n");
    }
    else if (strcmp(cmd, "rotate") == 0) {
        printf("Rotate Block\n");
    }
    else if (strcmp(cmd, "status") == 0) {
        printf("Game Status\n");
    }
    else if (strcmp(cmd, "exit") == 0) {
        printf("Exit\n");
    }
    else {
        printf("Unknown command\n");
    }
}

int main() {
    char input[100];

    while (1) {
        print_prompt();

        // 입력 받기
        fgets(input, sizeof(input), stdin);

        // 개행 제거
        input[strcspn(input, "\n")] = 0;

        // exit 처리
        if (strcmp(input, "exit") == 0) {
            break;
        }

        // 명령 실행
        execute_command(input);
    }

    return 0;
}