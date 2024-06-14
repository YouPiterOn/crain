#pragma once
#include "HttpRequest.hpp"
#include "HttpResponse.hpp"
#include "Route.hpp"
#include <vector>
#include <string>
#include <functional>

class Router {
public:
    void addRoute(std::string method, std::string route, Route::RouteHandler handler);
    HttpResponse executeRoute(HttpRequest& request);
private:
    std::vector<Route> routes;

    std::string pathToStatic = "public";
};

