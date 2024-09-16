#pragma once
#include <vector>
#include <queue>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <functional>
#include <atomic>
#include <winsock2.h>

namespace crain {
    class WorkersPool {
    public:
        WorkersPool(const size_t& threads);
        ~WorkersPool();

        // Add a new task to the pool
        void enqueue(const SOCKET& clientSocket, const std::string& message);

        void setRequestHandler(std::function<std::string(std::string)> requestHandler);

    private:
        // Worker threads
        std::vector<std::thread> workers;

        std::function<std::string(std::string)> requestHandler;

        // Messages queue
        std::queue<std::pair<SOCKET, std::string>> requests;

        // Synchronization
        std::mutex queueMutex;
        std::condition_variable queueCondition;
        std::atomic<bool> stop;
    };
}