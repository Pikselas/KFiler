#pragma once
#include"NetworkBuilder.h"
class NetworkClient : public NetworkBuilder
{
public:
	void Connect(const std::string& Address , const std::string& Port);
};