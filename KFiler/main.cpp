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
				ns.Send("Hello");
				ns.Send("World");
				ns.Send("MAMA MIA");
			}
			ns.DisConnect();
			ns.DestroyServer();
		};
		std::thread(Fun).detach();
		nc.Connect("127.0.0.1", "1024");
		if (nc.IsConnected())
		{
			std::cout << nc.Receive().value()
				      << nc.Receive().value()
				      << nc.Receive().value();

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