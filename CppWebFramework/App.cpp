#include "App.hpp"
#include <print>

App::App(std::string ip, int port) : server(ip, port) {}

void App::run() {
    server.run();
    appLoop();
}

void App::stop() {
    server.~Server();
}

void App::appLoop() {
    while(true) {
        auto serverReq = server.getRequest();
        if(serverReq.clientSocket == 0) {
            continue;
        }
        HttpRequest req(serverReq.rawMessage);
        HttpResponse res = router.executeRoute(req);
        ServerMessage serverRes(res.toString(), serverReq.clientSocket);
        server.addResponse(serverRes);
    }
}