# CppWebFramework

Web framework with custom server

Student project created just to learn, nothing specific

## User guide

### Quick start

 1. To start include `App.hpp` header and create object `App`
    ```
    App app("0.0.0.0", 4000);
    ```
    Arguments are host and port where to run the server

 2. Add routes using `router` component and its method `addRoute`
    ```
    app.router.addRoute("GET", "/home", [](handlerArgs){ 

    });
    ```
    Arguments for `addRoute` are method, route and handler

    Handler is a function with three arguments `HttpRequest request, HttpResponse respone, NextFunction next`

    You can utilize the defined `handlerArgs` keyword to place them automaticaly

 3. In lambda function perform actions with `request` using its functionality
    ```
    app.router.addRoute("GET", "/home", [](handlerArgs){ 
        response.setBodyFromFile("Example/public/index.html");
    });
    ```
 4. When you finish routing use `app.run()` to start the server and working loop
    ```
    #include "App.hpp"

    int main() {
        App app("0.0.0.0", 4000);

        app.router.addRoute("GET", "/home", [](handlerArgs){
            response.setBodyFromFile("Example/public/index.html");
        });

        app.run();

        return 0;
    }
    ```
### Basic features

 - In handler you can call `next()` function which will make router search for the next siutable route

 - By using `router.setPathToStatic()` you can set your own path to folder with static files
