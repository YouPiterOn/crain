#pragma once
#include <thread>
#include <mutex>
#include <queue>
#include <iostream>
#include <winsock2.h>
#include <atomic>
#include <condition_variable>
#include <functional>
#pragma comment(lib, "ws2_32.lib")

namespace crain {
    using RequestHandler = std::function<std::string(std::string)>;

    class Server {
    public:
        Server(std::string ip, int port);
        ~Server();

        void run();
        
        void setRequestHandler(RequestHandler);

    private:
        SOCKET serverSocket;
        WSADATA serverWsaData;
        std::string serverIpAddress;
        int serverPort;
        sockaddr_in socketAddress;
        int socketAddress_len;

        RequestHandler requestHandler;

        void clientHandler(SOCKET clientSocket);

        void requestProcesser(SOCKET clientSocket, std::string request);

        void sendResponse(SOCKET clientSocket, std::string response);

        std::atomic<bool> isRunning;

        void logError(const std::string &errorMessage);
        int startServer();
        void closeServer();
    };
}