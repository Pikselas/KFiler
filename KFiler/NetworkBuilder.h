#pragma once
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include<WinSock2.h>
#include<WS2tcpip.h>
#include<string>
#include<vector>
#include<optional>
#include<memory>
#pragma comment (lib, "Ws2_32.lib")

class NetworkBuilder
{
private:
	class Starter
	{
	private:
		Starter();
		~Starter();
		static Starter Stuart;
	};
private:
	constexpr static int MAX_HOST_LENGTH = 80;
protected:
	int RECEIVE_SIZE = 3600;
	std::unique_ptr<char[]> RECV_BUFF;
 	bool HasConnection = false;
	SOCKET CONNECTION_SOCKET = INVALID_SOCKET;
protected:
	NetworkBuilder();
public:
	static std::vector<std::string>GetDeviceIPs();
public:
	bool IsConnected() const;
    bool Send(const std::string&);
	std::optional<std::string> Receive();
	void CloseConnection();
};