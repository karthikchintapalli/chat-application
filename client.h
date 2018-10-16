#include <iostream>
#include <vector>
#include <cstring>
#include <algorithm>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>
#include <cstdio>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/time.h>

class Client{
private:
	int serverPort;
	int serverUDPPort;
	int sock;
	struct sockaddr_in serverAddress;
	struct sockaddr_in serverUDPAddress;

public:
	Client(char* serverIP, int serverPortNo);
	void run();
	void sendMessage(char* s);
	void sendFile(const char* fileName, const char* protocol);
};

extern int PORT;