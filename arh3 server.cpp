// arh3 server.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include <cstdio>
#include <iostream>
#include <string>
#include <utility>
#include <fstream>

#include <map>

#include <winsock.h>





using std::map;
using std::string;
using std::make_pair;
using namespace std;



string handleRequest(string s, string t) {
	if(s=="Who"){
		return "Kirill Tsybulkin, K-26, Variant 13";

	}
	if (s == "q")
	{
		return "q";
	}
	if (s.length() != t.length()) {
		return "correct len is "+to_string(t.length());
	}
	int counter = 0;
	for (int i = 0; i < s.length(); i++)
	{
		if (s[i] == t[i])
		{
			counter++;
		}
	}
	if (counter == t.length())
	{
		return "correct";
	}
	string result = to_string(counter);
	result.append(" of ");
	result.append(to_string(t.length()));
	result.append(" is correct");
	return result;
}



int main(int argc, char* argv[])
{
	std::ofstream outfile;

	outfile.open("server.txt", std::ios_base::out); // append instead of overwrite
	outfile << "server log\n";
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

	if (bind(s, (SOCKADDR*)& sa, sizeof(sa)) == SOCKET_ERROR)
		return -1;

	SOCKADDR_IN nsa;
	int sizeof_nsa = sizeof(nsa);

	char buffer[1024];
	int buffer_size = sizeof(buffer);
	string password;
	cout << "use 0 to 9 digits to make password\n";
	cin >> password;
	/*
	string mess;
	mess = "len of password is ";
	mess.append(to_string(password.length()));
	if (sendto(s, mess.c_str(), mess.length() + 1, 0, (SOCKADDR*)& nsa, sizeof(nsa)) == SOCKET_ERROR)
		return -1;
		*/
	bool work = true;
	while (work) {
		if (recvfrom(s, buffer, buffer_size, 0, (SOCKADDR*)& nsa, &sizeof_nsa) == SOCKET_ERROR)
			return -1;
		outfile << "request: " << buffer << "\n";
		string response = handleRequest(string{ buffer }, password);
		if (response != "q") {
			outfile << "response: " << response << "\n\n";
			if (sendto(s, response.c_str(), response.length() + 1, 0, (SOCKADDR*)& nsa, sizeof(nsa)) == SOCKET_ERROR)
				return -1;
		}
		else
		{
			work = false;
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
