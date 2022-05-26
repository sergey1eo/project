#include <stdio.h>
#pragma comment(lib, "WSock32.lib")
#include<winsock2.h>
#include <iostream>
#pragma warning (disable: 4996)
using namespace std;

SOCKET Connection;

void ClientHandler()// принятие сервером сообщения 
{
	char msg[256];
	while (true)
	{
		recv(Connection, msg, sizeof(msg), NULL);
		cout << msg << endl;
	}
}

int main(int argc, char* argv[])
{
	setlocale(LC_ALL, "");
	WSAData wsaData;//Создание структуры
	WORD DLLVersion = MAKEWORD(2, 1);//Загрузка библиотеки
	if (WSAStartup(DLLVersion, &wsaData) != 0)//1 параметр - передается запрашиваемая библиотека 
	{										  //2 параметр - ссылка на структуру wsaData
		cout << "Ошибка!" << endl;
		exit(1);
	}
	SOCKADDR_IN addr; // хранение адреса(интернет протокол)
	int sizeofaddr = sizeof(addr);
	addr.sin_addr.s_addr = inet_addr("127.0.0.1");// хранение IP адреса
	addr.sin_port = htons(1111);// порт для идентификации
	addr.sin_family = AF_INET;// семейство протоколов
	
    Connection = socket(AF_INET, SOCK_STREAM, NULL);
	if (connect(Connection, (SOCKADDR*)&addr, sizeof(addr)) != 0)// присоединение к серверу
	{
		cout << "Не удается подключиться к серверу" << endl;
		return 1;
	}
	else
	{
		cout << "Подкючено!" << endl;
	}

		CreateThread(NULL, NULL,(LPTHREAD_START_ROUTINE)ClientHandler, NULL, NULL, NULL);// новый поток
		
		char msg1[256]; // хранение сообщения
		while (true)
		{
			cin.getline(msg1, sizeof(msg1));// запись строки
			send(Connection, msg1, sizeof(msg1),NULL);// отправка сообщения
			Sleep(10);
		}
	


	system("pause");
	return 0;
}
