#pragma once
#include"NetworkBuilder.h"
#include<unordered_map>
#include<list>
class FileTransferer
{
public:
	struct FileStatus
	{
		std::string name;
		size_t size;
		size_t transferred;
	};
public:
	typedef std::queue<std::string> FileQtype;
	typedef std::vector<FileStatus> FileStatusListType;
	typedef std::vector<FileStatus>::iterator StatusITRType;
	typedef std::list<StatusITRType> ITRListType;
	typedef std::unordered_map<std::string, std::future<ITRListType>> ReportType;
protected:
	int MAX_THREAD_COUNT = 0;
	std::atomic_int USING_THREADS = 0;
	std::atomic_size_t TRANSFER_RATE = 4096;
protected:
	std::atomic_bool ContinueTransfer = false;
	FileQtype PendingFiles;
	FileStatusListType FileStatusList;
	ReportType TransferReport;
	std::mutex mtx;
public:
	const FileQtype& GetPendings() const noexcept;
	const FileStatusListType& GetFileStatusList() const noexcept;
	const ReportType& GetTransferReport() const noexcept;
	int GetMaxThreadCount() const noexcept;
	int GetUsingThreadCount() const noexcept;
	void SetTransferRate(const size_t Bytes) noexcept;
	size_t GetTransferRate() const noexcept;
	virtual void StopTransfer() = 0;
	virtual void StartTransfer() = 0;
};