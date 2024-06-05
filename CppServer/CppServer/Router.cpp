#pragma once
#include <map>
#include <string>
#include <functional>

class Router {
public:
    void addRoute(std::string, std::function<void>);
private:
    std::map<std::string, std::function<void()>> routes;
};