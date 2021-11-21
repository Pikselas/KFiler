#pragma once
#include<string>
#include<queue>
#include<atomic>
#include<future>
#include<vector>
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
	int MAX_THREAD_COUNT = 1;
	int USING_THREADS = 0;
protected:
	std::atomic_bool ContinueTransfer = false;
	FileQtype PendingFiles;
	StatusLtype StatusList;
public:
	const FileQtype& GetPendings() const noexcept;
	const StatusLtype& GetFileStatusList() const noexcept;
	int GetMaxThreadCount() const noexcept;
	int GetUsingThreadCount() const noexcept;
	void StopTransfer();
	virtual void StartTransfer() = 0;
};