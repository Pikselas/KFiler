#include "FileTransferer.h"

const FileTransferer::FileQtype& FileTransferer::GetPendings() const noexcept
{
	return PendingFiles;
}

const FileTransferer::TrackerType& FileTransferer::GetFileStatusList() const noexcept
{	
	return StatusTracker;
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
