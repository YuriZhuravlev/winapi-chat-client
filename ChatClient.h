#pragma once
#pragma comment(lib, "ws2_32.lib")
#pragma warning(disable : 4996)
#include <WinSock2.h>

typedef void (*CALLBACK_ACTION)(char*, int);
typedef void (*CALLBACK_FAILED)();

class ChatClient
{
public:
	ChatClient();
	ChatClient(const char* addr, const char* username, CALLBACK_ACTION callback, CALLBACK_FAILED failed);
	~ChatClient();
	void sendMessage(char* str);
	static DWORD WINAPI staticThreadStart(void* p);
private:
	SOCKET mSocket;
	SOCKADDR_IN mSockAddr;
	const char* mUsername;
	char mBuffer[1024];
	CALLBACK_ACTION mCallback;
	CALLBACK_FAILED mFailed;
	DWORD listenerSocket();
};