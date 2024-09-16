#include "../include/WorkersPool.hpp"
#include <print>
#include <iostream>

using namespace crain;

WorkersPool::WorkersPool(const size_t& threads) : stop(false) {
    for (size_t i = 0; i < threads; i++) {
        workers.emplace_back([this] {
            std::unique_lock<std::mutex> lock(this->queueMutex, std::defer_lock);

            std::pair<SOCKET, std::string> requestPair;
            SOCKET clientSocket;
            std::string request;
            std::string response;
        
            int bytesSent;
            long totalBytesSent;

            while(true) {

                { // Accessing messages
                    lock.lock();
                    
                    this->queueCondition.wait(lock, [this] {
                        return this->stop || !this->requests.empty();
                    });
                    if (this->stop && this->requests.empty())
                        return;

                    requestPair = this->requests.front();
                    this->requests.pop();

                    lock.unlock();
                }

                clientSocket = requestPair.first;
                request = requestPair.second;
                totalBytesSent = 0;

                response = requestHandler(request);

                while (totalBytesSent < response.size()) {
                    bytesSent = send(clientSocket, response.c_str(), response.size(), 0);
                    if (bytesSent < 0) {
                        break;
                    }
                    totalBytesSent += bytesSent;
                }
                if (totalBytesSent != response.size()) {
                    std::println(std::cerr, "WsaError: {}", WSAGetLastError());
                    std::println(std::cerr, "Error sending response to client.");
                }
            }
        });
    }
}

void WorkersPool::enqueue(const SOCKET& clientSocket, const std::string& message) {
    {
        std::unique_lock<std::mutex> lock(queueMutex);

        if (stop)
            throw std::runtime_error("enqueue on stopped ThreadPool");

        requests.push(std::pair<SOCKET, std::string>(clientSocket, message));
    }
    queueCondition.notify_one();
}

void WorkersPool::setRequestHandler(std::function<std::string(std::string)> requestHandler) {
    this->requestHandler = requestHandler;
}

WorkersPool::~WorkersPool() {
    {
        std::unique_lock<std::mutex> lock(queueMutex);
        stop = true;
    }
    queueCondition.notify_all();
    for (std::thread& worker : workers)
        worker.join();
}
