#define _WIN32_WINNT 0x0601
#include <iostream>
#include <vector>
#include <unordered_map>
#include <thread>
#include <mutex>
#include <string>
#include <winsock2.h>
#include <ws2tcpip.h>
#pragma comment(lib, "ws2_32.lib")
#include<cstring>
#include <algorithm>
#include <thread>
using namespace std;
vector<SOCKET> clients;
unordered_map<SOCKET, string> usernames;
mutex clientsMutex;
void receiving(SOCKET client);
void broadcasting(SOCKET client, const char* full);
void kick(const string &user) {
    SOCKET target;
    {
    lock_guard<mutex> lock(clientsMutex);
    for (auto i:usernames) {
        if (user==i.second) {
            target =i.first;
            break;
        }
    }
}
    if (target!=INVALID_SOCKET) {
        string last= "you have been kicked from the server";
        send(target,last.c_str(),last.length(),0);
        shutdown(target,SD_BOTH);
        closesocket(target);
        last = user  +" has been kicked from the server";
        broadcasting(target, last.c_str());
    }
}
void broadcasting(SOCKET client, const char* full) {
    lock_guard<mutex> lock(clientsMutex);
    for (auto i: clients) {
        if (i!=client) {
            send(i,full, strlen(full), 0);
        }
    }
}
void joining(SOCKET serversocket) {
    while (1) {
        SOCKET client = accept(serversocket,nullptr, nullptr);
        { //lockguard because clients is shared between multiple threads.
            lock_guard<mutex> lock(clientsMutex);
            clients.push_back(client);
        }
        thread(receiving,client).detach();
    }
}
void remove(SOCKET client) {
    lock_guard<mutex> lock(clientsMutex);
    for (auto i= clients.begin(); i!=clients.end();i++){
        if (*i==client){
            clients.erase(i);
            break;
        }
    }
    usernames.erase(client);
}
void receiving(SOCKET client) {
    char buffer[4096];
    int firstname = recv(client,buffer ,sizeof(buffer),0);
    buffer[firstname]= '\0';
    string username = buffer;
    {
        lock_guard<mutex> lock(clientsMutex);
        usernames[client]= username;
    }
    cout<<buffer<<" has joined"<<endl;
    while (1) {
        int received = recv(client,buffer,sizeof(buffer),0);
        if (received <= 0) {
            closesocket(client);
            break;
        }
        buffer[received]= '\0';
        cout<<username<<":"<<buffer<<endl;
        string full = username+ ": " +buffer ;
        broadcasting(client,full.c_str());
    }
    cout<<username<<" has left the chat"<<endl;
    string full = username+" has left the chat";
    broadcasting(client, full.c_str());
    remove(client);
    closesocket(client);
}
void adminfeatures() {
    string cmd;
    while (getline(cin, cmd)) {
        if (cmd.rfind("/kick ",0)==0) {
            string username = cmd.substr(6);
            kick(username);
        }
    }
}

int main() {
    WSADATA wsa;
    if (WSAStartup(MAKEWORD(2,2), &wsa)!=0) {
        cout<<"wsa startup failed"<<endl;
        return 1;
    }
    addrinfo hints{}, *res;
    hints.ai_family= AF_INET;
    hints.ai_socktype= SOCK_STREAM;
    hints.ai_flags= AI_PASSIVE;
    if (getaddrinfo(nullptr, "8080", &hints, &res)!=0) {
        cout<<"getaddrinfo failed"<<endl;
        return 1;
    }
    SOCKET serversocket = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
    if (serversocket == INVALID_SOCKET) {
        cout<<"socket failed "<<endl;
        freeaddrinfo(res);
        WSACleanup();
        return 1;
    }
    if (bind(serversocket, res->ai_addr, (int)res->ai_addrlen)==SOCKET_ERROR) {
        cout<<"bind failed "<<endl;
        freeaddrinfo(res);
        WSACleanup();
        return 1;
    }
    if (listen(serversocket, SOMAXCONN)==SOCKET_ERROR) {
        cout<<"listen failed"<<endl;
        freeaddrinfo(res);
        WSACleanup();
        return 1;
    }
    cout<<"listening on port 8080: "<<endl;
    thread add(joining,serversocket);
    thread admin(adminfeatures);
    add.join();
    admin.join();
}