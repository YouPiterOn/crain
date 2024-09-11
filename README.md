# Crain web framework

Student project created first of all to learn but feel free to use and commit <3

## User guide

### Quick start

 1. To start include `app.hpp` header and create object `App`
    ```
    crain::App app("0.0.0.0", 4000);
    ```
    Arguments are host and port where to run the server

 2. Add routes by calling `app.use`
    ```
    app.use(*method*, *route*, *handler1*, *handler2*, ...);
    ```
    or
    ```
    app.use(*handler1*, *handler2*, ...);
    ```

    Handler is a function with three arguments `HttpRequest request, HttpResponse respone, NextFunction next`

    You can utilize the defined `handlerArgs` keyword to place them automaticaly

    Example:
    ```
    app.use("GET", "/", [](handlerArgs){});
    ```

 3. Inside the handler function perform actions with `request` and `response` or call `next()` to move to the next suitable handler
    
    Example:
    ```
    app.use("GET", "/home/other",
        [](handlerArgs){
            request.addParam("p", "it works");
            next();
        },
        [](handlerArgs){
            response.setBody(request.getParam("p"));
        }
    );
    ```
 4. When you finish routing add path to the folder with static files to serve  by `app.setPathToStatic("example/public");` and use `app.run()` to start the server and working loop

    Full example:
    ```
    #include "crain/app.hpp"

    int main() {
        crain::App app("0.0.0.0", 4000);

        app.use("GET", "/home", [](handlerArgs){
            response.setBodyFromFile("public/index.html");
        });

        app.setPathToStatic("example/public");

        app.run();

        return 0;
    }
    ```
5. Originally library is compiled with clang++ and c++23

    To compile example use:
    ```
    clang++ -std=c++23 ./example/main.cpp ./crain/src/*.cpp -o main.exe
    ```
