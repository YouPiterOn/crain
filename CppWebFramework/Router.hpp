#pragma once
#include "HttpRequest.hpp"
#include "HttpResponse.hpp"
#include "Route.hpp"
#include <vector>
#include <string>
#include <functional>

#define handlerArgs HttpRequest& request, HttpResponse& response, NextFunction next

class Router {
public:
    void addRoute(const std::string& method, const std::string& route, const RouteHandler& handler);
    HttpResponse executeRoute(HttpRequest& request);
    void setPathToStatic(const std::string& path);
private:
    std::vector<Route> routes;

    std::string pathToStatic = "public";
};

