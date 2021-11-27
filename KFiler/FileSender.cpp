#include"FileSender.h"
FileSender::FileSender(const std::string& Port)
{
	MAIN_SERVER = std::make_unique<NetworkServer>(Port);
	MAIN_SERVER->Listen();
}
FileSender::~FileSender()
{
	FileServers.clear();
	if (MAIN_SERVER->IsConnected())
	{
		MAIN_SERVER->DisConnect();
	}
	MAIN_SERVER->DestroyServer();
}

std::vector<FileTransferer::FileStatus> FileSender::SendFile(std::weak_ptr<NetworkServer> server)
{
	return std::vector<FileStatus>();
}
void FileSender::IncreaseThread(const std::string& Port)
{
	UseablePorts.push(Port);
	MAX_THREAD_COUNT++;
}

void FileSender::DecreaseThread()
{
	if (UseablePorts.size() > 0)
	{
		UseablePorts.pop();
	}
	else if (FileServers.size() > 0)
	{
		if (!FileServers.back()->IsConnected())
		{
			return;
		}
	}
	if (MAX_THREAD_COUNT > 0)
	{
		MAX_THREAD_COUNT--;
	}
}

void FileSender::AddFile(const std::string& FilePath)
{
	PendingFiles.push(FilePath);
}

void FileSender::StopTransfer()
{
	ContinueTransfer = false;
}

void FileSender::StartTransfer()
{
	ContinueTransfer = true;
	int ThreadsCanBeUsed = 0;
	MAIN_SERVER->AcceptConnection();
	if (MAIN_SERVER->IsConnected())
	{
		//It waits for the response for how many threads Receiver can use
		while (true)
		{
			auto res = MAIN_SERVER->Receive();
			if (res)
			{
				ThreadsCanBeUsed = std::min(std::stoi(res.value()), MAX_THREAD_COUNT);
				break;
			}
		}
		std::string TmpDt = "";
		int Fss = (int)FileServers.size();
		for (int i = 0; i < ThreadsCanBeUsed; i++)
		{
			if (i < Fss)
			{
				TmpDt += FileServers[i]->GetListeningPort();
			}
			else
			{
				auto& TmpPort = UseablePorts.front();
				TmpDt += TmpPort;
				FileServers.emplace_back(std::make_shared<NetworkServer>(TmpPort));
				UseablePorts.pop();
			}
			if (i + 1 < ThreadsCanBeUsed)
			{
			  TmpDt += ";";
			}
		}
		MAIN_SERVER->Send(TmpDt);
		MAIN_SERVER->DisConnect();
	}
}
