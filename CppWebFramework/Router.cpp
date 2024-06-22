#include "Router.hpp"

void Router::addRoute(const std::string& method, const std::string& route, const Route::RouteHandler& handler) {
    routes.push_back(Route(method, route, handler));
}

HttpResponse Router::executeRoute(HttpRequest& request) {
    HttpResponse response;

    for(auto route : routes) {
        if(route.isRouteMatch(request)) {
            bool nextCalled = false;

            NextFunction next = [&nextCalled]() {
                nextCalled = true;
            };
            auto handler = route.getHandler();
            handler(request, std::ref(response), next);

            if(nextCalled) {
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