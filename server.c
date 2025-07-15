#include <stdio.h>
#include <stdlib.h>
#include <winsock2.h>
#include <time.h>

#pragma comment(lib, "ws2_32.lib")

int main() {
    WSADATA wsa;
    SOCKET s;
    struct sockaddr_in server, client;
    int slen = sizeof(client);
    char buffer[512];
    int guess, target;

    srand(time(0));
    target = rand() % 100 + 1;

    printf("Target number is set (1 to 100).\n");

    WSAStartup(MAKEWORD(2, 2), &wsa);
    s = socket(AF_INET, SOCK_DGRAM, 0);

    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons(8888);

    bind(s, (struct sockaddr *)&server, sizeof(server));

    while (1) {
        printf("Waiting for guesses...\n");
        memset(buffer, 0, sizeof(buffer));
        recvfrom(s, buffer, sizeof(buffer), 0, (struct sockaddr *)&client, &slen);
        guess = atoi(buffer);
        printf("Client guessed: %d\n", guess);

        if (guess < target)
            strcpy(buffer, "Higher");
        else if (guess > target)
            strcpy(buffer, "Lower");
        else
            strcpy(buffer, "Correct");

        sendto(s, buffer, strlen(buffer), 0, (struct sockaddr *)&client, slen);

        if (strcmp(buffer, "Correct") == 0)
            break;
    }

    closesocket(s);
    WSACleanup();
    return 0;
}
