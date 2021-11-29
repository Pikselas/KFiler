#include "FileReceiver.h"

FileReceiver::FileReceiver()
{
	MAIN_CLIENT = std::make_unique<NetworkClient>();
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
				PORTS = ksTools::split_by_delms(dt.value().data(), ";");
				break;
			}
		}
		MAIN_CLIENT->DisConnect();
	}
}
