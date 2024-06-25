#include "Server.hpp"
#include <WS2tcpip.h>
#include <print>
#include <future>
#include <chrono>

Server::Server(std::string ip, int port) : serverIpAddress(ip), serverPort(port), socketAddress_len(sizeof(socketAddress)) {
    if(startServer() != 0) {
        throw std::exception("Error starting server");
    }
}

Server::~Server() {
    closeServer();
}

void Server::run() {
    if (listen(serverSocket, 20) < 0) {
        logError("Socket listen failed");
        return;
    }
    std::println("#-----------------------------#");
    std::println("Server is running on:");
    std::println("Address: {}", inet_ntoa(socketAddress.sin_addr));
    std::println("Port: {}", ntohs(socketAddress.sin_port));
    std::println("#-----------------------------#");


    requestReciever_t = std::thread(&Server::requestReciever, this);
    responseSender_t = std::thread(&Server::responseSender, this);

    isRunning = true;
}


int Server::startServer() {
    if (WSAStartup(MAKEWORD(2, 0), &serverWsaData) != 0) {
        logError("WSAStartup failed");
        return 1;
    }
    serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket < 0) {
        logError("Cannot create socket");
        return 1;
    }
    socketAddress.sin_family = AF_INET;
    socketAddress.sin_port = htons(serverPort);
    inet_pton(AF_INET, serverIpAddress.c_str(), &socketAddress.sin_addr.s_addr);
    if (bind(serverSocket, (struct sockaddr *)&socketAddress, socketAddress_len) < 0) {
        logError("Cannot connect socket to address");
        return 1;
    }
    return 0;
}


void Server::closeServer() {
    closesocket(serverSocket);
    WSACleanup();
    isRunning = false;
    requestReciever_t.join();
}

ServerMessage Server::getRequest() {
    std::lock_guard<std::mutex> lock(requestQueue_mtx);
    while(requestQueue.empty()) { return ServerMessage("", 0); }
    ServerMessage m = requestQueue.front();
    requestQueue.pop();
    return m;
}

bool Server::isRequestQueueEmpty() {
    return requestQueue.empty();
}

void Server::addResponse(ServerMessage response) {
    std::lock_guard<std::mutex> lock(responseQueue_mtx);
    responseQueue.push(response);
    responseSender_cv.notify_all();
}

void Server::clientHandler(SOCKET clientSocket) {
    WSAPOLLFD fdArray[1];
    fdArray[0].fd = clientSocket;
    fdArray[0].events = POLLIN;

    while (true) {
        int pollResult = WSAPoll(fdArray, 1, 5000);

        if (pollResult > 0) {
            if (fdArray[0].revents & POLLERR) {
                logError("Socket error");
                return;
            }
            if (fdArray[0].revents & POLLHUP) {
                logError("Socket hang-up");
                return;
            }
            if (fdArray[0].revents & POLLNVAL) {
                logError("Invalid socket");
                return;
            }
            if (fdArray[0].revents & POLLIN) {
                char rawRequest[10240] = {0};
                int bytesReceived = recv(clientSocket, rawRequest, 10240, 0);
                if (bytesReceived < 0) {
                    logError("Failed to receive bytes from client socket connection");
                    return;
                }
                std::lock_guard<std::mutex> lock(requestQueue_mtx);
                requestQueue.push(ServerMessage(rawRequest, clientSocket));
            }
        } else if (pollResult == 0) {
            return;
        } else {
            logError("WSAPoll failed");
            return;
        }
    }
}


void Server::requestReciever() {
    while(isRunning) {
        SOCKET newSocket = accept(serverSocket, (sockaddr *)&socketAddress, &socketAddress_len);
        if (newSocket < 0) {
            logError("Server failed to accept incoming connection");
            continue;
        }
        std::thread(&Server::clientHandler, this, newSocket).detach();
    }
}

void Server::responseSender() {
    while (isRunning) {
        std::unique_lock<std::mutex> lock(responseQueue_mtx);
        
        responseSender_cv.wait(lock, [this] { return !responseQueue.empty() || !isRunning; });
        
        while (!responseQueue.empty()) {
            auto response = responseQueue.front();
            responseQueue.pop();
            lock.unlock();
            sendResponse(response);
            lock.lock();
        }
    }
}

void Server::sendResponse(const ServerMessage& message) {
    int bytesSent;
    long totalBytesSent = 0;
    while (totalBytesSent < message.rawMessage.size()) {
        bytesSent = send(message.clientSocket, message.rawMessage.c_str(), message.rawMessage.size(), 0);
        if (bytesSent < 0) {
            break;
        }
        totalBytesSent += bytesSent;
    }
    if (totalBytesSent != message.rawMessage.size()) {
        logError("Error sending response to client.");
    }
}

void Server::logError(const std::string &errorMessage) {
    std::println(std::cerr, "WsaError: {}", WSAGetLastError());
    std::println(std::cerr, "{}", errorMessage);
}
