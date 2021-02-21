#include "ChatClient.h"

ChatClient::ChatClient() { }

ChatClient::ChatClient(const char* addr, const char* username, CALLBACK_ACTION callback) {
	mUsername = username;
	mCallback = callback;

	WSAData wsaData;
	WORD DLLVersion = MAKEWORD(1, 2);
	if (WSAStartup(DLLVersion, &wsaData) != 0) {
		// todo onFailed()
		exit(1);
	}

	mSocket = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);

	mSockAddr.sin_family = PF_INET;
	mSockAddr.sin_addr.S_un.S_addr = inet_addr(addr);
	mSockAddr.sin_port = htons(8006);


	strcpy(mBuffer, "c++ client!");
	mCallback(mBuffer);
	connect(mSocket, (sockaddr*)&mSockAddr, sizeof(mSockAddr));
	int k = ::send(mSocket, username, strlen(username), 0);
	if (k < -1) {
		strcpy(mBuffer, "f*ck");
		mCallback(mBuffer);
	}

	CreateThread(0, 1024, staticThreadStart, (void*)this, 0, NULL);
}

ChatClient::~ChatClient() {

}

DWORD ChatClient::listenerSocket() {
	while (true) {
		int k = recv(mSocket, mBuffer, sizeof(mBuffer), 0);
		if (k > 0) {
			mCallback(mBuffer);
		}
		Sleep(30);
	}
	return 0;
}

DWORD WINAPI ChatClient::staticThreadStart(void* p) {
	ChatClient* client = (ChatClient*)p;
    return client->listenerSocket();
}

void ChatClient::sendMessage(char* str) {
	send(mSocket, str, strlen(str), 0);
}