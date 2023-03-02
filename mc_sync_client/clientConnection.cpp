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

}

void clientConnection::setFail() {
    this->failed = true;
}
bool clientConnection::getFail() {
    return this->failed;
}
void clientConnection::startConn() throw(networkIOException) {
    conn_sock = socket(AF_INET, SOCK_STREAM, 0);
    if (conn_sock == INVALID_SOCKET) {
        err_print("Socket Fail");
        setFail();
        throw networkIOException("Socket creation failed");
    }

    struct sockaddr_in dest_addr;
    dest_addr.sin_family = AF_INET;
    dest_addr.sin_port = htons(s_port);
    dest_addr.sin_addr.s_addr = inet_addr(s_ip.c_str());

    if (connect(conn_sock, (struct sockaddr*)&dest_addr, sizeof(dest_addr)) == SOCKET_ERROR) {
        err_print("Connect Fail. Server nicht gefunden");
        setFail();
        throw networkIOException("Unable to connect. Server not found");
        
    }

}
void clientConnection::stopConn() throw(networkIOException) {
    if (closesocket(conn_sock) == SOCKET_ERROR) {
        err_print("Fehler bei Close");
        setFail();
        throw networkIOException("Socket close failed");
    }
}

int clientConnection::sendBuffer(const char* buffer, int buffSize, int chunkSize) throw(networkIOException) {
    int i = 0;

    while (i < buffSize) {
        int l = send(this->conn_sock, &buffer[i], __min(chunkSize, buffSize - i), 0);
        if (l < 0) {
            err_print("sendBuffer Fail");   // Error
            throw networkIOException("Failed to send Buffer");
            return -1;
        }
        i += l;
    }
    return i;
}
int clientConnection::recvBuffer(char* buffer, int buffSize, int chunkSize) throw(networkIOException) {
    int i = 0;

    while (i < buffSize) {
        const int l = recv(this->conn_sock, &buffer[i], __min(chunkSize, buffSize - i), 0);
        if (l < 0)
        {
            throw networkIOException("Failed to receive Buffer");
	        return l;
        } // this is an error
        i += l;
    }
    return i;
}
long clientConnection::getFileSize(const char* filename) throw(fileIOException) {
    printf("Overflow: %d\n", errno);
    FILE* file = fopen(filename, "rb");
    if (file == NULL) {
        err_print("GetSize FileOpen Fail");
        throw fileIOException("GetSize FileOpen Fail");
        return -1;
    }

    if (fseek(file, 0L, SEEK_END) < 0) {
        err_print("GetSize Fail");
        fclose(file);
        throw fileIOException("GetSize seek failed");
        return -1;
    }
    errno = 0;
    long fsize = ftell(file);
    if (fsize < 0) {
        err_print("GetSize Fail");
        printf("Overflow: %d\n", errno);
        fclose(file);
        throw fileIOException("FileSize bigger than LONG_MAX");
        return -2;  // Datei > LONG_MAX
    }
    fclose(file);
    return fsize;
}
int clientConnection::sendFile(const char* filename, int chunkSize) throw(networkIOException, fileIOException) {
    // Dateigröße schicken:
    long fileSize = getFileSize(filename);
    if (fileSize < 0) {
        if (fileSize == -2) {
            std::stringstream ss(16);
        	ss << "Dateigröße von [" << filename << "] größer als MAX_LONG";
            printf("Dateigröße von [%s] größer als MAX_LONG\n", filename);
            throw networkIOException(ss.str().c_str());
        }
        else {  // Abgedeckt von getFileSize exception
            //throw;
            return -1;
        }
    }
    
    if (sendBuffer(reinterpret_cast<const char*>(&fileSize), sizeof(fileSize)) != sizeof(fileSize)) {
        throw networkIOException("sending FileSize failed");
        return -1;
    }

    FILE* file = fopen(filename, "rb");
    if (file == NULL) {
        throw fileIOException("opening File failed");
        return -2;
    }

    char* buffer = new char[chunkSize];
    bool errored = false;
    int64_t i = fileSize;
    while (i != 0) {
        const int64_t ssize = __min(i, (int64_t)chunkSize);
        if (fread(buffer, sizeof(char), ssize, file) < ssize) { errored = true; break; }
        const int l = sendBuffer(buffer, (int)ssize);
        if (l < 0) { errored = true; break; }
        i -= l;
    }
    delete[] buffer;

    fclose(file);

    //return errored ? -3 : fileSize;

    if (errored)
    {
        throw networkIOException("sending file failed");
        return -3;
    }
    else
    {
        return fileSize;
    }
}
int clientConnection::recvFile(const char* filename, int chunkSize) {
    return 0;
}