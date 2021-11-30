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

std::vector<FileTransferer::FileStatus> FileSender::SendFile(std::shared_ptr<NetworkServer> server)
{
	USING_THREADS++;
	std::vector<FileStatus> FS;
	try
	{
		constexpr auto FileBufferSize = 1024;
		auto FileBuffer = std::make_unique<char[]>(FileBufferSize + 1);
		server->AcceptConnection();
		while (server->IsConnected() && ContinueTransfer)
		{
			//wait for response
			while (server->IsConnected() && !server->Receive().has_value());
			//popping from pending files getting the actual filename and sending to receiver
			const auto FilePath = PendingFiles.front();
			PendingFiles.pop();
			const auto fileName = ksTools::split_by_delms(FilePath, "/").back();
			std::ifstream FL(FilePath, std::ios::binary);
			const auto fileSize = GetFileSize(FL);
			server->Send(fileName + ';' + std::to_string(fileSize));
			//wait for response
			while (server->IsConnected() && !server->Receive().has_value());
			while (server->IsConnected() && FL.good())
			{
				FileBuffer.get()[FL.gcount()] = '\0';
				server->Send(FileBuffer.get());
				if (FL.tellg() == -1)
				{
					FS.emplace_back(fileName, fileSize, true);
				}
			}
			FL.close();
		}
		server->DisConnect();
	}
	catch (NetworkBuilder::Exception e)
	{
		//..
	}
	USING_THREADS--;
	return FS;
}
size_t FileSender::GetFileSize(std::ifstream& file)
{
	file.seekg(0, file.end);
	size_t len = file.tellg();
	file.seekg(0, file.beg);
	return size_t();
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
				ThreadsCanBeUsed = std::min(std::stoi(std::string(res.value())), MAX_THREAD_COUNT);
				break;
			}
		}
		std::string TmpDt = "";
		int Fss = (int)FileServers.size();
		auto Itr = FileServers.end();
		for (int i = 0; i < ThreadsCanBeUsed; i++)
		{
			if (i < Fss)
			{
				TmpDt += FileServers[i]->GetListeningPort();
				Itr = FileServers.begin() + i;
			}
			else
			{
				auto& TmpPort = UseablePorts.front();
				TmpDt += TmpPort;
				FileServers.emplace_back(std::make_shared<NetworkServer>(TmpPort));
				UseablePorts.pop();
				Itr = FileServers.end() - 1;
			}
			if (i + 1 < ThreadsCanBeUsed)
			{
			  TmpDt += ";";
			}
			std::packaged_task<std::vector<FileStatus>()> task(std::bind(&FileSender::SendFile, this, *Itr));
			StatusList.emplace_back(task.get_future());
			std::thread(std::move(task)).detach();
		}
		MAIN_SERVER->Send(TmpDt);
		MAIN_SERVER->DisConnect();
	}
}
