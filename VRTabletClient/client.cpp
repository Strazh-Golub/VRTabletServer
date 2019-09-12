#include<iostream>
#include<WS2tcpip.h>
#include<string>
using namespace std;

#pragma comment (lib, "ws2_32.lib")

SOCKET sock;
sockaddr_in serv_addr;
char buffer[4096];
string ip;

void initClientConnection()
{
	sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock == INVALID_SOCKET)
	{
		cerr << "Could not create socket! Closing Client!" << endl;
		exit(GetLastError());
	}

	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = 54000;

	getline(cin, ip);

	if (inet_pton(serv_addr.sin_family, (PCSTR) &ip, &serv_addr.sin_addr) <= 0)
	{
		cerr << "Invalid Address! Closing Client!" << endl;
		exit(GetLastError());
	}

	if (connect(sock, (sockaddr*)& serv_addr, sizeof(serv_addr)) == 0)
	{
		cout << "VR Headset Connected!" << endl;	
	}
}

int main()
{
	initClientConnection();
	while (true)
	{
		cin >> buffer;
		send(sock, buffer, 4096, 0);
		cout << endl;
	}
	return 0;
}