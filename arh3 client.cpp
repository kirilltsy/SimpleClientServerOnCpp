// arh3 client.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//
#include <winsock.h>
#include <stdio.h>
#include <string>
#include <iostream>
#include <time.h>
#include <fstream>

using namespace std;
string password(int cur, int len)
{
	string res = "";
	for (int i = 0; i < len; i++)
	{
		res.append(to_string(cur % 10));
		cur /= 10;
	}
	return res;
}



int main(int argc, char* argv[])
{
	std::ofstream outfile;

	outfile.open("client.txt", std::ios_base::out); // append instead of overwrite
	outfile << "client log\n";
	WSADATA wsa_data;
	if (WSAStartup(0x0202, &wsa_data) || wsa_data.wVersion != 0x0202)
		return -1;

	SOCKET s = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if (s == INVALID_SOCKET)
		return -1;

	SOCKADDR_IN sa;
	sa.sin_family = AF_INET;
	sa.sin_port = htons(1038);
	sa.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");
	int sizeof_sa = sizeof(sa);

	SOCKADDR_IN nsa;
	int sizeof_nsa = sizeof(nsa);

	string message;
	cout << "use 0 to 9 digits to guess the password\n";
	/*
	char buffer[1024];
	if (recvfrom(s, buffer, sizeof(buffer), 0, (SOCKADDR*)& nsa, &sizeof_nsa) == SOCKET_ERROR)
		return -1;
	std::cout << buffer << '\n';
	*/


	int autopass = 0;
	bool work = true;
	while (work) {
		std::cout << "message =  ";
		getline(std::cin, message);
		if (message == "q")
		{
			work = false;
			if (sendto(s, message.c_str(), message.length() + 1, 0, (SOCKADDR*)& sa, sizeof_sa) == SOCKET_ERROR)
				return -1;
		}
		else {
			if (message == "auto")
			{

				cout << "How many seconds?\n";
				clock_t dclock;
				float restTime = 10.0;
				cin >> restTime;
				float fullTime = restTime;
				cout << "How many symbols?\n";
				int l = 0;
				cin >> l;
				outfile << "command: " << "auto "<<fullTime<<" s; "<<"len of password: "<<l << "\n";
				int counter = 0;
				dclock = clock();
				while (restTime > 0) {
					dclock = clock();
					//cout << restTime << "\n";
					string message = password(autopass, l);
					//cout << message<<"\n";
					if (sendto(s, message.c_str(), message.length() + 1, 0, (SOCKADDR*)& sa, sizeof_sa) == SOCKET_ERROR)
						return -1;
					char buffer[1024];
					if (recvfrom(s, buffer, sizeof(buffer), 0, (SOCKADDR*)& nsa, &sizeof_nsa) == SOCKET_ERROR)
						return -1;
					outfile << "request: " << message << "\n";
					outfile << "response: " << buffer << "\n";
					string win = buffer;
					//cout << win << "\n";
					if (win == "correct")
					{
						cout << "correct password is " << message << "\n";
						break;
					}
					dclock = clock() - dclock;
					//cout << ((float)dclock) / CLOCKS_PER_SEC << "\n";
					restTime -= ((float)dclock) / CLOCKS_PER_SEC;
					counter++;
					autopass++;
				}
				cout << counter << " attempts in " << fullTime - restTime << " seconds\n";
				outfile << "\n";
			}


			else {
				if (sendto(s, message.c_str(), message.length() + 1, 0, (SOCKADDR*)& sa, sizeof_sa) == SOCKET_ERROR)
					return -1;
				char buffer[1024];
				outfile << "command: " << "password" << "\n";
				outfile << "request: " << message << "\n";
				if (recvfrom(s, buffer, sizeof(buffer), 0, (SOCKADDR*)& nsa, &sizeof_nsa) == SOCKET_ERROR)
					return -1;
				std::cout << buffer << '\n';
				outfile << "response: " << buffer << "\n\n";
			}
		}
	}
	outfile.close();
	closesocket(s);
	WSACleanup();

	system("pause");
	return 0;
}



// Запуск программы: CTRL+F5 или меню "Отладка" > "Запуск без отладки"
// Отладка программы: F5 или меню "Отладка" > "Запустить отладку"

// Советы по началу работы 
//   1. В окне обозревателя решений можно добавлять файлы и управлять ими.
//   2. В окне Team Explorer можно подключиться к системе управления версиями.
//   3. В окне "Выходные данные" можно просматривать выходные данные сборки и другие сообщения.
//   4. В окне "Список ошибок" можно просматривать ошибки.
//   5. Последовательно выберите пункты меню "Проект" > "Добавить новый элемент", чтобы создать файлы кода, или "Проект" > "Добавить существующий элемент", чтобы добавить в проект существующие файлы кода.
//   6. Чтобы снова открыть этот проект позже, выберите пункты меню "Файл" > "Открыть" > "Проект" и выберите SLN-файл.
