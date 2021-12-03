#include "FileTransferer.h"

const FileTransferer::FileStatusListType& FileTransferer::GetFileStatusList() const noexcept
{	
	return FileStatusList;
}

const FileTransferer::ReportType& FileTransferer::GetTransferReport() const noexcept
{
	return TransferReport;
}

int FileTransferer::GetMaxThreadCount() const noexcept
{
	return MAX_THREAD_COUNT;
}

int FileTransferer::GetUsingThreadCount() const noexcept
{
	return USING_THREADS;
}

void FileTransferer::SetTransferRate(const int Bytes) noexcept
{
	TRANSFER_RATE = Bytes;
}

int FileTransferer::GetTransferRate() const noexcept
{
	return TRANSFER_RATE;
}

void FileTransferer::StopTransfer()
{
	ContinueTransfer = false;
}
