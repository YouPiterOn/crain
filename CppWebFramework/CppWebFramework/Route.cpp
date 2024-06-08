#include "Route.hpp"
#include <algorithm>
#include <sstream>

Route::Route(const std::string& path, const std::string& method) {
    setPath(path);
    setMethod(method);
}

bool Route::isRouteMatch(HttpRequest& request) {
    if(request.getMethod() != method) {
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
        if (routeParts[i][0] == ':') {
            std::string paramName = routeParts[i].substr(1);
            request.addParam(paramName, requestParts[i]);
        } else if (routeParts[i] != requestParts[i]) {
            return false;
        }
    }

    return true;
}

void Route::setMethod(const std::string& method) {
    std::transform(method.begin(), method.end(), method.begin(), ::toupper);
    this->method = method;
}
void Route::setPath(const std::string& path) {
    std::transform(path.begin(), path.end(), path.begin(), ::tolower);
    this->path = path;
}

std::string Route::getMethod() {
    return method;
}
std::string Route::getPath() {
    return path;
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