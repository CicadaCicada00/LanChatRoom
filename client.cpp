#define _WIN32_WINNT 0x0601
#include <iostream>
#include <string>
#include <thread>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <fstream>
#include <vector>
#include <algorithm>
#pragma comment(lib, "ws2_32.lib")
using namespace std;
void sending(SOCKET s) {
    while (1) {
        string msg;
        getline(cin ,msg);
        if (msg=="/exit") {
            break;
        }
        if (msg.empty()) continue;

        send(s, msg.c_str(), (int)msg.size(), 0 );
    }
    closesocket(s);
    WSACleanup();
}
void receiving(SOCKET s) {
    char buffer[1024];
    while (1) {
        int received = recv(s, buffer, sizeof(buffer)-1, 0);
        if (received<=0) {
            cout<<"disconnected "<<endl;
            break;
        }
        buffer[received]= '\0';
        cout<<buffer<<endl;
        cout.flush();
    }
}
int main() {
    WSADATA wsa;
    if (WSAStartup(MAKEWORD(2,2), &wsa)!=0) {
        cout<<"Startup failed"<<endl;
        return 1;
    }
    SOCKET s  = socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
    sockaddr_in serverAddr{};
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(8080);
    inet_pton(AF_INET, "127.0.0.1", &serverAddr.sin_addr);
    if (connect(s, (sockaddr*)&serverAddr, sizeof(serverAddr))==SOCKET_ERROR){
        cout<<"Connect failed"<<endl;
        closesocket(s);
        WSACleanup();
        return 1;
    }
    cout<<"connected to server"<<endl;
    string username;
    cout<<"enter username: "<<endl;
    getline( cin, username);
    cout<<"welcome to the server "<<username<<" you can start chatting"<<endl;
    send(s,username.c_str(),(int)username.size(),0 );
    thread share(sending,s);
    thread receive(receiving,s);
    share.join();
    receive.join();
    closesocket(s);
    WSACleanup();
}