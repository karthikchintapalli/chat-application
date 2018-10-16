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

class Server{
private:
	int port;
	int UDPport;
	int UDPsock;
	int sock;
	int conn;
	struct sockaddr_in serverAddress;
	struct sockaddr_in UDPserverAddress;
	struct sockaddr_in clientAddress;

public:
	Server(int portNo);
	void run(int mode);
	int receive(char *buff);
	int receiveUDP(char *buff);
	void receiveFile(const char *fileName, const char *protocol);

};

void *spawnClient(void *port);
void *startServer(void *mode);