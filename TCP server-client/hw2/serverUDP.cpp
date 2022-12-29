#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <winsock2.h>
#include <windows.h>
#include <string>
#include <iostream>

#pragma comment(lib, "Ws2_32.lib")
#pragma warning(disable: 4996)
using namespace std;

#define PORT 666    // порт сервера

struct client {
    string fio;
    int money;
    int date;
} A;

int main(int argc, char* argv[]) {
    char buff[1024];

    cout << "server is starting\n";

    string base[3];
    base[0] = "ddd";
    base[1] = "a";
    base[2] = "bb";

    if (WSAStartup(0x202, (WSADATA*)&buff[0]))
    {
        cout << "WSAStartup error: " << WSAGetLastError();
        return -1;
    }
    
    SOCKET Lsock;
    Lsock = socket(AF_INET, SOCK_DGRAM, 0);
    if (Lsock == INVALID_SOCKET) {
        cout << "SOCKET() ERROR: " << WSAGetLastError();
        WSACleanup();
        return -1;
    }
    //связывание сокета с локальным адрес
    sockaddr_in Laddr;
    Laddr.sin_family = AF_INET;
    Laddr.sin_addr.s_addr = INADDR_ANY;   
    Laddr.sin_port = htons(PORT);
    if (bind(Lsock, (sockaddr*)&Laddr, sizeof(Laddr)))
    {
        cout << "BIND ERROR:" << WSAGetLastError();
        closesocket(Lsock);
        WSACleanup();
        return -1;
    }

    while (1) {
        sockaddr_in Caddr;
        int Caddr_size = sizeof(Caddr);
        int bsize = recvfrom(Lsock, (char*)&A, sizeof(buff) - 1, 0,
            (sockaddr*)&Caddr, &Caddr_size);
        if (bsize == SOCKET_ERROR)
            cout << "RECVFROM() ERROR:" << WSAGetLastError();

        cout << A.fio << " " << A.money << " " << A.date << endl;

        // Определяем IP-адрес клиента и прочие атрибуты
        HOSTENT* hst;
        hst = gethostbyaddr((char*)&Caddr.sin_addr, 4, AF_INET);
        cout << "NEW DATAGRAM!\n" <<
            ((hst) ? hst->h_name : "Unknown host") << "/n" <<
            inet_ntoa(Caddr.sin_addr) << "/n" << ntohs(Caddr.sin_port) << '\n';
        buff[bsize] = '\0';              
        

        bool flag = true;
        for (int i = 0; i < 3; i++)
            if (A.fio == base[i])
                flag = false;

        double p = 1;
        int d = 0;
        string answ;
        if (flag)
        {
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
        }
        else
            answ = "You cannot take out a loan";
            
        cout << answ << endl;
        sendto(Lsock, (char*)&answ[0], answ.size(), 0, (sockaddr*)&Caddr, sizeof(Caddr));
    }      return 0;
}