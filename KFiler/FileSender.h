#pragma once
#define NOMINMAX
#include<fstream>
#include"FileTransferer.h"
#include"NetworkServer.h"
#include"KsXstr.hpp"
class FileSender : public FileTransferer
{
private:
	std::unique_ptr<NetworkServer> MAIN_SERVER;
	std::queue<std::string> UseablePorts;
	std::vector<std::shared_ptr<NetworkServer>> FileServers;
public:
	 FileSender(const std::string& Port);
	~FileSender();
private:
	ITRListType SendFile(std::shared_ptr<NetworkServer> FileServer);
	static size_t GetFileSize(std::ifstream& file);
public:
	void IncreaseThread(const std::string& Port);
	void DecreaseThread();
	void AddFile(const std::string& FilePath);
	void StopTransfer() override;
	void StartTransfer() override;
};