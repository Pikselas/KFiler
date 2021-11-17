#include "NetworkBuilder.h"
NetworkBuilder::Starter NetworkBuilder::Starter::Stuart;
NetworkBuilder::Starter::Starter()
{
	WSADATA wdt;
	WSAStartup(MAKEWORD(2, 2),&wdt);
}
NetworkBuilder::Starter::~Starter()
{
	WSACleanup();
}
NetworkBuilder::NetworkBuilder()
{
	RECV_BUFF = std::make_unique<char[]>(RECEIVE_SIZE);
}
std::vector<std::string> NetworkBuilder::GetDeviceIPs()
{
	using namespace std;
	vector<string> IPS;
	auto NameBuffer = std::make_unique<char[]>(MAX_HOST_LENGTH);
	if (gethostname(NameBuffer.get(), MAX_HOST_LENGTH) != SOCKET_ERROR)
	{
		hostent* hosts = gethostbyname(NameBuffer.get());
		if (hosts != nullptr)
		{
			for (int i = 0; hosts->h_addr_list[i] != nullptr; i++)
			{
				in_addr adrs;
				memcpy(&adrs, hosts->h_addr_list[i], sizeof(in_addr));
				IPS.emplace_back(inet_ntoa(adrs));
			}
		}
	}
	return IPS;
}

bool NetworkBuilder::IsConnected() const
{
	return HasConnection;
}

bool NetworkBuilder::Send(const std::string& data)
{
	if (send(CONNECTION_SOCKET, data.c_str(), (int)data.length(), 0) == SOCKET_ERROR)
	{
		closesocket(CONNECTION_SOCKET);
		return HasConnection = false;
	}
	return true;
}

std::optional<std::string> NetworkBuilder::Receive()
{
	if (recv(CONNECTION_SOCKET, RECV_BUFF.get(), RECEIVE_SIZE, 0) > 0)
	{
		return RECV_BUFF.get();
	}
	HasConnection = false;
	return {};
}

void NetworkBuilder::CloseConnection()
{
	shutdown(CONNECTION_SOCKET, SD_BOTH);
	closesocket(CONNECTION_SOCKET);
	HasConnection = false;
	CONNECTION_SOCKET = INVALID_SOCKET;
}
