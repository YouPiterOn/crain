#pragma once
#include <string>
#include <vector>
#include <functional>
#include "HttpRequest.hpp"
#include "HttpResponse.hpp"

using NextFunction = std::function<void()>;

class Route {
public:
    using RouteHandler = std::function<void(HttpRequest&, HttpResponse&, NextFunction)>;

    Route(const std::string& path, const std::string& method, const RouteHandler& handler);
   
    bool isRouteMatch(HttpRequest& request);

    void setMethod(std::string method);
    void setPath(std::string path);
    void setHandler(const RouteHandler& handler);

    std::string getMethod();
    std::string getPath();
    RouteHandler getHandler();
private:
    std::string path;
    std::string method;
    RouteHandler handler;

    std::vector<std::string> splitPath(const std::string& path);
};