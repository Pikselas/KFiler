#include"NetworkBuilder.h"
#include<iostream>
int main()
{
	for (auto x : NetworkBuilder::GetDeviceIPs())
	{
		std::cout << x << std::endl;
	}
	return 0;
}