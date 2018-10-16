#include "server.h"
#include "split.h"
#include "client.h"

using namespace std;

void *spawnClient(void *port)
{
    Client client = Client("127.0.0.1", *((int *)port));
    client.run();
    pthread_exit(NULL);
}

void *startServer(void *mode)
{
    Server server = Server(PORT);
    int serverMode = *((int *)mode);
    server.run(serverMode);
    pthread_exit(NULL);
}

Server :: Server(int portNo)
{
	port = portNo;
	UDPport = port + 1;
	int opt = 1;
	sock = socket(AF_INET, SOCK_STREAM, 0);
	setsockopt(sock, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt));
	serverAddress.sin_family = AF_INET;
    serverAddress.sin_addr.s_addr = INADDR_ANY;
    serverAddress.sin_port = htons(portNo);
    bind(sock, (struct sockaddr *)&serverAddress, sizeof(serverAddress));

    struct timeval timeout;
    timeout.tv_sec = 0;
	timeout.tv_usec = 1000;

    UDPsock = socket(AF_INET, SOCK_DGRAM, 0);
    setsockopt(UDPsock, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof(timeout));
	UDPserverAddress.sin_family = AF_INET;
    UDPserverAddress.sin_addr.s_addr = INADDR_ANY;
    UDPserverAddress.sin_port = htons(UDPport);
    bind(UDPsock, (struct sockaddr *)&UDPserverAddress, sizeof(UDPserverAddress));

}
int Server :: receive(char *buff)
{
	char stringSize[5];
	int receivedBytes = recv(conn, stringSize, 4, 0);
	stringSize[4] = '\0';
	if(receivedBytes == 0) return 0;
	int size = stoi(stringSize);
	int recvB = recv(conn, buff, size, 0);
	buff[recvB] = '\0';
	return recvB;
	//cout << "recv(): " << buff << endl;
}
int Server :: receiveUDP(char *buff)
{
	unsigned int sizeOfAddress = sizeof(clientAddress);
	//char stringSize[5];
	//int receivedBytes = recvfrom(UDPsock, stringSize, 4, 0, (struct sockaddr *)&clientAddress, &sizeOfAddress);
	//stringSize[receivedBytes] = '\0';
	//if(receivedBytes == 0) return 0;
	//int size = stoi(stringSize);
	int recvB = recvfrom(UDPsock, buff, 2048, 0, (struct sockaddr *)&clientAddress, &sizeOfAddress);
	buff[recvB] = '\0';
	return recvB;
	//cout << "recv(): " << buff << endl;
}
void Server :: receiveFile(const char *fileName, const char *protocol)
{
	int UDPmode = 0;
	if (strcmp(protocol, "UDP") == 0)
		UDPmode = 1;
	char noOfParts[10] = {0};
	receive(noOfParts);
	int parts = stoi(noOfParts);


	int f = open(fileName, O_WRONLY | O_CREAT, 0777);

	cout << "Receiving " << fileName << " [          ] 0%";
	cout.flush();

	for(int i = 0; i < parts; i++)
	{
		char chunk[2049] = {0};
		int sizeOfChunk;
		if(UDPmode) receiveUDP(chunk);
		else sizeOfChunk = receive(chunk);
		
		write(f, chunk, sizeOfChunk);

		int bars = ((i + 1) * 10)/parts;
		int percentage = ((i + 1) * 100)/parts;

		cout << "\rReceiving " << fileName << " [";
		for(int j = 0; j < bars; j++)
			cout << "=";

		for(int j = 0; j < (10 - bars); j++)
			cout << " ";

		cout << "] " << percentage << "%";
		cout.flush();

	}
	cout << endl;
	close(f);
	cout << "Received file" << endl;

}
void Server :: run(int mode)
{
	int sin_size = sizeof(struct sockaddr_in);
	socklen_t *temp = (socklen_t *) &sin_size;
	//cout << "Listening\n";
	listen(sock, 3);
	while(1)
	{
		int size = 0;
		char userName[20];
	    conn = accept(sock, (struct sockaddr *)&clientAddress, temp);
	    //cout << "Connection accepted\n";
	    char strserverPort[5];
	    size = receive(strserverPort);
	    size = receive(userName);
	    cout << endl << "You are now chatting with " << userName << endl;

	    if(mode == 0)
	    {
	    	int serverPort = stoi(strserverPort);
	    	int *arg = &serverPort;
	    	int rc;
	    	pthread_t clientID;
	    	rc = pthread_create(&clientID, NULL, spawnClient, (void *)arg);
	    }
	    else
	    {
	    	cout << "\n\n>> ";
	    	cout.flush();
	    }
		
		char s[2048];
	    while(1){
	    	int receivedBytes = receive(s);
	    	//cout << s << endl;
	    	//cout << strlen(s) << endl;
	    	//cout << receivedBytes << endl;
	    	//cout << s.length() << endl;
	    	if(receivedBytes == 0){
	    		cout << userName << " has left the chat." << endl;
	    		break;
	    	}
	    	//cout << "Creating vector" << endl;
	    	vector <string> v;
			int parts = split(s, v);
			if(strcmp(v[0].c_str(), "Sending") == 0 && ((strcmp(v[2].c_str(), "TCP")) 
				|| (strcmp(v[2].c_str(), "UDP"))))
			{
				cout << endl << userName << ": " << s << endl;
				receiveFile(v[1].c_str(), v[2].c_str());
				cout << ">> ";
				cout.flush();
				continue;
			}
			//cout << strlen(s) << endl;
			//for(int i = 0; i < strlen(s); i++) cout << s[i] << endl;
	    	cout << endl << userName << ": " << s << "\n>> ";
	    	//for(int i = 0; i < parts; i++) cout << v[i] << endl;
	    	cout.flush();
	    }
	}

}