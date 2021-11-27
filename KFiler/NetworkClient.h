#pragma once
#include"NetworkBuilder.h"
class NetworkClient : public NetworkBuilder
{
private:
	std::string ConnectedOn;
public:
	void Connect(const std::string& Address , const std::string& Port);
	const std::string& GetConnectedOn() const noexcept;
};