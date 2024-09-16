#include "../include/App.hpp"
#include <print>

using namespace crain;

App::App(std::string ip, int port, ServerConfig serverConfig) : server(ip, port, serverConfig) {}

void App::run() {
    server.setRequestHandler(std::bind(&App::requestHandler, this, std::placeholders::_1));
    server.run();
}

void App::setPathToStatic(const std::string& path) {
    router.setPathToStatic(path);
}

std::string App::requestHandler(std::string request) {
    HttpRequest req(request);

    router_m.lock();
    HttpResponse res = router.executeRoute(req);
    router_m.unlock();

    return res.toString();
}