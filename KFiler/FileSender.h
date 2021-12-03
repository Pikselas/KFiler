#pragma once
#define NOMINMAX
#include<fstream>
#include"FileTransferer.h"
#include"NetworkServer.h"
#include"KsXstr.hpp"
class FileSender : public FileTransferer
{
public:
	typedef std::queue<std::string> FileQtype;
private:
	std::unique_ptr<NetworkServer> MAIN_SERVER;
	std::queue<std::string> UseablePorts;
	std::vector<std::shared_ptr<NetworkServer>> FileServers;
	FileQtype PendingFiles;
public:
	 FileSender(const std::string& Port);
	~FileSender();
private:
	IndxListType SendFile(std::shared_ptr<NetworkServer> FileServer);
	static size_t GetFileSize(std::ifstream& file);
public:
	void IncreaseThread(const std::string& Port);
	void DecreaseThread();
	void AddFile(const std::string& FilePath);
	const FileQtype& GetPendings() const noexcept;
	void StopTransfer() override;
	void StartTransfer() override;
};