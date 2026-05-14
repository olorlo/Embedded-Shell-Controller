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

// int x = WIDTH/2;
// int y = 0;

// ----------------
// 구조체 만들기
typedef struct {
    int shape[4][4];
    int x;
    int y;

} Block;

Block current;

// ----------------

// 맵 만들기
void draw_map() {

    printf("\033[H\033[J");

    for (int i = 0; i < HEIGHT; i++){
        for (int j = 0; j < WIDTH; j++){
            int printed = 0;

            // 현재 블록 검사
            for (int r=0;r<4;r++){
                for (int c=0;c<4;c++){
                    if (current.shape[r][c]) {
                        int drawY = current.y + r;
                        int drawX = current.x + c;
                        if (i == drawY && j == drawX){
                            printf("O");
                            printed = 1;
                        }
                    }
                }
            }
            if (!printed)
                printf("%c", map[i][j]);
        }

        printf("\n");
    }

    fflush(stdout);
}




// 블록 관련된 함수들 ----------------
// 블록 생성 
void spawn_block() {
    
    // shape 초기화
    for (int i=0;i<4;i++){
        for (int j=0;j<4;j++){
            current.shape[i][j] = 0;
        }
    }
    
    current.shape[0][0] = 1;
    current.shape[0][1] = 1;
    current.shape[1][0] = 1;
    current.shape[1][1] = 1;
    
    // 출발 지점 설정
    current.x = WIDTH / 2;
    current.y = 0;
    
}

// 블록 경계 처리
// newX, newY로 갈 수 있는 지 확인 
// 갈 수 있다면 1
// 못 간다면 0
int can_move(int newX, int newY) {
    for (int r=0;r<4;r++){
        for (int c =0;c<4;c++){
            
            // 블록 칸인 경우만 검사
            if (current.shape[r][c]) {
                int nextY = newY + r;
                int nextX = newX + c;
                
                // 왼쪽 / 오른쪽 벽
                if (nextX < 0 || nextX >= WIDTH)
                    return 0;
            
                // 바닥
                if (nextY >= HEIGHT)
                    return 0;
        
                // 고정 블록 충돌
                if (map[nextY][nextX] == 'X')
                    return 0;
            }
        }
    }
    return 1;
}

// 블록이 바닥에 닿았을 때
void fix_block() {
    for (int r =0;r<4;r++){
        for (int c =0;c<4; c++){
            if (current.shape[r][c]) {
                int mapY = current.y + r;
                int mapX = current.x + c;

                map[mapY][mapX] = 'X';
            } 
        }
    }
}

// ----------------

// 중력 처리
void* gravity(void* arg) {
    while (1) {
        sleep(1);

        pthread_mutex_lock(&lock);

        // 게임 종료 조건
        // if (map[current.y][current.x] == 'X'){
        //     printf("GAME OVER\n");
        //     exit(0);
        // }
        
        // 아래가 있으면 내려감
        if (can_move(current.x, current.y + 1))
            current.y++;
        // 바닥 또는 블록 충돌
        else {
            // 현재 블록 고정 
            fix_block();
            spawn_block();

            if (!can_move(current.x, current.y)) {
                printf("GAME OVER\n");
                exit(0);
            }
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
    spawn_block();
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

            // 왼쪽으로 갈 수 있다면 이동 
            if (can_move(current.x -1, current.y))
                current.x --;
            draw_map();
            // printf("[GAME] Move Left\n");

            pthread_mutex_unlock(&lock);
        }

        // right 오른쪽 이동 
        else if (strcmp(cmd, "right") == 0) {

            // mutex 보호
            pthread_mutex_lock(&lock);
            
            // 오른쪽으로 갈 수 있다면 이동
            if (can_move(current.x+1, current.y))
                current.x ++;
            draw_map();
            // printf("[GAME] Move Right\n");

            pthread_mutex_unlock(&lock);
        }

        // down 밑으로 이동
        else if (strcmp(cmd, "down") == 0) {

            // mutex 보호
            pthread_mutex_lock(&lock);

            // 아래쪽으로 갈 수 있다면 이동
            if (can_move(current.x, current.y+1))
                current.y ++;
            else {
                // O을 X로 바꿈 바닥으로 취급 -> 한칸 기준일 때
                // map[current.y][current.x] = 'X';

                // 여러칸일 때
                fix_block();
                spawn_block();

                if (!can_move(current.x, current.y)){
                    printf("GAME OVER\n");
                    exit(0);
                }
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