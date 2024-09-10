#include "App.hpp"
#include <iostream>

int main() {
    auto s = [](handlerArgs){
        response.setBodyFromFile("Example/public/index.html");
    };

    App app("0.0.0.0", 4000);

    app.use("GET", "/", s);

    app.use("GET", "/home/other",
        [](handlerArgs){
            request.addParam("p", "it works");
            next();
        },
        [](handlerArgs){
            response.setBody(request.getParam("p"));
        }
    );

    app.setPathToStatic("Example/public");

    app.run();

    return 0;
}