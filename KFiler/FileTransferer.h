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
	typedef std::vector<FileStatus> FileStatusListType;
	typedef std::list<size_t> IndxListType;
	typedef std::unordered_map<std::string, std::future<IndxListType>> ReportType;
protected:
	int MAX_THREAD_COUNT = 0;
	std::atomic_int USING_THREADS = 0;
	std::atomic_int TRANSFER_RATE = 4096;
protected:
	std::atomic_bool ContinueTransfer = false;
	FileStatusListType FileStatusList;
	ReportType TransferReport;
	std::mutex mtx;
public:
	const FileStatusListType& GetFileStatusList() const noexcept;
	const ReportType& GetTransferReport() const noexcept;
	int GetMaxThreadCount() const noexcept;
	int GetUsingThreadCount() const noexcept;
	void SetTransferRate(const int Bytes) noexcept;
	int GetTransferRate() const noexcept;
	virtual void StopTransfer() = 0;
	virtual void StartTransfer() = 0;
};