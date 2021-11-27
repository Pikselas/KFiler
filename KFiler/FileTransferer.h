#pragma once
#include<string>
#include<queue>
#include<atomic>
#include<future>
#include<vector>
#include<mutex>
class FileTransferer
{
public:
	class FileStatus
	{
	  private:
		std::string name;
		int size;
		bool success;
	  public:
		FileStatus(const std::string name,const int size,const bool status);
		std::string GetName() const noexcept;
		int GetSize() const noexcept;
		bool SuccessStatus() const noexcept;
	};
public:
	typedef std::queue<std::string> FileQtype;
	typedef std::vector<std::future<std::vector<FileStatus>>> StatusLtype;
protected:
	int MAX_THREAD_COUNT = 0;
	std::atomic_int USING_THREADS = 0;
protected:
	std::atomic_bool ContinueTransfer = false;
	FileQtype PendingFiles;
	StatusLtype StatusList;
	std::mutex mtx;
public:
	const FileQtype& GetPendings() const noexcept;
	const StatusLtype& GetFileStatusList() const noexcept;
	int GetMaxThreadCount() const noexcept;
	int GetUsingThreadCount() const noexcept;
	virtual void StopTransfer() = 0;
	virtual void StartTransfer() = 0;
};