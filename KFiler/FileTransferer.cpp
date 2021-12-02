#include "FileTransferer.h"

FileTransferer::FileStatus::FileStatus(const std::string name, const int size, const bool status) : name(name) , size(size) , success(status)
{}

std::string FileTransferer::FileStatus::GetName() const noexcept
{
	return name;
}

int FileTransferer::FileStatus::GetSize() const noexcept
{
	return size;
}

bool FileTransferer::FileStatus::SuccessStatus() const noexcept
{
	return success;
}

const FileTransferer::FileQtype& FileTransferer::GetPendings() const noexcept
{
	return PendingFiles;
}

const FileTransferer::StatusLtype& FileTransferer::GetFileStatusList() const noexcept
{	
	return StatusList;
}

int FileTransferer::GetMaxThreadCount() const noexcept
{
	return MAX_THREAD_COUNT;
}

int FileTransferer::GetUsingThreadCount() const noexcept
{
	return USING_THREADS;
}

void FileTransferer::SetTransferRate(const size_t Bytes) noexcept
{
	TRANSFER_RATE = Bytes;
}

size_t FileTransferer::GetTransferRate() const noexcept
{
	return TRANSFER_RATE;
}

void FileTransferer::StopTransfer()
{
	ContinueTransfer = false;
}
