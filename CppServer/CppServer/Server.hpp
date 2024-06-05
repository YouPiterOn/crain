#pragma once
#include <iostream>
#include <winsock2.h>
#pragma comment(lib, "ws2_32.lib")

class Server {
public:
    Server(std::string ip, int port);
    ~Server();
    void Run();
    
private:
    SOCKET serverSocket;
    WSADATA serverWsaData;
    std::string serverIpAddress;
    int serverPort;
    sockaddr_in socketAddress;
    int socketAddress_len;

    void exitWithError(const std::string &errorMessage);
    int startServer();
    void closeServer();
};