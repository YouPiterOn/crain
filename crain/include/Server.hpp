#pragma once
#include <thread>
#include <mutex>
#include <queue>
#include <winsock2.h>
#include <atomic>
#include <condition_variable>
#include <functional>
#include "WorkersPool.hpp"
#include "ServerConfig.hpp"
#pragma comment(lib, "ws2_32.lib")

namespace crain {
    class Server {
    public:
        Server(std::string ip, int port, ServerConfig config);
        ~Server();

        void run();
        
        void setRequestHandler(std::function<std::string(std::string)> requestHandler);
    private:
        const ServerConfig config;

        SOCKET serverSocket;
        WSADATA serverWsaData;
        std::string serverIpAddress;
        int serverPort;
        sockaddr_in socketAddress;
        int socketAddress_len;

        void clientHandler(SOCKET clientSocket);

        WorkersPool workersPool;

        std::atomic<bool> isRunning;

        void logError(const std::string &errorMessage);
        int startServer();
        void closeServer();
    };
}