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

    isRunning = true;

    while(isRunning) {
        SOCKET newSocket = accept(serverSocket, (sockaddr *)&socketAddress, &socketAddress_len);
        if (newSocket < 0) {
            logError("Server failed to accept incoming connection");
            continue;
        }
        std::thread(&Server::clientHandler, this, newSocket).detach();
    }
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
}

void Server::setRequestHandler(RequestHandler handler) {
    requestHandler = handler;
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
                std::thread(&Server::requestProcesser, this, clientSocket, rawRequest).detach();
            }
        } else if (pollResult == 0) {
            return;
        } else {
            logError("WSAPoll failed");
            return;
        }
    }
}


void Server::requestProcesser(SOCKET clientSocket, std::string request) {
    std::string response = requestHandler(request);
    sendResponse(clientSocket, response);
    return;
}

void Server::sendResponse(SOCKET clientSocket, std::string response) {
    int bytesSent;
    long totalBytesSent = 0;
    while (totalBytesSent < response.size()) {
        bytesSent = send(clientSocket, response.c_str(), response.size(), 0);
        if (bytesSent < 0) {
            break;
        }
        totalBytesSent += bytesSent;
    }
    if (totalBytesSent != response.size()) {
        logError("Error sending response to client.");
    }
}

void Server::logError(const std::string &errorMessage) {
    std::println(std::cerr, "WsaError: {}", WSAGetLastError());
    std::println(std::cerr, "{}", errorMessage);
}
