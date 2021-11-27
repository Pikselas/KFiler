#include<iostream>
#include<thread>
#include"FileSender.h"
#include"NetworkClient.h"
int main()
{
	try
	{
		FileSender fs("1234");
		fs.IncreaseThread("1235");
		fs.IncreaseThread("1236");
		std::thread(&FileSender::StartTransfer,&fs).detach();
		NetworkClient nc;
		nc.Connect("127.0.0.1","1234");
		if (nc.IsConnected())
		{
			nc.Send("3");
			while (true)
			{
				auto dt = nc.Receive();
				if (dt)
				{
					std::cout << dt.value();
					break;
				}
			}
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