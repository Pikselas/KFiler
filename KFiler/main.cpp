#include<iostream>
#include<thread>
#include"FileSender.h"
#include"FileReceiver.h"
int main()
{
	try
	{
		auto trnsfSize = 100;
		//creating a sender 
		FileSender fs("1234");
		
		//creating a receiver 
		FileReceiver fr;
		
		//adding file for sending
		fs.AddFile("C:/Users/Aritra Maji/Downloads/Umiko_Ahagon_X_Sakura_Nene.png");
		
		//increasing channel for sender with a port ->defaults to 0
		fs.IncreaseThread("1235");
		fs.SetTransferRate(trnsfSize);
		fr.SetTransferRate(trnsfSize);
		
		//increasing channel for receiver (no port need to be added for receiver)->dfaults to 0
		fr.IncreaseThread();

		//setting sender's address and port
		fr.SetSender(NetworkBuilder::GetDeviceIPs()[0], "1234");
		//sending file via child thread
		std::thread(&FileSender::StartTransfer,&fs).detach();
		
		//receiving file via main thread
		fr.StartTransfer();
		auto&[report,ftr] = *fs.GetTransferReport().begin();
		auto& lst = fs.GetFileStatusList();
		while (ftr.wait_for(std::chrono::seconds(0)) != std::future_status::ready)
		{
			if (!lst.empty())
			{
				system("cls");
				std::cout << lst.front().size << ":" << lst.front().transferred;
			}
		}
		std::cin.get();
		
	}
	catch (NetworkBuilder::Exception e)
	{
		std::cout << e.what();
	}
	catch (std::exception e)
	{
		std::cout << e.what();
	}
	return 0;
}