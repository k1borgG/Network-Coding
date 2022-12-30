#include <winsock2.h>
#include <iostream>
#include <string>
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#pragma warning(disable: 4996)
#pragma comment (lib,"Ws2_32.lib")
#define PORT 666
#define SERVERADDR "127.0.0.1"
using namespace std;

struct client {
    string fio;
    int money;
    int date;
} B;

int main() {
    char buff[1024];
    cout << "TCP BANK CLIENT\n";

    if (WSAStartup(0x202, (WSADATA*)&buff[0]))
    {
        cout << "WSAStart_error:\n" << WSAGetLastError();
        cin.get();   
        return -1;
    }

    SOCKET my_sock; my_sock = socket(AF_INET, SOCK_STREAM, 0);
    if (my_sock < 0)
    {
        cout << "Socket()_error:\n" << WSAGetLastError();
        cin.get();  
        return -1;
    }

    sockaddr_in dest_addr;
    dest_addr.sin_family = AF_INET;
    dest_addr.sin_port = htons(PORT);
    HOSTENT* hst;
    if (inet_addr(SERVERADDR) != INADDR_NONE)
        dest_addr.sin_addr.s_addr = inet_addr(SERVERADDR);
    else
        if (hst = gethostbyname(SERVERADDR))
            ((unsigned long*)&dest_addr.sin_addr)[0] = ((unsigned long**)hst->h_addr_list)[0][0];
        else {
            cout << "Invalid address \n" << SERVERADDR;
            closesocket(my_sock);    
            WSACleanup();
            cin.get();         
            return -1;
        }

    if (connect(my_sock, (sockaddr*)&dest_addr, sizeof(dest_addr)))
    {
        cout << "Connect error\n" << WSAGetLastError();
        cin.get();       
        return -1;
    }
    cout << "Соединение с " << SERVERADDR << " успешно установлено (Type quit for exit)\n\n";

    int nsize;
    while ((nsize = recv(my_sock, &buff[0], 80, 0)) != SOCKET_ERROR) 
    {
        buff[nsize] = '\0'; 
        cout << buff << endl;
        cout << "Enter your fio,amount and term of loan" << endl;
        string strr;
        getline(cin, strr);
        send(my_sock, (char*)&strr[0], strr.size(), 0);
        if (strr == "quit")  
        {
            cout << "Exit...";
            closesocket(my_sock);
            WSACleanup();
            return 0;
        }
    }
    cout << "Recv error\n" << WSAGetLastError();
    closesocket(my_sock);
    WSACleanup();
    cin.get();
    return -1;
}
