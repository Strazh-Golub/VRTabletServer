#include<iostream>
#include<ws2tcpip.h>
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

        memset(host, 0, NI_MAXHOST);
        memset(service, 0, NI_MAXSERV);

        cout << "Waiting for connection..." << endl;

        Sleep(5000);

        if(getnameinfo((sockaddr*) &client, sizeof(client), host, NI_MAXHOST, service, NI_MAXSERV, 0) == 0)
        {
            cout << host << " connected on port " << service << endl;
        }
        else
        {
            inet_ntop(AF_INET, &client.sin_addr, host, NI_MAXHOST);

            cout << host << "connected on port " << ntohs((u_short)&client.sin_addr) << endl;
        }

        closesocket(listening);

        char buf[4096];

        while (true)
        {
            memset(buf, 0, 4096);

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

            send(clientSocket, buf, bytes + 2, 0);
        }

        closesocket(clientSocket);

        WSACleanup();
    }
}

void loop()
{
    initConnection();
    while (!CloseWindow(window))
    {
        if(GetAsyncKeyState(VK_ESCAPE))
        {
            closesocket(clientSocket);
            closesocket(listening);
            CloseWindow(window);
        }
    }

}

int main()
{
    loop();
    return 0;
}