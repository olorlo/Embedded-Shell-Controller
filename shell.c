// shell.c: 입력 담당(컨트롤러)
// 사용자 -> shell -> 명령 전달 
// 다른 프로그램으로 보내는 역할

// pthread 버전
// shell 역할: 키보드 입력만 담당

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <pthread.h>
#include <termios.h>

int to_game[2];   // shell -> game
int from_game[2]; //  game -> shell

void print_prompt() {
    printf("> ");
    fflush(stdout);
}

void* read_game_output(void* arg) {
    char buffer[1000000];

    while (1){
        int n = read(from_game[0], buffer, sizeof(buffer)-1);

        if (n>0){
            buffer[n] = 0;
            printf("%s", buffer);
            fflush(stdout);
        }
    }
    return NULL;
}

int main() {
    // 입력 관련 ------------
    struct termios oldt, newt;

    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    // ---------------------

    char input[100];

    // fd[0]: read, fd[1]: write
    pipe(to_game); 
    pipe(from_game);

    pid_t pid = fork();

    if (pid == 0) {
        // 자식 프로세스 (game 실행)

        // dup2(fd[0], STDIN_FILENO); // pipe -> stdin 연결
        // close(fd[1]);

        dup2(to_game[0], STDIN_FILENO);
        dup2(from_game[1], STDOUT_FILENO);

        close(to_game[1]);   // 쓰기쪽 필요 없음
        close(from_game[0]); // 읽기쪽 필요 없음

        execl("./game", "game", NULL);
    }
    else {
        // 부모 프로세스 (shell)
        // close(fd[0]);

        close(to_game[0]);    // 읽기 필요 없음
        close(from_game[1]);  // 쓰기 필요 없음
        
        pthread_t output_thread;

        pthread_create(&output_thread, NULL, read_game_output, NULL);
        
        while (1) {
            
            
            // 문자 하나 입력 받기 (실시간 입력)
            char ch;
            read(STDIN_FILENO, &ch, 1);

            // 입력 변환
            if (ch == 'a')
                write(to_game[1], "left\n", 5);
            else if (ch == 'd')
                write(to_game[1], "right\n", 6);
            else if (ch == 's')
                write(to_game[1], "down\n", 5);
            else if (ch == 'w')
                write(to_game[1], "rotate\n", 7);
            else if (ch == 'q') {
                write(to_game[1], "exit\n", 5);
                break;
            }

            // 단어 입력 받기
            // fgets(input, sizeof(input), stdin);

            // 개행 제거
            // input[strcspn(input, "\n")] = 0;

            // game으로 input 전달
            // write(to_game[1], input, strlen(input));
            // write(to_game[1], "\n", 1);
            
            // exit 처리
            // if (strcmp(input, "exit") == 0)
            //     break;

            // // game 출력 받아오기
            // char buffer[100000];
            // int n = read(from_game[0], buffer, sizeof(buffer)-1);
            // buffer[n] = 0;
            // printf("%s", buffer);

            // pthread_join(output_thread, NULL);
        }
    }


    return 0;
}