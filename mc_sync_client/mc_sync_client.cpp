#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdio.h>
#include <string>
#include <string.h>

#pragma comment(lib, "Ws2_32.lib")
#pragma warning(disable : 4996)
//#define _WINSOCK_DEPRECATED_NO_WARNINGS

using namespace std;

void err_print(const char* nachricht) {
    fprintf(stderr, "%s\n", nachricht);
}

void err_close() {
    WSACleanup();
    exit(-1);
}

WSADATA wsaData;

#define PORT 8090
string ip = "127.0.0.1";
#define MSG_SIZE 500

int main() {

    int iResult;

    // Initialize Winsock
    iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (iResult != 0) {
        printf("WSAStartup failed: %d\n", iResult);
        return 1;
    }

    SOCKET conn_sock = socket(AF_INET, SOCK_STREAM, 0);
    if (conn_sock == INVALID_SOCKET) {
        err_print("Socket Fail");
        err_close();
    }

    printf("Sock: %d\n", conn_sock);

    struct sockaddr_in dest_addr;
    dest_addr.sin_family = AF_INET;
    dest_addr.sin_port = htons(PORT);
    dest_addr.sin_addr.s_addr = inet_addr(ip.c_str());

    if (connect(conn_sock, (struct sockaddr*)&dest_addr, sizeof(dest_addr)) == SOCKET_ERROR) {
        err_print("Connect Fail");
        err_close();
    }

    char msg[MSG_SIZE];
    memset(msg, 0, MSG_SIZE);

    int len = recv(conn_sock, msg, MSG_SIZE, 0);

    printf("Recv-Len: %d\n", len);
    printf("%s\n", msg);

    // Aufräumen
    if (closesocket(conn_sock) == SOCKET_ERROR) {
        err_print("Fehler bei Close");
    }
    if (WSACleanup() != 0) {
        err_print("Fail bei Cleanup");
    }

    return 0;
}