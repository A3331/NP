#include <stdio.h>
#include <stdlib.h>
#include <winsock2.h>

#pragma comment(lib, "ws2_32.lib")

int main() {
    WSADATA wsa;
    SOCKET s;
    struct sockaddr_in server;
    int slen = sizeof(server);
    char buffer[512], message[512];

    WSAStartup(MAKEWORD(2, 2), &wsa);
    s = socket(AF_INET, SOCK_DGRAM, 0);

    server.sin_family = AF_INET;
    server.sin_port = htons(8888);
    server.sin_addr.s_addr = inet_addr("127.0.0.1");

    while (1) {
        printf("Enter your guess: ");
        fgets(message, sizeof(message), stdin);
        sendto(s, message, strlen(message), 0, (struct sockaddr *)&server, slen);

        memset(buffer, 0, sizeof(buffer));
        recvfrom(s, buffer, sizeof(buffer), 0, (struct sockaddr *)&server, &slen);
        printf("Server says: %s\n", buffer);

        if (strcmp(buffer, "Correct") == 0)
            break;
    }

    closesocket(s);
    WSACleanup();
    return 0;
}
