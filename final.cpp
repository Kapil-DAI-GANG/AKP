#include <iostream>
#include<conio.h>
#include <fstream>
#include <ws2tcpip.h>
#include <WinSock2.h>
#include <windows.h>
#include<stdlib.h>
#include<ctime>
#include<iomanip>
#include<cstdlib>
#pragma comment(lib, "ws2_32.lib")
#pragma warning(disable:4996)
#pragma comment(lib, "user32")
#define _CRT_SECURE_NO_WARNINGS
void back();
void history();
void help();
void about();
COORD coordinate = { 0,0 };
void gotoxy(int x, int y)
{
	COORD coord;
	coord.X = x;
	coord.Y = y;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}
int barl = 20;
using namespace std;
class pBar {
public:
	void update(double newProgress) {
		currentProgress += newProgress;
		amountOfFiller = (int)((currentProgress / neededProgress) * (double)pBarLength);
	}
	void print() {
		currUpdateVal %= pBarUpdater.length();
		cout << "\r" << firstPartOfpBar;
		for (int a = 0; a < amountOfFiller; a++) {
			cout << pBarFiller;
		}
		cout << pBarUpdater[currUpdateVal];
		for (int b = 0; b < pBarLength - amountOfFiller; b++) {
			cout << " ";
		}
		cout << lastPartOfpBar
			<< " (" << (int)(100 * (currentProgress / neededProgress)) << "%)"
			<< flush;
		currUpdateVal += 1;
	}
	std::string firstPartOfpBar = "[",
		lastPartOfpBar = "]",
		pBarFiller = "|",
		pBarUpdater = "/-\\|";
private:
	int amountOfFiller,
		pBarLength = 50,
		currUpdateVal = 0;
	double currentProgress = 0,
		neededProgress = 100;
};
void dbase(char n[FILENAME_MAX])
{
	time_t a;
	struct tm* ti;
	time(&a);
	ti = localtime(&a);
	ofstream infile;
	infile.open("database.txt", ios::app);
	infile << asctime(ti);
	infile << setw(30);
	infile << n;
	infile << "\n";



}
int send()
{
	char x;
	system("cls");
	gotoxy(95, 28);
	cout << "THIS IS YOUR SERVER..." << endl;
	gotoxy(90, 29);
	cout << "(press y to continue and b to go back )" << endl;
	gotoxy(90, 30);
	cin >> x;
	cin.ignore();
	if (x == 'b')
	{
		system("cls");
		back();
	}
	else
	{
		char ans;
		gotoxy(90, 31);
		cout << "...do you want your ip (y/n)..." << endl;
		gotoxy(90, 32);
		cin >> ans;
		if (ans == 'y')
		{
			system("cls");

			system("C:/Windows/System32/ipconfig.exe");

			system("pause");
		}
		system("cls");
		gotoxy(90, 28);
		cout << "waiting..." << endl;
		WSADATA wsData;
		WORD ver = MAKEWORD(2, 2);
		if (WSAStartup(ver, &wsData) != 0) {
			gotoxy(60, 29);
			cerr << "Error starting winsock!" << endl;
			return -1;
		}

		SOCKET listenerSock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

		if (listenerSock == INVALID_SOCKET) {
			gotoxy(90, 30);
			cerr << "Error creating listener socket! " << WSAGetLastError() << endl;
			WSACleanup();
			return -1;
		}

		sockaddr_in listenerHint;
		listenerHint.sin_family = AF_INET;
		listenerHint.sin_port = htons(54000);
		listenerHint.sin_addr.S_un.S_addr = INADDR_ANY;

		bind(listenerSock, (sockaddr*)&listenerHint, sizeof(listenerHint));
		listen(listenerSock, SOMAXCONN);

		sockaddr_in clientHint;
		int clientSize = sizeof(clientHint);

		SOCKET clientSock = accept(listenerSock, (sockaddr*)&clientHint, &clientSize);

		if (clientSock == SOCKET_ERROR) {
			gotoxy(90, 31);
			cerr << "Error accept socket! " << WSAGetLastError() << endl;
			closesocket(listenerSock);
			WSACleanup();
			return -1;
		}

		char host[NI_MAXHOST];
		char serv[NI_MAXSERV];

		if (getnameinfo((sockaddr*)&clientHint, sizeof(clientHint), host, NI_MAXHOST, serv, NI_MAXSERV, 0) == 0) {
			system("cls");
			gotoxy(90, 28);
			cout << "Succesfully Connected !!!!!!!" << endl;
			cout << "Transfer your files and data at one go " << endl;
			cout << "Host: " << host << " connected on port: " << serv << endl;
		}
		else {
			inet_ntop(AF_INET, &clientHint.sin_addr, host, NI_MAXHOST);
			cout << "Succesfully Connected !!!!!!!" << endl;
			cout << "Transfer your files and data at one go " << endl;
			cout << "Host: " << host << " connected on port: " << ntohs(clientHint.sin_port) << endl << endl;
		}

		closesocket(listenerSock);

		const char* welcomeMsg = "Welcome to file server.";
		bool clientClose = false;
		char fileRequested[FILENAME_MAX];
		const int fileAvailable = 200;
		const int fileNotfound = 404;
		const int BUFFER_SIZE = 1024;
		char bufferFile[BUFFER_SIZE];
		std::ifstream file;

		// sending welcome message
		int bysendMsg = send(clientSock, welcomeMsg, strlen(welcomeMsg), 0);

		if (bysendMsg == 0) {
			// error sending data - break loop
			closesocket(clientSock);
			WSACleanup();
			return -1;
		}

		do {
			memset(fileRequested, 0, FILENAME_MAX);
			int byRecv = recv(clientSock, fileRequested, FILENAME_MAX, 0);
			dbase(fileRequested);
			if (byRecv == 0 || byRecv == -1) {
				// error receive data - break loop
				clientClose = true;
			}

			// open file
			file.open(fileRequested, ios::binary);

			if (file.is_open()) {
				// file is available
				int bySendinfo = send(clientSock, (char*)&fileAvailable, sizeof(int), 0);
				if (bySendinfo == 0 || bySendinfo == -1) {
					// error sending data - break loop
					clientClose = true;
				}

				// get file size
				file.seekg(0, ios::end);
				long fileSize = file.tellg();

				// send filesize to client
				bySendinfo = send(clientSock, (char*)&fileSize, sizeof(long), 0);
				if (bySendinfo == 0 || bySendinfo == -1) {
					// error sending data - break loop
					clientClose = true;
				}
				file.seekg(0, ios::beg);
				// read file with do-while loop
				do {
					// read and send part file to client
					file.read(bufferFile, BUFFER_SIZE);
					if (file.gcount() > 0)
					{
						bySendinfo = send(clientSock, bufferFile, file.gcount(), 0);
					}


					if (bySendinfo == 0 || bySendinfo == -1) {
						// error sending data - break loop
						clientClose = true;
						break;
					}
				} while (file.gcount() > 0);
				file.close();
				pBar bar;
				for (int i = 0; i < 100; i++) {
					bar.update(1); //How much new progress was added (only needed when new progress was added)
					//Print pBar:
					bar.print();
					Sleep(1);
				}
				cout << endl;
				system("cls");
				gotoxy(90, 28);
				cout << "\n transfer succesfull " << endl;
				cout << "\n Succesfully SENT file ::" << FILENAME_MAX;

			}

			else {
				// Can't open file or file not found
				int bySendCode = send(clientSock, (char*)&fileNotfound, sizeof(int), 0);
				if (bySendCode == 0 || bySendCode == -1) {
					// error sending data - break loop
					clientClose = true;
				}
			}
		} while (!clientClose);
	}
}

