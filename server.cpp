
#include <iostream>
#include <winsock2.h>
#include <windows.h>

#pragma comment(lib, "ws2_32.lib")

#define PORT 5446
#define WINSOCK_API_INIT_ERROR 255

using namespace std;

int main() {
	WSADATA wsaData; // структура для хранения информации о конкретной реализации интерфейса Windows Sockets
	WORD DLLVersion = MAKEWORD(2, 1); // BYTE bLow(major), BYTE bHigh 
	int r;
	if (r = WSAStartup(DLLVersion, &wsaData)) { //Функция WSAStartup инициализирует WinSock. Эта функция всегда самая первая при начале работы с WinSock
		cout << "WSAStartup failed with error: %d\n" << r;
		return WINSOCK_API_INIT_ERROR;
	}

	SOCKET result = socket(AF_INET, SOCK_STREAM, 0); // создание сокета, AF_INET для протокола  IP, для передачи данных через канал связи с TCP
	SOCKADDR_IN sa; // Структура sockaddr_in описывает сокет для работы с протоколами IP
	memset(&sa, 0, sizeof(sa));
	sa.sin_family = AF_INET;
	sa.sin_port = htons(PORT);

	bind(result, (struct sockaddr*)&sa, sizeof(sa)); // привязка адреса к сокету
	listen(result, SOMAXCONN); // перевод созданного сокета в состояние, в котором он ожидает входящего соединения

	SOCKADDR_IN client_addr;
	int client_addr_size = sizeof(client_addr);

	cout << "SERVER\n";
	SOCKET client_socket;
	while (client_socket = accept(result, (struct sockaddr*)&client_addr, &client_addr_size)) { // ожидается входящее соединение
		cout << "Client Connected!\n";
		char file_name[50];
		cout << "Input file name\n";
		scanf("%s", &file_name);
		send(client_socket, file_name, sizeof(file_name), 0); // отправка
		char cheaker;
		recv(client_socket, &cheaker, sizeof(cheaker), 0); // получение
		if (cheaker == 1) cout << "OK\n";
		else cout << "NE_OK\n";
	}
	return EXIT_SUCCESS;
}
