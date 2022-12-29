#include <iostream>
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <winsock2.h>
#include <string>
#include <windows.h>
#pragma comment (lib, "Ws2_32.lib")
#pragma warning(disable: 4996)

using namespace std;

#define SRV_HOST ""
#define SRV_PORT 1234
#define CLNT_PORT 1235
#define BUF_SIZE 128

struct client {
    string fio;
    int hours;
    int count;
} B;

int main() {
    char buff[1024];
    if (WSAStartup(0x0202, (WSADATA*)&buff[0])) {
        cout << "Error WSAStartup \n" << WSAGetLastError();
        return -1;
    }
  
    SOCKET s;
    int from_len;
    char buf[BUF_SIZE] = { 0 };
    char a[BUF_SIZE] = { 0 };

    hostent* hp;
    sockaddr_in clnt_sin, srv_sin;
    s = socket(AF_INET, SOCK_STREAM, 0);
    clnt_sin.sin_family = AF_INET;
    clnt_sin.sin_addr.s_addr = 0;
    clnt_sin.sin_port = htons(CLNT_PORT);
    bind(s, (sockaddr*)&clnt_sin, sizeof(clnt_sin));

    hp = gethostbyname(SRV_HOST);
    srv_sin.sin_port = htons(SRV_PORT);
    srv_sin.sin_family = AF_INET;
    ((unsigned long*)&srv_sin.sin_addr)[0] = ((unsigned long**)hp->h_addr_list)[0][0];
    connect(s, (sockaddr*)&srv_sin, sizeof(srv_sin));
    string mst;

    do {
        from_len = recv(s, (char*)&buf, BUF_SIZE, 0);
        buf[from_len] = 0;
        cout << buf << endl;

        cin >> B.fio;
        cin >> B.hours;
        cin >> B.count;

        send(s, (char*)&B, sizeof(B), 0);

        from_len = recv(s, (char*)&a, BUF_SIZE, 0);
        a[from_len] = 0;
        cout << a << endl;
        cout << "Continue? (1 - yes/2 - no) " << endl;
        cin >> mst;
        send(s, (char*)&mst[0], mst.size(), 0);
        cout << " " << endl;
    } while (mst != "2");
    cout << "exit" << endl;
    cin.get();
    closesocket(s);
    return 0;
}
