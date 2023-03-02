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

    
    //c1.getFileSize("C:\\Users\\aran\\source\\repos\\mc_sync_client\\mc_sync_client\\clientConnection.cpp");
    //printf("Size: %d\n", c1.getFileSize("C:\\Users\\aran\\source\\repos\\mc_sync_client\\mc_sync_client\\clientConnection.cpp"));
    //printf("Size: %d\n", c1.getFileSize("C:\\Users\\aran\\source\\repos\\mc_sync_client\\mc_sync_client\\long_max.txt"));
    //printf("Gefailed: %d\n", c1.getFail());
    //printf("Long Max: %ld\n", LONG_MAX);

    try
    {
        clientConnection c1 = clientConnection(ip, PORT);
        int l = c1.sendBuffer("FFFFF", 5);
        printf("END %d\n", l);
        c1.sendFile("max.txt");
        c1.stopConn();
    }
    catch(networkIOException& e)
    {
        cout << "Fehler: " << e.what() << endl;
    }
    catch(fileIOException& e)
    {
        cout << "Fehler: " << e.what() << endl;
    }

    return 0;
}