int receive()
{
	char x;
	system("cls");
	gotoxy(91, 22);
	cout << "THIS IS YOUR CLIENT.." << endl;
	gotoxy(91, 23);
	cout << "(press y to continue and b to go back )" << endl;
	gotoxy(90, 24);
	cin >> x;
	cin.ignore();
	if (x == 'b')
	{
		system("cls");
		back();
	}
	else
	{
		system("cls");
		gotoxy(90, 28);
		cout << "...THIS IS YOUR CLIENT..." << endl;
		WSADATA wsData;
		WORD ver = MAKEWORD(2, 2);

		if (WSAStartup(ver, &wsData) != 0) {
			gotoxy(90, 29);
			cerr << "Error starting winsock!" << endl;
			return -1;
		}

		SOCKET clientSock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

		if (clientSock == INVALID_SOCKET) {
			gotoxy(90, 30);
			cerr << "Error creating socket!, " << WSAGetLastError() << endl;
			return -1;
		}

		char serverAddress[NI_MAXHOST];
		memset(serverAddress, 0, NI_MAXHOST);
		gotoxy(90, 31);
		cout << "Enter server address: ";
		gotoxy(90, 32);
		cin.getline(serverAddress, NI_MAXHOST);
		gotoxy(90, 33);
		cin.ignore();

		sockaddr_in hint;
		hint.sin_family = AF_INET;
		hint.sin_port = htons(54000);
		inet_pton(AF_INET, serverAddress, &hint.sin_addr);

		char welcomeMsg[255];
		const int BUFFER_SIZE = 1024;
		char bufferFile[BUFFER_SIZE];
		char fileRequested[FILENAME_MAX];
		ofstream file;


		if (connect(clientSock, (sockaddr*)&hint, sizeof(hint)) == SOCKET_ERROR) {
			system("cls");
			gotoxy(90, 28);
			cerr << "Error connect to server!, " << WSAGetLastError() << endl;
			closesocket(clientSock);
			WSACleanup();
			return -1;
		}

		int byRecv = recv(clientSock, welcomeMsg, 255, 0);
		if (byRecv == 0 || byRecv == -1) {
			closesocket(clientSock);
			WSACleanup();
			return -1;
		}

		bool clientClose = false;
		int codeAvailable = 404;
		const int fileAvailable = 200;
		const int fileNotfound = 404;
		long fileRequestedsize = 0;
		do {
			int fileDownloaded = 0;
			memset(fileRequested, 0, FILENAME_MAX);
			gotoxy(90, 28);
			cout << "Enter file name: " << endl;
			cin.getline(fileRequested, FILENAME_MAX);

			byRecv = send(clientSock, fileRequested, FILENAME_MAX, 0);
			if (byRecv == 0 || byRecv == -1) {
				clientClose = true;
				break;
			}

			byRecv = recv(clientSock, (char*)&codeAvailable, sizeof(int), 0);
			if (byRecv == 0 || byRecv == -1) {
				clientClose = true;
				break;
			}
			if (codeAvailable == 200) {
				byRecv = recv(clientSock, (char*)&fileRequestedsize, sizeof(long), 0);
				if (byRecv == 0 || byRecv == -1) {
					clientClose = true;
					break;
				}

				file.open(fileRequested, ios::binary | ios::trunc);

				do {
					memset(bufferFile, 0, BUFFER_SIZE);
					byRecv = recv(clientSock, bufferFile, BUFFER_SIZE, 0);

					if (byRecv == 0 || byRecv == -1) {
						clientClose = true;
						break;
					}

					file.write(bufferFile, byRecv);
					fileDownloaded += byRecv;
				} while (fileDownloaded < fileRequestedsize);
				file.close();
				cout << endl;
				system("cls");
				gotoxy(90, 28);
				cout << "SUCCESSFULLY RECEIVED THE FILE " << FILENAME_MAX << endl;

			}
			else if (codeAvailable == 404) {
				gotoxy(90, 30);
				cout << "Can't open file or file not found!" << endl;
			}
		} while (!clientClose);
		closesocket(clientSock);
		WSACleanup();
		return 0;
	}
}

