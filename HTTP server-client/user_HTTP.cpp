#include <string>
#define _WINSOCK_DEPRECATED_NO_WARNINGS  
#include <winsock2.h>
#include <iostream>
#pragma comment (lib,"Ws2_32.lib")
#pragma warning(disable: 4996)  
using namespace std;
#define request "GET / HTTP/1.0\r\n127.0.0.1\r\n\r\n" 
#define max_packet_size   65535
int main() {
    WSADATA ws; 
    SOCKET s;
    sockaddr_in adr;
    HOSTENT* hostname;
    char buff[max_packet_size];
    if (WSAStartup(MAKEWORD(2, 2), &ws) != 0) {
        cout << "Error with Library Winsock" << endl;
        return -1;
    }

    s = socket(AF_INET, SOCK_STREAM, 0);
    if (s == -1){
        cout << "Error with socket" << endl;
        return -1;
    }
    hostname = gethostbyname("127.0.0.1");
    if (hostname == NULL){
        cout << "Error with HostName" << endl;
        return -1;
    }

    adr.sin_family = AF_INET;
    ((unsigned long*)&adr.sin_addr)[0] = ((unsigned long**)hostname->h_addr_list)[0][0];
    adr.sin_port = htons(8000); //80

    int c = connect(s, (sockaddr*)&adr, sizeof(adr));    
    if (c == -1){
        cerr << "listen failed with error: " << WSAGetLastError() << endl;
        cout << "Error with Connect" << endl;
        return -1;
    }

    int Send = send(s, (char*)&request, sizeof(request), 0);     
    if (Send == -1){
        cout << "Error with Send" << endl;
        return -1;
    }

    int msg = 0;
    do{
        int msg = recv(s, (char*)&buff, max_packet_size, 0);
        if (msg == -1)
        {
            cout << "Error with Recieve" << endl;
            return -1;
        }
        for (int i = 0; i < msg; i++)
            cout << buff[i];
    } while (msg != 0);

    int cs = closesocket(s);
    if (cs == 1) {  
        cout << "Error with CloseSocket" << endl;
        return -1;
    }
    cin.get();
    WSACleanup();
}