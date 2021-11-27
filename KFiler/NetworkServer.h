#pragma once
#include"NetworkBuilder.h"
class NetworkServer : public NetworkBuilder
{
private:
	SOCKET LISTEN_SOCKET = INVALID_SOCKET;
	std::string ListeningOn;
public:
	NetworkServer(const std::string& Port);
	~NetworkServer();
	void Listen() const;
	void AcceptConnection();
	const std::string& GetListeningPort() const;
	std::string GetClientIP() const;
	void DestroyServer();
};