void back()
{
	system("cls");
	int j, sp = 39;
	char x;
	char ca;
	system("cls");
	gotoxy(90, 10);
	cout << "kAPIL DAI AND GANG EXPRESS" << endl;
	gotoxy(80, 15);
	cout << "\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2 SELECT MODES \xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2";
	gotoxy(80, 18);
	cout << "\xAF\xAF 1. SEND  \n";
	gotoxy(80, 21);
	cout << "\xAF\xAF 2. RECEIVE\n";
	gotoxy(80, 24);
	cout << "\xAF\xAF 3. VIEW TRANSACTION \n";
	gotoxy(80, 26);
	cout << "\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2";
	for (j = 15; j < 26; j++)
	{
		gotoxy(124, j);
		cout << "\xB2\xB2";
	}
	for (j = 16; j < 26; j++)
	{
		if (j != 18 && j != 21 && j != 24)
		{
			gotoxy(80, j);
			cout << "\xB2\xB2";
		}
	}
	gotoxy(93, 32);
	cout << "Press 'h' for help " << endl;
	gotoxy(93, 33);
	cout << "Press 'a' for about" << endl;
	gotoxy(93, 35);
	cout << "Press 'e' for exit" << endl;
	gotoxy(93, 36);

	cin >> x;
	if (x == '1')
	{
		send();
	}
	else if (x == '2')
	{
		receive();
	}
	else if (x == '3')
	{
		history();
	}
	else if (x == 'h')
	{
		help();
	}
	else if (x == 'a')
	{
		about();
	}
	else if (x == 'e')
	{
		exit(2);
	}
}
void history()
{
	string st;
	char std;
	int ij = 26;
	ifstream outfile;
	outfile.open("database.txt");
	system("cls");
	gotoxy(80, 20);
	cout << "DATABASE SERVER::::" << endl;
	gotoxy(80, 21);
	cout << "---------------------------------" << endl;
	while (!outfile.eof())
	{

		st = outfile.get();
		cout << st;
	}
	outfile.close();
	cout << "\n";
	system("pause");
	cout << "enter 'enter' key to return back " << endl;
	std = fgetc(stdin);
	if (std == 10)
	{
		system("cls");
		back();
	}

}
void help()
{
	system("cls");

	gotoxy(90, 20);
	cout << "~~~~~~~HELP~~~~~~~~~~~~ " << endl;
	cout << "\n";
	gotoxy(60, 25);
	cout << "1. The sender should be executed first followed by the receiver " << endl;
	gotoxy(60, 26);
	cout << "2. The files to be sent have to be saved in the same folder as that of 'main.cpp'." << endl;
	gotoxy(60, 27);
	cout << "3. For any further problems or queries, you can contact us to email : kapildaiorganization2014@gmail.com " << endl;
	gotoxy(60, 28);
	cout << "press any key to go to the main menu"<<endl;
	gotoxy(60, 29);
	getch();
	back();
}
void about()
{
	system("cls");
	gotoxy(100, 20);
	cout << "~~ABOUT~~~" << endl;
	gotoxy(60, 25);
	cout << "This is an open source program created by THE KAPIL DAI GANG for the 3rd semestar OOP Project.  " << endl;
	gotoxy(60, 26);
	cout << "You can update the code and make your changes and code can be found online in 'https://github.com/Kapil-DAI-GANG/AKP'" << endl;
	gotoxy(60, 27);
	cout << "TEAM MEMBERS:" << endl;
	gotoxy(60, 28);
	cout << "1. Aashish Chapain \t THA075BCT002" << endl;
	gotoxy(60, 29);
	cout << "2. Ashok Budha     \t THA075BCT013" << endl;
	gotoxy(60, 30);
	cout << "3. Bibek Khanal    \t THA075BCT016" << endl;
	gotoxy(60, 31);
	cout << "4. Kapil Shrestha  \t THA075BCT022" << endl;
	gotoxy(60, 32);
	cout << "ALL RIGHTS RESERVED ..." << endl;
	gotoxy(60, 33);
	cout << "press any key to go to the main menu" << endl;
	gotoxy(60, 34);
	getch();
	back();
}
int main()
{
	::SendMessage(::GetConsoleWindow(), WM_SYSKEYDOWN, VK_RETURN, 0x20000000);
	HANDLE hConsole;
	hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hConsole, 31);
	system("CLS");
	back();
}
