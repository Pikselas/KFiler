#include<iostream>
#include<thread>
#include"FileSender.h"
#include"FileReceiver.h"
int main()
{
	try
	{
		std::string str;
		int ch;
		std::string port = "1024";
		std::unique_ptr<FileSender> FS = nullptr;
		std::unique_ptr<FileReceiver> FR = nullptr;
		FileTransferer* FT = nullptr;
		std::cout << "1.SEND\n2.RECEIVE\n";
		std::cin >> ch;
		if(ch == 1)
		{
			FS = std::make_unique<FileSender>(port);
			while (ch != 5)
			{
				system("cls");
				std::cout << "1.ADD FILE\n"
					<< "2.INCRESE CHANNELS\n"
					<< "3.DECREASE CHANNELS\n"
					<< "4.SET TRANSFER RATE\n"
					<< "5.START TRANSFER\n";
				std::cin >> ch;
				switch (ch)
				{
				case 1:
					std::cout << "ENTER FILEPATH:";
					std::cin >> str;
					FS->AddFile(str);
					break;
				case 2:
					std::cout << "ENTER PORT:";
					std::cin >> str;
					FS->IncreaseThread(str);
					break;
				case 3:
					FS->DecreaseThread();
					break;
				case 4:
					std::cout << "ENTER BYTE COUNTS:";
					std::cin >> ch;
					FS->SetTransferRate(ch);
					ch = 4;
					break;
				case 5:
					std::cout << "TRY CONNECTING ONE OF THESE IPs:\n";
					for (auto& ip : NetworkBuilder::GetDeviceIPs())
					{
						std::cout << ip << "\n";
					}
					std::cout << "WAITING FOR CONNECTION...";
					FS->StartTransfer();
					break;
				}
			}
			FT = FS.get();
		}
		else if (ch == 2)
		{
			std::string sender;
			FR = std::make_unique<FileReceiver>();
			while (ch != 5)
			{
				system("cls");
				std::cout << "1.SET SENDER\n"
					<< "2.INCREASE CHANNELS\n"
					<< "3.DECREASE CHANNELS\n"
					<< "4.SET TRANSFER RATE\n"
					<< "5.START TRANSFER\n";
				std::cin >> ch;
				switch (ch)
				{
				case 1:
					std::cin >> sender;
					FR->SetSender(sender, port);
					break;
				case 2:
					FR->IncreaseThread();
					break;
				case 3:
					FR->DecreaseThread();
					break;
				case 4:
					std::cout << "ENTER BYTE COUNTS\n";
					std::cin >> ch;
					FR->SetTransferRate(ch);
					ch = 4;
					break;
				case 5:
					FR->StartTransfer();
					break;
				}
			}
			FT = FR.get();
		}
		if (FT != nullptr)
		{
			auto& rpt = FT->GetTransferReport();
			auto& StatList = FT->GetFileStatusList();
			bool Completed = false;
			while (!Completed)
			{
				system("cls");
				for (auto& [prt, ftr] : rpt)
				{
					if (ftr.wait_for(std::chrono::seconds(0)) != std::future_status::ready)
					{
						Completed = false;
						break;
					}
					else
					{
						Completed = true;
					}
				}
				for (auto& stats : StatList)
				{
					std::cout << stats.name << "\n" << stats.size << ":" << stats.transferred << "\n\n";
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
	std::cin.get();
	std::cin.get();
	return 0;
}