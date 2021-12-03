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

FileSender::ITRListType FileSender::SendFile(std::shared_ptr<NetworkServer> server)
{
	USING_THREADS++;
	ITRListType List;
	try
	{
		int FileBufferSize = TRANSFER_RATE;
		auto FileBuffer = std::make_unique<char[]>(FileBufferSize);
		server->Listen();
		server->AcceptConnection();
		while (server->IsConnected() && ContinueTransfer)
		{
			//wait for response
			while (server->IsConnected() && !server->Receive().has_value());
			//popping from pending files getting the actual filename and sending to receiver
			if (!PendingFiles.empty())
			{
				mtx.lock();
				const auto FilePath = PendingFiles.front();
				PendingFiles.pop();
				mtx.unlock();
				const auto fileName = ksTools::split_by_delms(FilePath, "/").back();
				std::ifstream FL(FilePath, std::ios::binary);
				const auto fileSize = GetFileSize(FL);
				server->Send(fileName + ';' + std::to_string(fileSize));
				mtx.lock();
				FileStatusList.emplace_back(fileName, fileSize, 0);
				List.emplace_back(FileStatusList.end() - 1);
				mtx.unlock();
				//wait for response
				while (server->IsConnected() && !server->Receive().has_value());
				while (server->IsConnected() && FL.good())
				{
					if (FileBufferSize != TRANSFER_RATE)
					{
						FileBufferSize = TRANSFER_RATE;
						FileBuffer = std::make_unique<char[]>(FileBufferSize);
					}
					FL.read(FileBuffer.get(), FileBufferSize);
					int ReadCount = (int)FL.gcount();
					List.back()->transferred += ReadCount;
					//using overloaded function so that no data get losts
					server->Send(FileBuffer.get(), ReadCount);
				}
				FL.close();
			}
			else
			{
				break;
			}
		}
		server->DisConnect();
	}
	catch (NetworkBuilder::Exception e)
	{
		//..
	}
	USING_THREADS--;
	return List;
}
size_t FileSender::GetFileSize(std::ifstream& file)
{
	file.seekg(0, file.end);
	size_t len = file.tellg();
	file.seekg(0, file.beg);
	return len;
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

	FileStatusList.clear();
	FileStatusList.reserve(PendingFiles.size());

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
				TransferReport.reserve(ThreadsCanBeUsed);
				break;
			}
		}
		std::string TmpDt = "";
		int Fss = (int)FileServers.size();
		auto Itr = FileServers.end();
		
		TransferReport.clear();
		TransferReport.reserve(ThreadsCanBeUsed);
		
		for (int i = 0; i < ThreadsCanBeUsed; i++)
		{
			std::string TmpPort;
			if (i < Fss)
			{
				TmpPort = FileServers[i]->GetListeningPort();
				Itr = FileServers.begin() + i;
			}
			else
			{
				TmpPort = UseablePorts.front();
				FileServers.emplace_back(std::make_shared<NetworkServer>(TmpPort));
				UseablePorts.pop();
				Itr = FileServers.end() - 1;
			}
			TmpDt += TmpPort;
			if (i + 1 < ThreadsCanBeUsed)
			{
			  TmpDt += ";";
			}
			std::packaged_task<ITRListType()> task(std::bind(&FileSender::SendFile, this, *Itr));
			TransferReport.emplace(TmpPort, task.get_future());
			std::thread(std::move(task)).detach();
		}
		MAIN_SERVER->Send(TmpDt);
		MAIN_SERVER->DisConnect();
	}
}
