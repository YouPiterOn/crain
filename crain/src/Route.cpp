#include "../include/Route.hpp"
#include <algorithm>
#include <sstream>

using namespace crain;

Route::Route(const std::string& method, const std::string& path, const RouteHandler& handler) {
    setMethod(method);
    setPath(path);
    setHandler(handler);
}

bool Route::isRouteMatch(HttpRequest& request) {
    if(request.getMethod() != method && method != "ANY") {
        return false;
    }
    auto routeParts = splitPath(path);
    auto requestParts = splitPath(request.getURI());
    if (routeParts.size() != requestParts.size()) {
        return false;
    }

    for (size_t i = 0; i < routeParts.size(); ++i) {
        if (routeParts[i].empty() || requestParts[i].empty()) {
            continue;
        }
        if (routeParts[i] == "*") {
            continue;
        }
        if (routeParts[i][0] == ':') {
            std::string paramName = routeParts[i].substr(1);
            request.addParam(paramName, requestParts[i]);
        } else if (routeParts[i] != requestParts[i]) {
            return false;
        }
    }

    return true;
}

void Route::setMethod(std::string method) {
    auto m(method);
    std::transform(m.begin(), m.end(), m.begin(), [](char c) { return std::toupper(c); });
    this->method = method;
}
void Route::setPath(std::string path) {
    std::transform(path.begin(), path.end(), path.begin(), [](char c) { return std::tolower(c); });
    this->path = path;
}
void Route::setHandler(const RouteHandler& handler) {
    this->handler = handler;
}

std::string Route::getMethod() {
    return method;
}
std::string Route::getPath() {
    return path;
}
RouteHandler Route::getHandler() {
    return handler;
}



std::vector<std::string> Route::splitPath(const std::string& path){
    std::vector<std::string> tokens;
    std::string token;
    std::istringstream tokenStream(path);
    while (std::getline(tokenStream, token, '/')) {
        tokens.push_back(token);
    }
    return tokens;
}