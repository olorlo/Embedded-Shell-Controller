// game.c: 처리 담당(엔진)
// shell에서 받은 명령 -> game -> 처리
// 명령어를 받아서 실제 동작 수행

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <pthread.h>

#define WIDTH 10
#define HEIGHT 10

pthread_mutex_t lock;

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

void* gravity(void* arg) {
    while (1) {
        sleep(1);

        pthread_mutex_lock(&lock);

        // 게임 종료 조건
        if (map[y][x] == 'X'){
            printf("GAME OVER\n");
            exit(0);
        }
        
        // 아래가 있으면 내려감
        if ((y < HEIGHT - 1) && (map[y+1][x] != 'X'))
            y++;
        // 바닥 또는 블록 충돌
        else {
            // 현재 블록 고정 
            map[y][x] = 'X';

            // 새 블록 생성 
            x = WIDTH / 2;
            y = 0;
        }
        draw_map();
    
        pthread_mutex_unlock(&lock);
    }
    printf("gravity\n");
    fflush(stdout);
}


int main() {
    char cmd[100];
    
    init_map();
    draw_map();

    pthread_t t1;

    pthread_mutex_init(&lock, NULL);

    pthread_create(&t1, NULL, gravity, NULL);

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

            // gravity thread와 main thread와 둘다 x, y, map, draw_map()에 접근 => mutex 보호
            pthread_mutex_lock(&lock);

            if ((x > 0) && (map[y][x-1] != 'X'))
                x --;
            draw_map();
            // printf("[GAME] Move Left\n");

            pthread_mutex_unlock(&lock);
        }

        // right 오른쪽 이동 
        else if (strcmp(cmd, "right") == 0) {

            // mutex 보호
            pthread_mutex_lock(&lock);
            
            if ((x < WIDTH - 1) && (map[y][x + 1] != 'X'))
                x ++;
            draw_map();
            // printf("[GAME] Move Right\n");

            pthread_mutex_unlock(&lock);
        }

        // down 밑으로 이동
        else if (strcmp(cmd, "down") == 0) {

            // mutex 보호
            pthread_mutex_lock(&lock);

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

            pthread_mutex_unlock(&lock);
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