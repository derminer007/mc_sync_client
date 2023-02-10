#pragma once

#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdio.h>
#include <string>
#include <string.h>
#include <exception>

#pragma comment(lib, "Ws2_32.lib")
#pragma warning(disable : 4996)

using namespace std;

/// <summary>
/// print to stderr
/// </summary>
/// <param name="nachricht">string to be printed</param>
void err_print(const char* nachricht);

/// <summary>
/// Cleanup WSA and exit with -1
/// </summary>
void err_close();

class clientConnection
{
private:
	WSADATA wsaData;
	int iResult;

	SOCKET conn_sock;
	string s_ip;
	int s_port;
	bool failed;

	void setFail();
	void startConn();
	void stopConn();

public:
	clientConnection(string ip, int port);
	~clientConnection();

	bool getFail();

	int sendBuffer(const char* buffer, int buffSize, int chunkSize = 4*1024);
	int recvBuffer(char* buffer, int buffSize, int chunkSize = 4*1024);
	int sendFile(const char* filename, int chunkSize = 64*1024);
	int recvFile(const char* filename, int chunkSize = 64*1024);
	int getFileSize(const char* filename);
	/*string getFileHash();*/
};

