#pragma once
#include<fstream>
#include"FileTransferer.h"
#include"NetworkClient.h"
#include"KsXstr.hpp"
class FileReceiver : FileTransferer
{
private:
	std::unique_ptr<NetworkClient> MAIN_CLIENT;
private:
	std::pair<std::string, std::string> Sender;
	std::vector<std::shared_ptr<NetworkClient>> Clients;
public:
	std::vector<std::string> PORTS;
public:
	FileReceiver();
private:
	size_t ReceiveFile(std::shared_ptr<NetworkClient> client,const std::string& port);
public:
	void IncreaseThread();
	void DecreaseThread();
	void SetSender(const std::string& IP , const std::string& PORT);
	void StopTransfer() override;
	void StartTransfer() override;
};