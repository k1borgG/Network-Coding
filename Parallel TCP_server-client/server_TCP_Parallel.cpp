#include <iostream>
#include <winsock2.h>  
#include <string>
#include <sstream>
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#pragma warning(disable: 4996)
#pragma comment (lib,"Ws2_32.lib")
using namespace std;

u_short MY_PORT = 666;        

#define PRINTNUSERS if (nclients)\
cout << " user online " << nclients <<endl;\
  else cout << "No User online\n";

DWORD WINAPI ConToClient(LPVOID client_socket);

int nclients = 0;

struct client {
    string fio;
    int money;
    int date;
} A;

int main() {
    char buff[1024];   
    cout << "TCP SERVER - loan calculator\n";

    if (WSAStartup(0x0202, (WSADATA*)&buff[0]))
    {
        cout << "Error WSAStartup \n" << WSAGetLastError();
        return -1;
    } 

    SOCKET mysocket;
    if ((mysocket = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {     
        cout << "Error socket \n" << WSAGetLastError();
        WSACleanup();  
        return -1;
    }

    sockaddr_in local_addr;
    local_addr.sin_family = AF_INET;
    local_addr.sin_port = htons(MY_PORT);
    local_addr.sin_addr.s_addr = 0;

    if (bind(mysocket, (sockaddr*)&local_addr, sizeof(local_addr)))
    {   
        cout << "Error bind \n" << WSAGetLastError();
        closesocket(mysocket);  
        WSACleanup();
        return -1;
    }

    if (listen(mysocket, 0x100))
    {    
        cout << "Error listen: " << WSAGetLastError();
        closesocket(mysocket);
        WSACleanup();
        return -1;
    }

    cout << "Waiting connections\n";

    SOCKET client_socket;    
    sockaddr_in client_addr;   
    int client_addr_size = sizeof(client_addr);

    while ((client_socket = accept(mysocket, (sockaddr*)&client_addr, &client_addr_size))) //извлечение запросов из очереди
    {
        nclients++;  
        HOSTENT* hst; 
        hst = gethostbyaddr((char*)&client_addr.sin_addr.s_addr, 4, AF_INET);
        cout << "+new connect!\n";   
        if (hst)
            cout << hst->h_name;
        else
            cout << "";
        cout << inet_ntoa(client_addr.sin_addr);
        PRINTNUSERS cout << endl;
        DWORD thID;
        CreateThread(NULL, NULL, ConToClient, &client_socket, NULL, &thID);
    }
    return 0;
}

DWORD WINAPI ConToClient(LPVOID client_socket)
{
    SOCKET my_sock;   int len;
    my_sock = ((SOCKET*)client_socket)[0];
    char buff[1024];
    char sHELLO[] = "Welcome to loan calc \r\n";
    send(my_sock, sHELLO, sizeof(sHELLO), 0);
    while (SOCKET_ERROR != (len = recv(my_sock, (char*)&buff[0], 1024, 0)))
    {
        buff[len] = '\0';
        istringstream s(buff);
        s >> A.fio;
        s >> A.money;
        s >> A.date;

        cout << "getted: " << buff << endl;

        double p = 1;
        int d = 0;
        string answ;

        if (A.date > 12)
            p += 0.25;
        else p += 0.5;
        if (A.money > 1000000)
            p += 0.5;
        else
            p += 1;
        d = (A.money * p) / A.date;
        string ds = to_string(d);
        answ = "Your monthly payment = " + ds;
        if (A.fio!="quit")
            cout << answ << endl << endl;
        send(my_sock, answ.c_str(), answ.size(), 0);
    }
    nclients--; 
    cout << "-disconnect\n";
    PRINTNUSERS closesocket(my_sock);
    return 0;
}