#include "Router.hpp"

void Router::addRoute(std::string method, std::string route, std::function<void(HttpRequest& req, HttpResponse& res)> handler) {
    routes[Route(route, method)] = handler;
}

std::function<void(HttpRequest& req, HttpResponse& res)> Router::findRoute(HttpRequest& request) {
    for(auto pair : routes) {
        auto route = pair.first;
        if(route.isRouteMatch(request)) {
            return pair.second;
        }
    }
}