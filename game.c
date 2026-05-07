// game.c: 처리 담당(엔진)
// shell에서 받은 명령 -> game -> 처리
// 명령어를 받아서 실제 동작 수행

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>

#define WIDTH 10
#define HEIGHT 10

// map 초기화
char map[HEIGHT][WIDTH];
void init_map() {
    for (int i = 0; i < HEIGHT; i++){
        for (int j = 0; j < WIDTH; j++){
            map[i][j] = '.';
        }
    }
}

int x = WIDTH/2;
int y = 0;

// 맵 만들기
void draw_map() {

    printf("\033[H\033[J");

    for (int i = 0; i < HEIGHT; i++){
        for (int j = 0; j < WIDTH; j++){
            if (i == y && j == x) 
                printf("O");
            else 
                printf("%c", map[i][j]);
        }

        printf("\n");
    }

    fflush(stdout);
}


int main() {
    char cmd[100];
    
    init_map();
    draw_map();

    while (1) {

        // shell에서 전달받는 입력 
        fgets(cmd, sizeof(cmd), stdin);

        // 개행 제거
        cmd[strcspn(cmd, "\n")] = 0;

        // 종료 조건
        if (strcmp(cmd, "exit") == 0){
            // printf("Game Exit\n");
            fflush(stdout);
            break;
        }

        // 명령어 처리
        if (strcmp(cmd, "start") == 0){
            // printf("[GAME] Start\n");
        }

        // left 왼쪽 이동
        else if (strcmp(cmd, "left") == 0) {
            if ((x > 0) && (map[y][x-1] != 'X'))
                x --;
            draw_map();
            // printf("[GAME] Move Left\n");
        }

        // right 오른쪽 이동 
        else if (strcmp(cmd, "right") == 0) {
            if ((x < WIDTH - 1) && (map[y][x + 1] != 'X'))
                x ++;
            draw_map();
            // printf("[GAME] Move Right\n");
        }

        // down 밑으로 이동
        else if (strcmp(cmd, "down") == 0) {
            if ((y < HEIGHT - 1) && (map[y + 1][x] != 'X'))
                y ++;
            else {
                // O을 X로 바꿈 바닥으로 취급 
                map[y][x] = 'X';

                // 처음 위치로 원위치
                x = WIDTH /2;
                y = 0;
            }
            draw_map();
            // printf("[GAME] Move Down\n");
        }

        // rotate 회전
        else if (strcmp(cmd, "rotate") == 0) {
            // printf("[GAME] Rotate Block\n");
        }
        else if (strcmp(cmd, "status") == 0) {
            printf("[GAME] Status OK\n");
        }
        else {
            printf("[GAME] Unknown command\n");
        }

        fflush(stdout);

    }
    return 0;
}