#pragma once
#include"NetworkBuilder.h"
class NetworkServer : public NetworkBuilder
{
private:
	SOCKET LISTEN_SOCKET = INVALID_SOCKET;
public:
	NetworkServer(const std::string& Port);
	~NetworkServer();
	void Listen() const;
	void AcceptConnection();
	void DestroyServer();
};