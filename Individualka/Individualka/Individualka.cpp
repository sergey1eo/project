#include <stdio.h>
#pragma comment(lib, "WSock32.lib")
#include<winsock2.h>
#include <iostream>

#pragma warning (disable: 4996)
using namespace std;
SOCKET Connections[100];// массив для подключения сразу нескольских клиентов
int Counter = 0; // индекс соединения

void ClientHandler(int index)
{
	char msg[256];
	while (true)
	{
		recv(Connections[index], msg, sizeof(msg), NULL);// принятие сообщения
		for (int i = 0; i < Counter; i++)//отправка сообщений(кроме отправщика)
		{
			if (i==index)// чтобы сообщения не отправлялось дважды
			{
				continue; 
			}
			send(Connections[i], msg, sizeof(msg), NULL);// отправка остальным клиентам
		}
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

	SOCKET slisten = socket(AF_INET, SOCK_STREAM, NULL);// передача протоколов
	bind(slisten, (SOCKADDR*)&addr, sizeof(addr));// указатель на структуру и размер струкутуры
	listen(slisten, SOMAXCONN);// определение по какому порту нужно запустить
	                          // прослушивание и максимальное число запросов в обработке
	SOCKET newConnection;
	for (int i = 0; i < 100; i++)
	{
		newConnection = accept(slisten, (SOCKADDR*)&addr, &sizeofaddr);// указательна структуру
																  // и размер структуры
		if (newConnection == 0) // проверка на подключение клиента
		{
			cout << "Ошибка!" << endl;
		}
		else
		{
			cout << "Клиент присоединился" << endl;
			char msg[256] = "Привет, это моя сетевая прогармма)";
			send(newConnection, msg, sizeof(msg), NULL);

			Connections[i] = newConnection;
			Counter++;
			CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)ClientHandler, (LPVOID)(i), NULL, NULL);// работа в два потока

			
		}
	}

	system("pause");
	return 0;
}