#include "../include/Router.hpp"
#include "../include/utils.hpp"

using namespace crain;

void Router::addRoute(const std::string& method, const std::string& route, const RouteHandler& handler) {
    routes.push_back(Route(method, route, handler));
}

HttpResponse Router::executeRoute(HttpRequest& request) {
    HttpResponse response;

    std::vector<std::string> requestParts = splitPath(request.getURI());

    bool nextCalled = false;
    NextFunction next = [&nextCalled]() {
        nextCalled = true;
    };

    for(const auto &route : routes) {
        if(route.isRouteMatch(request, requestParts)) {
            
            auto handler = route.getHandler();
            handler(request, std::ref(response), next);

            if(nextCalled) {
                nextCalled = false;
                continue;
            }

            return response;
        }
    }
    response.setBodyFromFile(pathToStatic + request.getURI());
    if(response.getStatusCode() != 404) {
        return response;
    }
    response.setStatus(404, "Route not found");
    return response;
}

void Router::setPathToStatic(const std::string& path) {
    pathToStatic = path;
}