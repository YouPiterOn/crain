#pragma once
#include <string>
#include <vector>
#include "HttpRequest.hpp"

class Route {
public:
    Route(const std::string& path, const std::string& method);
   
    bool isRouteMatch(HttpRequest& request);

    void setMethod(const std::string& method);
    void setPath(const std::string& path);
    std::string getMethod();
    std::string getPath();

private:
    std::string path;
    std::string method;

    std::vector<std::string> splitPath(const std::string& path);
};