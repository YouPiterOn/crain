#include "App.hpp"

int main() {
    App app("0.0.0.0", 4000);

    app.router.addRoute("GET", "/home", [](handlerArgs){
        response.setBodyFromFile("Example/public/index.html");
    });

    app.router.addRoute("GET", "/home/other", [](handlerArgs){
        request.addParam("p", "it works");
        next();
    });

    app.router.addRoute("GET", "/home/other", [](handlerArgs){
        response.setBody(request.getParam("p"));
    });

    app.router.setPathToStatic("Example/public");

    app.run();

    return 0;
}