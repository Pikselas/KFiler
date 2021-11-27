#pragma once
#define NOMINMAX
#include<algorithm>
#include<cstdlib>
#include"FileTransferer.h"
#include"NetworkServer.h"
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
	std::vector<FileStatus> SendFile(NetworkServer& server);
public:
	void IncreaseThread(const std::string& Port);
	void DecreaseThread();
	void AddFile(const std::string& FilePath);
	void StopTransfer() override;
	void StartTransfer() override;
};