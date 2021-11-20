#include"NetworkClient.h"
#include<iostream>
int main()
{
	NetworkClient nc;
	try
	{
		nc.Connect("127.0.0.1", "80");
	}
	catch (NetworkBuilder::Exception e)
	{
		std::cerr << e.what();
	}
	return 0;
}