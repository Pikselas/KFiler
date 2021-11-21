#include<iostream>
#include<thread>
#include<functional>
#include"NetworkClient.h"
#include"NetworkServer.h"
int main()
{
	try
	{
		NetworkServer ns("1024");
		NetworkClient nc;
		ns.Listen();
		auto Fun = [&] {
			ns.AcceptConnection();
			if (ns.IsConnected())
			{
				ns.Send(ns.GetClientIP());
			}
			ns.DisConnect();
			ns.DestroyServer();
		};
		std::thread(Fun).detach();
		nc.Connect(NetworkBuilder::GetDeviceIPs()[0], "1024");
		if (nc.IsConnected())
		{
			std::cout << nc.Receive().value();

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