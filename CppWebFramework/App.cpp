#include "App.hpp"
#include <print>

App::App(std::string ip, int port) : server(ip, port) {}

void App::run() {
    server.run();
    appLoop();
}

void App::appLoop() {
    while (true) {
            if(server.isRequestQueueEmpty()) {
                continue;
            }
            ServerMessage serverReq = server.getRequest();
            HttpRequest req(serverReq.rawMessage);
            HttpResponse res = router.executeRoute(req);
            ServerMessage serverRes(res.toString(), serverReq.clientSocket);
            server.addResponse(serverRes);
    }
}