#include "ChatClient.h"

ChatClient::ChatClient() {  }

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
}

ChatClient::~ChatClient() {

}

DWORD ChatClient::listenerSocket() {
	connect(mSocket, (sockaddr*)&mSockAddr, sizeof(mSockAddr));
	int k = ::send(mSocket, mUsername, strlen(mUsername), 0);
	if (k < -1) {
		strcpy(mBuffer, "error connect! Restart please");
		mCallback(mBuffer, strlen(mBuffer));
	}
	while (true) {
		int k = recv(mSocket, mBuffer, sizeof(mBuffer), 0);
		if (k > 0) {
			mCallback(mBuffer, k);
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