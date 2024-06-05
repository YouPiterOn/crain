#include "Server.hpp"
#include "HttpRequest.hpp"
#include <WS2tcpip.h>
#include <print>

Server::Server(std::string ip, int port) : serverIpAddress(ip), serverPort(port), socketAddress_len(sizeof(socketAddress)) {
    startServer();
}

Server::~Server() {
    closeServer();
}

void Server::Run() {
    if (listen(serverSocket, 20) < 0) {
        exitWithError("Socket listen failed");
    }
    std::println("#-----------------------------#");
    std::println("Server is running on:");
    std::println("Address: {}", inet_ntoa(socketAddress.sin_addr));
    std::println("Port: {}", ntohs(socketAddress.sin_port));
    std::println("#-----------------------------#");


    while(true) {
        SOCKET new_socket = accept(serverSocket, (sockaddr *)&socketAddress, &socketAddress_len);
        if (new_socket < 0) {
            exitWithError("Server failed to accept incoming connection");
        }
        char rawRequest[10240] = {0};
        int bytesReceived = recv(new_socket, rawRequest, 10240, 0);
        if (bytesReceived < 0) {
            exitWithError("Failed to receive bytes from client socket connection");
        }

        std::println("\n------ Received Request from client ------\n");
        HttpRequest request(rawRequest);

        closesocket(new_socket);
    };
}

int Server::startServer() {
    if (WSAStartup(MAKEWORD(2, 0), &serverWsaData) != 0) {
        exitWithError("WSAStartup failed");
        return 1;
    }
    serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket < 0) {
        exitWithError("Cannot create socket");
        return 1;
    }
    socketAddress.sin_family = AF_INET;
    socketAddress.sin_port = htons(serverPort);
    inet_pton(AF_INET, serverIpAddress.c_str(), &socketAddress.sin_addr.s_addr);
    if (bind(serverSocket, (struct sockaddr *)&socketAddress, socketAddress_len) < 0) {
        exitWithError("Cannot connect socket to address");
        return 1;
    }
    return 0;
}

void Server::closeServer() {
    closesocket(serverSocket);
    WSACleanup();
}

void Server::exitWithError(const std::string &errorMessage) {
    std::println(std::cerr, "{}", WSAGetLastError());
    std::println(std::cerr, "Error: {}", errorMessage);
    exit(1);
}
