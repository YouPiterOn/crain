#include "Server.hpp"
#include "HttpRequest.hpp"
#include "HttpResponse.hpp"
#include <WS2tcpip.h>
#include <print>

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


    requestReciever_t = std::thread(&Server::recieveRequests, this);

    /*HttpRequest request(rawRequest);
    HttpResponse response;

    response.setBodyFromFile(request.getURI().substr(1));

    std::string rawResponse = response.toString();*/
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
}

ServerMessage Server::getRequest() {
    std::lock_guard<std::mutex> lock(requestQueue_mtx);
    ServerMessage m = requestQueue.front();
    requestQueue.pop();
    return m;
}

int Server::sendResponse(const ServerMessage& message) {
    int bytesSent;
    long totalBytesSent = 0;
    while (totalBytesSent < message.rawMessage.size()) {
        bytesSent = send(message.clientSocket, message.rawMessage.c_str(), message.rawMessage.size(), 0);
        if (bytesSent < 0) {
            break;
        }
        totalBytesSent += bytesSent;
    }
    if (totalBytesSent == message.rawMessage.size()) {
        std::println("------ Server Response sent to client ------\n\n");
    }
    else {
        logError("Error sending response to client.");
    }
}


void Server::recieveRequests() {
    while(true) {
        SOCKET newSocket = accept(serverSocket, (sockaddr *)&socketAddress, &socketAddress_len);
        if (newSocket < 0) {
            logError("Server failed to accept incoming connection");
            continue;
        }

        char rawRequest[10240] = {0};
        int bytesReceived = recv(newSocket, rawRequest, 10240, 0);
        if (bytesReceived < 0) {
            logError("Failed to receive bytes from client socket connection");
            continue;
        }
        std::println("\n------ Received Request from client ------\n");

        std::lock_guard<std::mutex> lock(requestQueue_mtx);
        requestQueue.push(ServerMessage(rawRequest, newSocket));
    }
}

void Server::logError(const std::string &errorMessage) {
    std::println(std::cerr, "WsaError: {}", WSAGetLastError());
    std::println(std::cerr, "{}", errorMessage);
}
