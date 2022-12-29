#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <winsock2.h>
#include <string>
#include <windows.h>
#include <iostream>
#pragma comment(lib, "ws2_32.lib")
#pragma warning(disable: 4996)
using namespace std;

#define PORT 666
#define SERVERADDR "127.0.0.1" 

struct client {
    string fio;
    int money;
    int date;
} B;

int main(int argc, char* argv[])
{
    char buff[10 * 1014];
    cout << "Demo Client\n";
    
    if (WSAStartup(0x202, (WSADATA*)&buff))
    {
        cout << "WSASTARTUP ERROR: " << WSAGetLastError() << "\n";
        return -1;
    }
    
    SOCKET my_sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (my_sock == INVALID_SOCKET) {
        cout << "SOCKET() ERROR: " << WSAGetLastError() << "\n";
        WSACleanup();
        return -1;
    }
    
    HOSTENT* hst;
    sockaddr_in Daddr;
    Daddr.sin_family = AF_INET;
    Daddr.sin_port = htons(PORT);

    if (inet_addr(SERVERADDR)) // определение IP-адреса узла
        Daddr.sin_addr.s_addr = inet_addr(SERVERADDR);
    else
        if (hst = gethostbyname(SERVERADDR))
            Daddr.sin_addr.s_addr = ((unsigned long**)
                hst->h_addr_list)[0][0];
        else {
            cout << "Unknown Host: " << WSAGetLastError() << "\n";
            closesocket(my_sock);
            WSACleanup();
            return -1;
        }

    while (1) {
        cout << "Do you want to calculate the loan ? (1 - yes / 2 - no)  ";
        string SS;
        getline(cin, SS);
        if (SS == "2") break;

        cout << "Enter your fio,amount and term of loan" << endl;
        cin >> B.fio;
        cin >> B.money;
        cin >> B.date;
        getchar();

        sendto(my_sock, (char*)&B, sizeof(B), 0,
            (sockaddr*)&Daddr, sizeof(Daddr));
        
        sockaddr_in SRaddr;
        int SRaddr_size = sizeof(SRaddr);
        int n = recvfrom(my_sock, buff, sizeof(buff), 0,
            (sockaddr*)&SRaddr, &SRaddr_size);
        if (n == SOCKET_ERROR) {
            cout << "RECVFROM() ERROR:" << WSAGetLastError() << "\n";
            closesocket(my_sock);
            WSACleanup();  return -1;
        }
        buff[n] = '\0';
        cout  << buff << "\n";
    }
    closesocket(my_sock);
    WSACleanup();     return 0;
}