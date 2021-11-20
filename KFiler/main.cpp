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
		std::thread(std::bind(&NetworkServer::AcceptConnection,&ns)).detach();
		nc.Connect("127.0.0.1", "1024");
		if (nc.IsConnected())
		{
			std::cout << "hello";
		}
	}
	catch (NetworkBuilder::Exception e)
	{
		std::cout << e.what();
	}
	return 0;
}