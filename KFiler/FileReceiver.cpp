#include "FileReceiver.h"
FileReceiver::FileReceiver()
{
	MAIN_CLIENT = std::make_unique<NetworkClient>();
}

size_t FileReceiver::ReceiveFile(std::shared_ptr<NetworkClient> client ,const std::string& port)
{
	USING_THREADS++;
	size_t count = 0;
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
			std::ofstream FL(Name, std::ios::binary);
			size_t RecvCount = 0;
			while (client->IsConnected())
			{
				auto dt = client->Receive(1024);
				if (dt)
				{
					auto d = dt.value();
					RecvCount += d.second;
					FL.write(d.first,d.second);
					if (RecvCount >= FileSize)
					{
						mtx.lock();
						StatusList.emplace_back(Name, FileSize, true);
						mtx.unlock();
						break;
					}
					
				}
			}
			FL.close();
		}
	}
	catch (NetworkBuilder::Exception e)
	{
		
	}
	USING_THREADS--;
	return count;
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
		for (auto& prt : PORTS)
		{
			Clients.emplace_back(std::make_shared<NetworkClient>());
			std::packaged_task<size_t()> tsk(std::bind(&FileReceiver::ReceiveFile, this, Clients.back(),prt));
			FileCountList.emplace_back(tsk.get_future());
			std::thread(std::move(tsk)).detach();
		}
		MAIN_CLIENT->DisConnect();
	}
}
