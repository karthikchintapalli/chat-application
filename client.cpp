#include "client.h"
#include "split.h"

using namespace std;

void Client :: sendMessage(char* s)
{
	char size[5];
	sprintf(size, "%04lu", strlen(s));
	send(sock, size, 4, 0);
	send(sock, s, strlen(s), 0);
}
Client :: Client(char* serverIP, int serverPortNo)
{
	serverPort = serverPortNo;
	serverUDPPort = serverPort + 1;
	struct hostent *host;
	host = gethostbyname(serverIP);
	sock = socket(AF_INET, SOCK_STREAM, 0);
	serverAddress.sin_family = AF_INET;
	serverAddress.sin_port = htons(serverPortNo);
	serverAddress.sin_addr = *((struct in_addr *)host->h_addr);

	serverUDPAddress.sin_family = AF_INET;
	serverUDPAddress.sin_port = htons(serverUDPPort);
	serverUDPAddress.sin_addr = *((struct in_addr *)host->h_addr);
}

void Client :: sendFile(const char *fileName, const char *protocol)
{
	int UDPmode = 0;
	int UDPsock;
	if (strcmp(protocol, "UDP") == 0)
	{
		UDPmode = 1;
		UDPsock = socket(AF_INET, SOCK_DGRAM, 0);
	}
	int f = open(fileName, O_RDONLY);
	int fileSize = lseek(f, (off_t)0, SEEK_END);
	int chunkSize = 2048;
	int parts = (fileSize/chunkSize) + 1;

	char noOfParts[10] = {0};
	sprintf(noOfParts, "%d", parts);
	sendMessage(noOfParts);

	cout << "Sending " << fileName << " [          ] 0%";
	cout.flush();

	lseek(f, (off_t)0, SEEK_SET);
	for(int i = 0; i < parts; i++)
	{
		char chunk[2049] = {0};
		int sizeOfChunk = read(f, chunk, 2048);

		char size[5];
		sprintf(size, "%04d", sizeOfChunk);

		if(UDPmode)
		{
			//sendto(UDPsock, size, 4, 0, (struct sockaddr *)&serverUDPAddress, sizeof(serverUDPAddress));
			sendto(UDPsock, chunk, sizeOfChunk, 0,(struct sockaddr *)&serverUDPAddress, sizeof(serverUDPAddress));
		}
		else
		{
			send(sock, size, 4, 0);
			send(sock, chunk, sizeOfChunk, 0);
		}

		int bars = ((i + 1) * 10)/parts;
		int percentage = ((i + 1) * 100)/parts;

		cout << "\rSending " << fileName << " [";
		for(int j = 0; j < bars; j++)
			cout << "=";

		for(int j = 0; j < (10 - bars); j++)
			cout << " ";

		cout << "] " << percentage << "%";
		cout.flush();

	}
	cout << endl;
	close(f);
	cout << "Sent file" << endl;

}

void Client :: run()
{
	char userName[20] = {0};
	connect(sock, (struct sockaddr *)&serverAddress, sizeof(serverAddress));
	//cout << "Connected to server\n";
	char chport[5] = {0};
	sprintf(chport, "%d", PORT);
	sendMessage(chport);
	cout << "Enter your username: ";
	scanf("%s", userName);
	sendMessage(userName);
	while(1){
		cout << ">> ";
		char s[2048] = {0};
		char c;
		int count = 0;
		scanf("%c", &c);
		while(c != '\n')
		{
			s[count++] = c;
			scanf("%c", &c);
		}
		s[count++] = '\0';
		count = 0;
		if (strlen(s) == 0) {
			cout << '\r';
			continue;
		}
		
		sendMessage(s);	

		vector <string> v;
		int parts = split(s, v);
		if(strcmp(v[0].c_str(), "Sending") == 0 && ((strcmp(v[2].c_str(), "TCP")) 
			|| (strcmp(v[2].c_str(), "UDP"))))
		{
			sendFile(v[1].c_str(), v[2].c_str());
			cout.flush();
			continue;
		}
	}
	
}