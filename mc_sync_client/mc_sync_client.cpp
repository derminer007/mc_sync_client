#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdio.h>
#include <string>
#include <string.h>

#pragma comment(lib, "Ws2_32.lib")
#pragma warning(disable : 4996)
//#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include "clientConnection.h"

using namespace std;

//void err_print(const char* nachricht) {
//    fprintf(stderr, "%s\n", nachricht);
//}
//
//void err_close() {
//    WSACleanup();
//    exit(-1);
//}

WSADATA wsaData;

#define PORT 8090
string ip = "127.0.0.1";
#define MSG_SIZE 500

int main() {

    clientConnection c1 = clientConnection(ip, PORT);
    //c1.getFileSize("C:\\Users\\aran\\source\\repos\\mc_sync_client\\mc_sync_client\\clientConnection.cpp");
    //printf("Size: %d\n", c1.getFileSize("C:\\Users\\aran\\source\\repos\\mc_sync_client\\mc_sync_client\\clientConnection.cpp"));
    printf("Size: %d\n", c1.getFileSize("clientConnection.cpp"));
    printf("Gefailed: %d\n", c1.getFail());

    return 0;
}