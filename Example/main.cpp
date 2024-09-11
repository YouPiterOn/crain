#include "../crain/app.hpp"
#include <iostream>

int main() {
    auto s = [](handlerArgs){
        response.setBodyFromFile("example/public/index.html");
    };

    crain::App app("0.0.0.0", 4000);

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

    app.setPathToStatic("example/public");

    app.run();

    return 0;
}