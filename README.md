# Terminal Chat Server (C++ / Winsock)

A lightweight multi-client terminal chat application built in **C++** using the **Windows Winsock API**.

---

## Features

*  Multiple clients can connect simultaneously
*  Username-based chat system
*  Real-time message broadcasting
*  Join and leave notifications
*  Server-side user kicking
*  Thread-safe client management using mutexes
*  Built with raw Winsock API (no external networking libraries)

---

##  Project Structure

```text
LanChatRoom/
├── server.cpp    # Chat server
└── client.cpp    # Chat client
```

---
##  Requirements

* Windows 7 or newer
* C++17 or newer
* Winsock2
* Any C++ compiler with thread support

### Tested With

* MinGW GCC
* CLion
* Visual Studio

---

## 🔨 Building

### MinGW / g++

```bash
g++ server.cpp -o server -lws2_32
g++ client.cpp -o client -lws2_32
```

### Visual Studio

Create a **C++ Console Application** and build normally.

---

## Running the Application

### 1. Start the Server

```bash
server.exe
```

Expected output:

```text
Listening on port 8080
```

### 2. Start One or More Clients

Open separate terminals and run:

```bash
client.exe
```

---

## Example Session

```text
[Server]
User1 connected
User2 connected

User1: Hello everyone!
User2: Hi User1!

User1 left the chat
```

---

# Creating Your Own LAN Chat Server

By default the project uses:

```text
IP Address: 127.0.0.1
Port: 8080
```

This configuration only works on the same computer.

To allow other devices on your local network to connect:

---

## Step 1 — Find the Server's LAN IP Address

On the machine running the server:

```bash
ipconfig
```

Look for:

```text
IPv4 Address . . . . . . . . . . : 192.168.1.15
```

Your address will be different.

---

## Step 2 — Update the Client IP

In `client.cpp`, locate:

```cpp
inet_pton(AF_INET, "127.0.0.1", &serverAddr.sin_addr);
```

Replace:

```cpp
"127.0.0.1"
```

with your server's LAN IP:

```cpp
inet_pton(AF_INET, "192.168.1.15", &serverAddr.sin_addr);
```

Recompile the client.

---

## Step 3 — Verify the Port

The server listens on:

```cpp
getaddrinfo(nullptr, "8080", &hints, &res);
```

The client connects using:

```cpp
serverAddr.sin_port = htons(8080);
```

Both values must match.

For example, if the server uses:

```cpp
"5000"
```

the client must use:

```cpp
serverAddr.sin_port = htons(5000);
```

---

## Step 4 — Allow Firewall Access

Windows Defender Firewall may block incoming connections.

Allow:

```text
server.exe
```

through the firewall when prompted.

---

## LAN Example

### Server

```text
192.168.1.15
```

### Clients

```text
192.168.1.20
192.168.1.25
192.168.1.40
```

All clients connect to:

```cpp
inet_pton(AF_INET, "192.168.1.15", &serverAddr.sin_addr);
```

and can communicate through the server.

---

## What I Learned

This project helped me understand:

* TCP networking
* Socket programming
* Winsock APIs
* Client-server architecture
* Multithreading
* Thread synchronization with mutexes
* Real-time communication systems

---

## 📄 License

This project is intended for learning and educational purposes.
Feel free to modify, improve, and build upon it.
