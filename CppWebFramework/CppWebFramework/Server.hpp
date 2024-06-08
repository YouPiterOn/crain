#pragma once
#include "Router.hpp"
#include "HttpHandler.hpp"
#include <thread>
#include <mutex>
#include <queue>
#include <iostream>
#include <winsock2.h>
#pragma comment(lib, "ws2_32.lib")

struct ServerMessage {
    ServerMessage(std::string rawMessage, SOCKET clientSocket) : rawMessage(rawMessage), clientSocket(clientSocket) {}
    std::string rawMessage;
    SOCKET clientSocket;
};

class Server {
public:
    Server(std::string ip, int port);
    ~Server();

    void run();
    ServerMessage getRequest();
    int sendResponse(const ServerMessage&);

private:
    SOCKET serverSocket;
    WSADATA serverWsaData;
    std::string serverIpAddress;
    int serverPort;
    sockaddr_in socketAddress;
    int socketAddress_len;

    std::thread requestReciever_t;
    std::mutex requestQueue_mtx;
    std::queue<ServerMessage> requestQueue;

    void recieveRequests();
    void logError(const std::string &errorMessage);
    int startServer();
    void closeServer();
};
