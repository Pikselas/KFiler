#include "FileReceiver.h"
FileReceiver::FileReceiver()
{
	MAIN_CLIENT = std::make_unique<NetworkClient>();
}

FileReceiver::ITRListType FileReceiver::ReceiveFile(std::shared_ptr<NetworkClient> client ,const std::string& port)
{
	USING_THREADS++;
	ITRListType List;
	try
	{
		client->Connect(Sender.first, port);
		while (client->IsConnected() && ContinueTransfer)
		{
			std::string Name;
			size_t FileSize = 0;
			client->Send("YS");
			while (client->IsConnected())
			{
				auto Dt = client->Receive();
				if (Dt)
				{
					auto res = ksTools::split_by_delms(std::string(Dt.value()), ";");
					Name = res.front();
					std::stringstream ss;
					ss << res.back();
					ss >> FileSize;
					break;
				}
			}
			client->Send("YS");
			mtx.lock();
			FileStatusList.emplace_back(Name, FileSize, 0);
			List.emplace_back(FileStatusList.end() - 1);
			mtx.unlock();
			std::ofstream FL(Name, std::ios::binary);
			size_t RecvCount = 0;
			while (client->IsConnected() && RecvCount < FileSize)
			{
				auto dt = client->Receive(TRANSFER_RATE);
				if (dt)
				{
					auto d = dt.value();
					RecvCount += d.second;
					List.back()->transferred = RecvCount;
					FL.write(d.first,d.second);					
				}
			}
			FL.close();
		}
		client->DisConnect();
	}
	catch (NetworkBuilder::Exception e)
	{
		
	}
	USING_THREADS--;
	return List;
}

void FileReceiver::IncreaseThread()
{
	MAX_THREAD_COUNT++;
}

void FileReceiver::DecreaseThread()
{
	if (MAX_THREAD_COUNT > 0)
	{
	   MAX_THREAD_COUNT--;
	}
}

void FileReceiver::SetSender(const std::string& IP , const std::string& PORT)
{
	Sender = std::make_pair(IP, PORT);
}

void FileReceiver::StopTransfer()
{
	ContinueTransfer = false;
}

void FileReceiver::StartTransfer()
{
	ContinueTransfer = true;
	
	//FileStatusList.clear();
	//FileStatusList.reserve(PendingFiles.size());

	MAIN_CLIENT->Connect(Sender.first, Sender.second);
	if (MAIN_CLIENT->IsConnected())
	{
		MAIN_CLIENT->Send(std::to_string(MAX_THREAD_COUNT));
		while (MAIN_CLIENT->IsConnected())
		{
			auto dt = MAIN_CLIENT->Receive();
			if (dt)
			{
				ksTools::split_by_delms(dt.value().data(), ";",PORTS);
				break;
			}
		}
		
		TransferReport.clear();
		TransferReport.reserve(PORTS.size());
		
		for (auto& prt : PORTS)
		{
			Clients.emplace_back(std::make_shared<NetworkClient>());
			std::packaged_task<ITRListType()> tsk(std::bind(&FileReceiver::ReceiveFile, this, Clients.back(),prt));
			TransferReport.emplace(prt, tsk.get_future());
			std::thread(std::move(tsk)).detach();
		}
		MAIN_CLIENT->DisConnect();
	}
}
