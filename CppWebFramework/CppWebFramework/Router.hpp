#pragma once
#include "HttpRequest.hpp"
#include "HttpResponse.hpp"
#include "Route.hpp"
#include <map>
#include <string>
#include <functional>

class Router {
public:
    void addRoute(std::string method, std::string route, std::function<void(HttpRequest& req, HttpResponse& res)> handler);
    std::function<void(HttpRequest& req, HttpResponse& res)> findRoute(HttpRequest& request);
private:
    std::map<Route, std::function<void(HttpRequest& req, HttpResponse& res)>> routes;
};

