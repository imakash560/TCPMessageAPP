
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define WIN32_LEAN_AND_MEAN
#include<windows.h>
#include<tchar.h>
#include<iostream>
#include<winsock2.h>
#include<vector>
#include<process.h>
#pragma comment(lib,"ws2_32.lib" )
using namespace std;

unsigned int _stdcall mesgClient(void* data);
unsigned int _stdcall recvClient(void* data);
vector<SOCKET> client_log;

int _tmain(int argc, _TCHAR* argv[])
{

	WSADATA wsaData;
	int iResult;
	sockaddr_in addr;
	SOCKET sock, client;

	addr.sin_family = AF_INET;
	addr.sin_port = htons(2222);
	addr.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");

	iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);//2.2 

	if (iResult)
	{
		printf("WSA startup failed");
		return 0;
	}


	sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	if (sock == INVALID_SOCKET)
	{
		printf("Invalid socket");
		return 0;
	}

	iResult = bind(sock, (sockaddr*)&addr, sizeof(sockaddr_in));

	if (iResult)
	{

		printf("bind failed %u", GetLastError());

		return 0;
	}

	iResult = listen(sock, SOMAXCONN);
	printf("Server is listening \n");

	if (iResult)
	{

		printf("iResult failed %u", GetLastError());

		return 0;
	}
	while (client = accept(sock, 0, 0))
	{
		
		if (client == INVALID_SOCKET)
		{
			printf("invalid client socket", GetLastError());
			continue;
		}
		client_log.push_back(client);
		_beginthreadex(0, 0, recvClient, (void*)&client, 0, 0);
		_beginthreadex(0, 0, mesgClient, (void*)&client, 0, 0);
	}


	return 0;
}


unsigned int _stdcall recvClient(void* data)
{
	SOCKET* client = (SOCKET*)data;
	SOCKET Client = *client;
	cout << "Client Connected" << endl;

	char buff[3000];
	while (true) {
		ZeroMemory(buff, 3000);
		int rec = recv(Client, buff, 3000, 0);
		if (rec == SOCKET_ERROR) {
			cout << "Error in receiving" << endl;
			break;
		}
		cout << endl << "Client #" << Client << " - " << buff << endl;
		if (rec == 0) {
			cout << "Client Disconnected" << endl;
			return 0;
		}

	}

}

unsigned int _stdcall mesgClient(void* data)
{
	SOCKET* client = (SOCKET*)data;
	SOCKET Client = *client;
	

	SOCKET curr = Client;
	while (true) {
	cout << "Do you want to send a message : 1 or 0" << endl;
	int var;
	cin >> var;
	if (var) {
		cout << "Enter the id of client between 0 to " << client_log.size() - 1 << endl;
		int id;
		cin >> id;
		SOCKET curr = client_log[id];
		char buff[3000];
		
			ZeroMemory(buff, 3000);
			fgets(buff, 3000, stdin);
			int snd = send(curr, buff, 3000, 0);
			if (snd == SOCKET_ERROR) {
				cout << "Error in sending" << endl;
				return 0;
			}
			/*if (buff[0] == '+') {
				break;
			}*/
		}
	}

}