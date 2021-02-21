#pragma once
#pragma comment(lib, "ws2_32.lib")
#pragma warning(disable : 4996)
#include <WinSock2.h>

typedef void (*CALLBACK_ACTION)(char*);

class ChatClient
{
public:
	ChatClient();
	ChatClient(const char* addr, const char* username, CALLBACK_ACTION callback);
	~ChatClient();
	void sendMessage(char* str);
private:
	static DWORD WINAPI staticThreadStart(void* p);
	SOCKET mSocket;
	SOCKADDR_IN mSockAddr;
	const char* mUsername;
	char mBuffer[1024];
	CALLBACK_ACTION mCallback;
	DWORD listenerSocket();
};