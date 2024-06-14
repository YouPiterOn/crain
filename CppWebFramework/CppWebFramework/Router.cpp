#include "Router.hpp"

void Router::addRoute(std::string method, std::string route, Route::RouteHandler handler) {
    routes.push_back(Route(route, method, handler));
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
            handler(request, response, next);

            if(nextCalled) {
                continue;
            }

            return response;
        }
    }
    response.setBodyFromFile(request.getURI().substr(1));
    if(response.getBody() != "") {
        return response;
    }
    response.setStatus(404, "Route not found");
}
