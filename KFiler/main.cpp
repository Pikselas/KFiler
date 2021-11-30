#include<iostream>
#include<thread>
#include"FileSender.h"
#include"FileReceiver.h"
int main()
{
	try
	{
		FileSender fs("1234");
		FileReceiver fr;
		
		fs.IncreaseThread("1235");
		fs.IncreaseThread("1236");
		
		fr.IncreaseThread();
		fr.IncreaseThread();
		fr.SetSender(NetworkBuilder::GetDeviceIPs()[0], "1234");
		
		std::thread(&FileSender::StartTransfer,&fs).detach();

		fr.StartTransfer();

		for (auto& prt : fr.PORTS)
		{
			std::cout << prt << std::endl;
		}
		
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