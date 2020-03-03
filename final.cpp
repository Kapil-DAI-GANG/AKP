#include <iostream>
#include<conio.h>
#include <fstream>
#include <ws2tcpip.h>
#include <WinSock2.h>
#include <windows.h>
#include<stdlib.h>
#include<ctime>
#include<iomanip>
#pragma comment(lib, "ws2_32.lib")
#define _CRT_SECURE_NO_WARNINGS
void back();
void history();
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
	infile.open("database.txt");
	infile << asctime(ti);
	infile << setw(30);
	infile << n;
	infile << "/n";



}

int send()
{
	char x;
	system("cls");
	cout << "THIS IS YOUR SERVER..." << endl;
	cout << "(press y to continue and b to go back )" << endl;
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
		cout << "waiting..." << endl;
		WSADATA wsData;
		WORD ver = MAKEWORD(2, 2);

		if (WSAStartup(ver, &wsData) != 0) {
			cerr << "Error starting winsock!" << endl;
			return -1;
		}

		SOCKET listenerSock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

		if (listenerSock == INVALID_SOCKET) {
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
			cerr << "Error accept socket! " << WSAGetLastError() << endl;
			closesocket(listenerSock);
			WSACleanup();
			return -1;
		}

		char host[NI_MAXHOST];
		char serv[NI_MAXSERV];

		if (getnameinfo((sockaddr*)&clientHint, sizeof(clientHint), host, NI_MAXHOST, serv, NI_MAXSERV, 0) == 0) {
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
	cout << "THIS IS YOUR CLIENT.." << endl;
	cout << "(press y to continue and b to go back )" << endl;
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
		cout << "...THIS IS YOUR CLIENT..." << endl;
		WSADATA wsData;
		WORD ver = MAKEWORD(2, 2);

		if (WSAStartup(ver, &wsData) != 0) {
			cerr << "Error starting winsock!" << endl;
			return -1;
		}

		SOCKET clientSock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

		if (clientSock == INVALID_SOCKET) {
			cerr << "Error creating socket!, " << WSAGetLastError() << endl;
			return -1;
		}

		char serverAddress[NI_MAXHOST];
		memset(serverAddress, 0, NI_MAXHOST);

		cout << "Enter server address: ";
		cin.getline(serverAddress, NI_MAXHOST);
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
				/*pBar bar;
				for (int i = 0; i < 100; i++) {
					bar.update(1); //How much new progress was added (only needed when new progress was added)
					//Print pBar:
					bar.print(); //This should be called more frequently than it is in this demo (you'll have to see what looks best for your program)
					Sleep(1);
				}*/
				cout << endl;
				cout << "SUCCESSFULLY RECEIVED THE FILE " << FILENAME_MAX << endl;

			}
			else if (codeAvailable == 404) {
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
	int x;
	cout << "1) send" << endl;
	cout << "2) receive" << endl;
	cout << "3) Display Transactions" << endl;
	cout << ">>";
	cin >> x;
	cin.ignore();
	if (x == 1)
	{
		send();
	}
	else if (x == 2)
	{
		receive();
	}
	else if (x == 3)
	{
		history();
	}
}
void history()
{
	string st;

	ifstream outfile;
	outfile.open("database.txt");
	while (!outfile.eof())
	{
		st= outfile.get();
		cout << st;
	}
	outfile.close();


}
int main()
{
	back();

}
