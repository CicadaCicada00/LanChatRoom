Terminal Chat Server (C++ Winsock)
A simple multi-client terminal chat application built in C++ using the Windows Winsock API.

Features
Multiple clients can connect simultaneously
Username-based chat
Real-time message broadcasting
Join and leave notifications
Server-side user kicking
Thread-safe client management using mutexes
Built using raw Winsock API (no external networking libraries)
Project Structure

.
├── server.cpp
└── client.cpp
Server
Responsible for:

Accepting incoming connections
Managing connected clients
Broadcasting messages
Tracking usernames
Handling disconnects
Admin commands
Client
Responsible for:

Connecting to the server
Sending messages
Receiving broadcasts
Displaying chat messages
Requirements
Windows 7 or newer
C++17 or newer
Winsock2
Any C++ compiler supporting threads
Tested with:

MinGW
CLion
Visual Studio
g++ server.cpp -o server -lws2_32
g++ client.cpp -o client -lws2_32
Visual Studio
Create a C++ Console Project and build normally.

Running
Start the server
server.exe
Expected output:

listening on port 8080
Start clients
Open multiple terminals and run:

client.exe
Example Session
Creating Your Own LAN Chat Server
By default the project uses:

IP Address: 127.0.0.1
Port: 8080
This only works on the same computer. To allow other devices on your local network to connect:

Step 1: Find the server's LAN IP
On the computer running the server:

ipconfig
Look for:

IPv4 Address . . . . . : 192.168.1.15
Your address will be different.

Step 2: Change the client IP
In client.cpp locate:

inet_pton(AF_INET, "127.0.0.1", &serverAddr.sin_addr);
Replace:

"127.0.0.1"
with the server machine's LAN IP:

inet_pton(AF_INET, "192.168.1.15", &serverAddr.sin_addr);
Recompile the client.

Step 3: Verify the port
The server listens on:

getaddrinfo(nullptr, "8080", &hints, &res);
The client connects to:

serverAddr.sin_port = htons(8080);
Both values must match.

For example, if you change the server port to:

"5000"
then the client must also use:

serverAddr.sin_port = htons(5000);
Step 4: Allow the server through the firewall
Windows Firewall may block incoming connections.

Allow:

server.exe
through Windows Defender Firewall when prompted.

LAN Example
Server PC:

192.168.1.15
Client PCs:

192.168.1.20
192.168.1.25
192.168.1.40
All clients connect using:

inet_pton(AF_INET, "192.168.1.15", &serverAddr.sin_addr);
and can chat with each other through the server.

What I Learned
This project helped me understand:

TCP networking
Socket APIs
Concurrent programming
Thread synchronization
Network protocols
Client-server architecture
and serves as a foundation for more advanced networking projects.
