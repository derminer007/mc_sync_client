#include "clientConnection.h"

void err_print(const char* nachricht) {
    fprintf(stderr, "%s\n", nachricht);
}

void err_close() {
    WSACleanup();
    exit(-1);
}

clientConnection::clientConnection(string ip, int port) {
    this->s_ip = ip;
    this->s_port = port;
    failed = false;

    iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (iResult != 0) {
        printf("WSAStartup failed: %d\n", iResult);
        setFail();
    }
    
    startConn();
}
clientConnection::~clientConnection() {
    if (WSACleanup() != 0) {
        err_print("Fail bei Cleanup");
    }

    stopConn();
}

void clientConnection::setFail() {
    this->failed = true;
}
bool clientConnection::getFail() {
    return this->failed;
}
void clientConnection::startConn() {
    conn_sock = socket(AF_INET, SOCK_STREAM, 0);
    if (conn_sock == INVALID_SOCKET) {
        err_print("Socket Fail");
        setFail();
    }

    struct sockaddr_in dest_addr;
    dest_addr.sin_family = AF_INET;
    dest_addr.sin_port = htons(s_port);
    dest_addr.sin_addr.s_addr = inet_addr(s_ip.c_str());

    if (connect(conn_sock, (struct sockaddr*)&dest_addr, sizeof(dest_addr)) == SOCKET_ERROR) {
        err_print("Connect Fail. Server nicht gefunden");
        setFail();
        //throw networkIOException("Unable to connect");
    }

}
void clientConnection::stopConn() {
    if (closesocket(conn_sock) == SOCKET_ERROR) {
        err_print("Fehler bei Close");
        setFail();
    }
}

int clientConnection::sendBuffer(const char* buffer, int buffSize, int chunkSize) {
    int i = buffSize;

    while (i > 0) {
        int l = send(this->conn_sock, &buffer[i], __min(chunkSize, buffSize - i), 0);
        if (l < 0) {
            err_print("sendBuffer Fail");   // Error
            return -1;
        }
        i += l;
    }
    return i;
}
int clientConnection::recvBuffer(char* buffer, int buffSize, int chunkSize) {
    int i = 0;

    while (i < buffSize) {
        const int l = recv(this->conn_sock, &buffer[i], __min(chunkSize, buffSize - i), 0);
        if (l < 0) { return l; } // this is an error
        i += l;
    }
    return i;
}
int clientConnection::getFileSize(const char* filename) {
    FILE* file = fopen(filename, "rb");
    if (file == NULL) {
        err_print("GetSize FileOpen Fail");
        return -1;
    }

    if (fseek(file, 0L, SEEK_END) < 0) {
        err_print("GetSize Fail");
        fclose(file);
        return -1;
    }
    long fsize = ftell(file);
    if (fsize < 0) {
        err_print("GetSize Fail");
        fclose(file);
        return -1;
    }
    fclose(file);
    return fsize;
}
int clientConnection::sendFile(const char* filename, int chunkSize) {
    return 0;
}
int clientConnection::recvFile(const char* filename, int chunkSize) {
    return 0;
}