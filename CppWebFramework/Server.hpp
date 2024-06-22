#pragma once
#include <thread>
#include <mutex>
#include <queue>
#include <iostream>
#include <winsock2.h>
#include <atomic>
#include <condition_variable>
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
    void addResponse(ServerMessage response);

private:
    SOCKET serverSocket;
    WSADATA serverWsaData;
    std::string serverIpAddress;
    int serverPort;
    sockaddr_in socketAddress;
    int socketAddress_len;

    void clientHandler(SOCKET clientSocket);

    std::thread requestReciever_t;
    std::mutex requestQueue_mtx;
    std::queue<ServerMessage> requestQueue;

    void requestReciever();

    std::thread responseSender_t;
    std::condition_variable responseSender_cv;
    std::mutex responseQueue_mtx;
    std::queue<ServerMessage> responseQueue;

    void responseSender();
    void sendResponse(const ServerMessage&);

    std::atomic<bool> isRunning;

    void logError(const std::string &errorMessage);
    int startServer();
    void closeServer();

};
