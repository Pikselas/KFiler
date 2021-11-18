#pragma once
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include<WinSock2.h>
#include<WS2tcpip.h>
#include<string>
#include<vector>
#include<optional>
#include<locale>
#include<codecvt>
#include<exception>
#pragma comment (lib, "Ws2_32.lib")

class NetworkBuilder
{
public:
	class Exception : public std::exception
	{
	private:
		int line;
		std::string file;
		std::string Message;
	public:
		Exception(int line, const char* file, const int ErrorCode);
		const char* what() const noexcept override;
		int GetLine() const noexcept;
		const std::string& GetFile() const noexcept;
	};
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
	bool IsConnected() const noexcept;
    void Send(const std::string&);
	std::optional<std::string> Receive();
	void CloseConnection();
};

#define ThrowException(code) throw NetworkBuilder::Exception(__LINE__,__FILE__,code)