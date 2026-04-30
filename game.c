#include <stdio.h>
#include <string.h>

int main() {
    char cmd[100];

    while (1) {
        fgets(cmd, sizeof(cmd), stdin);
        cmd[strcspn(cmd, "\n")] = 0;

        printf("Game received: %s\n", cmd);
    }

    return 0;
}