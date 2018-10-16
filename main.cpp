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

#include "client.h"
#include "server.h"

using namespace std;

int PORT;

int main(int argc, char const *argv[])
{
	int rc;
	pthread_t threadID;
	pthread_attr_t attr;
	void *status;

	pthread_attr_init(&attr);
	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);

	PORT = stoi(argv[1]);

	if(argc == 2)
	{
		int i = 0;
		int *mode = &i;
		rc = pthread_create(&threadID, &attr, startServer, (void *)mode);
		rc = pthread_join(threadID, &status);

	}
	else if(argc == 4)
	{
		int i = 1;
		int *mode = &i;
		char IP[20];
		strcpy(IP, argv[2]);
		int port = stoi(argv[3]);
		rc = pthread_create(&threadID, NULL, startServer, (void *)mode);

		Client client = Client(IP, port);
		client.run();

	}
	return 0;
}