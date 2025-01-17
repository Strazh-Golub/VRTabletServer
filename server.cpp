#include<iostream>
#include<ws2tcpip.h>
#include<string>
using namespace std;

#pragma comment (lib, "ws2_32.lib")

WSADATA wsData;
WORD version = MAKEWORD(2, 2);
int wsOK = WSAStartup(version, &wsData);
SOCKET clientSocket;
SOCKET listening;

HWND window = GetConsoleWindow();

void initConnection()
{

    if (wsOK != 0)
    {
        cerr << "Failed to init winsock! Closing application!" << endl;
        exit(-1);
    }

    listening = socket(AF_INET, SOCK_STREAM, 0);
    if (listening == INVALID_SOCKET)
    {
        cerr << "Failed to create a socket! Closing Application!" << endl;
        exit(-1);
    }

    sockaddr_in hint;
    hint.sin_family = AF_INET;
    hint.sin_port = htons(54000);
    hint.sin_addr.S_un.S_addr = INADDR_ANY;

    bind(listening, (sockaddr*) &hint, sizeof(hint));

    listen(listening, SOMAXCONN);

    sockaddr_in client;
    int client_size = sizeof(client);

    clientSocket = accept(listening, (sockaddr*) &client, &client_size);

    if (clientSocket == INVALID_SOCKET)
    {
        cerr << "Can't accept listening socket! Closing Application!" << endl;
        exit(-1);
    }
    else
    {
        char host[NI_MAXHOST];
        char service[NI_MAXSERV];

        ZeroMemory(host, 0);
        ZeroMemory(service, 0);

		int triesRemaining = 5;

        cout << "Waiting for connection..." << endl;

		while (triesRemaining > 0)
		{
			if (getnameinfo((sockaddr*)& client, sizeof(client), host, NI_MAXHOST, service, NI_MAXSERV, 0) == 0)
			{
				cout << host << " connected on port " << service << endl;
				break;
			}
			else
			{
				inet_ntop(AF_INET, &client.sin_addr, host, NI_MAXHOST);

				cout << host << "connected on port " << ntohs((u_short)& client.sin_addr) << endl;
				break;
			}
			Sleep(1000);
			triesRemaining--;
			if (triesRemaining == 0)
			{
				cerr << "Could not connect VR Tablet Headset to server! Closing!" << endl;
				break;
			}
		}

        closesocket(listening);

        char buf[4096];

        while (true)
        {
            ZeroMemory(buf, 0);

            int bytes = recv(clientSocket, buf, 4096, 0);

            if (bytes == SOCKET_ERROR)
            {
                cerr << "Could not recieve data from client! Closing Application!" << endl;
				break;
            }
            else if(bytes == 0)
            {
                cout << "Client Disconnected! Closing!" << endl;
                break;
            }

			cin >> buf;			

			send(clientSocket, buf, bytes + 1, 0);

			cout << endl;
        }

        closesocket(clientSocket);

        WSACleanup();
    }
}

void run()
{
    initConnection();
}

int main()
{
    run();
    return 0;
}