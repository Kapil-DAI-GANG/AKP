#include"header.h"
#include <fstream>
using namespace std;
int main()
{
	int c, d;
	string file, ip;
	COORD co = { 0,0 };
	HANDLE Screen;
	Screen = GetStdHandle(STD_OUTPUT_HANDLE);
	COORD max = GetLargestConsoleWindowSize(Screen);
	co.X = (max.X - 12) / 2;
	co.Y = max.Y / 2;
	SetConsoleCursorPosition(Screen, co);
	cout << "1) SEND" << endl;
	co.X = (max.X - 12) / 2;
	co.Y = max.Y / 2 + 1;
	SetConsoleCursorPosition(Screen, co);
	cout << "2) RECEIVE" << endl;
	cout << "please select :" << endl;
	cout << ">> ";
	cin >> c;
	system("cls");
	if (c == 1)
	{
		cout << "SELECT THE TYPE OF DATA TO SEND:" << endl;
		cout << "1) TEXT FILE" << endl;
		cout << "2) IMAGE FILE" << endl;
		cout << "3) AUDIO FILE" << endl;

		cout << "select >>" << endl;
		cin >> d;
		system("cls");
		if (d == 1)
		{
			cout << "enter the text file path:";
			cin>> file;
			cout << endl << "enter the server ip-address:";
			cin>> ip;
			system("pause");
			system("cls");

			string ipaddress = ip;  //ip address of the server
			int port = 54000;				   //listening port no in the server

			//initialize winsock....

			WSADATA data;
			int result = WSAStartup(MAKEWORD(2, 2), &data);
			if (result != 0)
			{
				cerr << ".....Windows socket startup failed....." << endl;
				return 0;
			}


			//create a socket....

			SOCKET calling = socket(AF_INET, SOCK_STREAM, 0);
			if (calling == INVALID_SOCKET)
			{
				cerr << "....Error creating a socket...." << endl;
				WSACleanup();
				return 0;
			}


			//fill in the hint structure....

			sockaddr_in cadd;
			cadd.sin_family = AF_INET;
			cadd.sin_port = htons(port);
			inet_pton(AF_INET, ipaddress.c_str(), &cadd.sin_addr);

			//connect to the server....
			cout << "waiting..." << endl;
			int conresult = connect(calling, (sockaddr*)&cadd, sizeof(cadd));
			if (conresult == SOCKET_ERROR)
			{
				cerr << "....connection unsuccessful...." << endl << "#<run the server first>" << endl;
				closesocket(calling);
				WSACleanup();
				return 0;

			}
			system("cls");
			cout << "****YOU ARE CONNECTED TO THE SERVER****" << endl;

			//for text....

			if (d == 1)
			{
				char buf[4096];
				string line;

				
					//promt the user for some text....

					//cout << ">>>> ";
					//getline(cin, userinput);

					ifstream myfile(file);
					if (myfile.is_open())
					{
						while (getline(myfile, line))
						{

							//sending data....

							int sresult = send(calling, line.c_str(), line.size() + 1, 0);
							if (sresult != SOCKET_ERROR)
							{
								//wait for response....
								ZeroMemory(buf, 4096);

								int bytereceived = recv(calling, buf, 4096, 0);
								if (bytereceived > 0)
								{
									//echo response to consle....
									//cout << "<SERVER> " << string(buf, 0, bytereceived) << endl;
								}


							}

						}
						myfile.close();
					}

					else
					{
						cout << "Unable to open file";

					}
			}
			cout << ".....TRANSFER PROCESS COMPLETED....." << endl;
			system("pause");
			//close down everything....
			closesocket(calling);
			WSACleanup();
		}






	}



	//for receiving....
	else if (c == 2)
	{
		cout << "SELECT THE TYPE OF DATA TO RECEIVE:" << endl;
		cout << "1) TEXT FILE" << endl;
		cout << "2) IMAGE FILE" << endl;
		cout << "3) AUDIO FILE" << endl;

		cout << "select >>" << endl;
		cin >> d;
		cout << "!---the sender and receiver should be connected to the same network...." << endl;
		cout << "!---also supply your ipv4-address to the sender...." << endl;
		system("pause");
		system("cls");
		if (d == 1)
		{
			WSADATA wsaData;
			int result;

			//initializing winsock....

			result = WSAStartup(MAKEWORD(2, 2), &wsaData);
			if (result != 0)
			{
				cerr << ".....Windows socket startup failed....." << endl;
				WSACleanup();
				return 0;
			}
			else
			{
				cerr << ".....Windows socket startup successful....." << endl;
			}

			//create a socket

			SOCKET listening = socket(AF_INET, SOCK_STREAM, 0);
			if (listening == INVALID_SOCKET)
			{
				cerr << "....Error creating a socket...." << endl;
				closesocket(listening);
				WSACleanup();
				return 0;
			}
			else
			{
				cout << "....socket initialization complete...." << endl;
			}

			//bind the ip address and port to the socket

			sockaddr_in add; //it is a structure that stores the socket parameter data
			add.sin_family = AF_INET;  //this stores the protocol used in the socket
			add.sin_port = htons(54000); //it stores the port no for the socket 
			add.sin_addr.S_un.S_addr = INADDR_ANY; // it helps to bind the socket to any type of address

			bind(listening, (sockaddr*)&add, sizeof(add));

			//tell winsock the socket is for listening

			if (listen(listening, SOMAXCONN) == SOCKET_ERROR)
			{
				cerr << "....listening of client unsuccessful...." << endl;
				closesocket(listening);
				WSACleanup();
				return 0;
			}
			else
			{
				cout << "....listening...." << endl;
			}

			//after lsitening.......

			//wait for a connection

			sockaddr_in client;
			//int clientSize = sizeof(client);
			SOCKET clientSocket = accept(listening, NULL, NULL);
			if (clientSocket == INVALID_SOCKET)
			{
				cerr << "....accept failed...." << endl;
				closesocket(listening);
				WSACleanup();
				return 0;
			}
			else
			{
				cout << "....CLIENT CONNECTIONS SUCCESSFUL.... \n\n\n\n" << endl;
			}



			//close listening socket

			closesocket(listening);

			//saving the sent data to file....

			if (d == 1)
			{
				char buf[4096];

				
					ZeroMemory(buf, 4096);

					// wait for the client to send data....
					int bytesreceived = recv(clientSocket, buf, 4096, 0);
					if (bytesreceived == SOCKET_ERROR)
					{
						cerr << "....error while receiving data client disconnected...." << endl;
						
					}

					if (bytesreceived == 0)
					{
						cout << "....client disconnected...." << endl;
						
					}
					//write to file....

					fstream new_file;
					new_file.open("received_file.txt", ios::out);
					if (!new_file)
					{
						cout << "File creation failed";
					}
					else
					{

						new_file << buf;    //Writing to file
						new_file.close();
					}
					send(clientSocket, buf, bytesreceived + 1, 0);
				


			}
			cout << ".....TRANSFER PROCESS COMPLETED....." << endl;
			system("pause");
			// cleanup
			closesocket(clientSocket);
			WSACleanup();
			return 0;
		}
	}
}


