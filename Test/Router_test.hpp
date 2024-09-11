#pragma once
#include "Router.hpp"
#include "UnitTests.hpp"

void RouterTests() {
    
    UnitTests testSuite;

    testSuite.addTest("Add and execute route (success)", [](){
        crain::Router router;
        std::string x = "0";
        std::string rawResponse = "HTTP/1.1 200 OK\r\n\r\n";

        router.addRoute("GET", "/home", [&x](handlerArgs){
            x = "1";
        });

        crain::HttpRequest req("GET /home HTTP/1.1");
        crain::HttpResponse response = router.executeRoute(req);
        ASSERT_EQ(x, "1");
        ASSERT_EQ(response.toString(), rawResponse);
    });

    testSuite.addTest("Add and execute route (faliure)", [](){
        crain::Router router;
        std::string x = "0";
        std::string rawResponse = "HTTP/1.1 404 Route not found\r\n\r\n";

        router.addRoute("GET", "/home", [&x](handlerArgs){
            x = "1";
        });

        crain::HttpRequest req("GET /nothome HTTP/1.1");
        crain::HttpResponse response = router.executeRoute(req);
        ASSERT_EQ(x, "0");
        ASSERT_EQ(response.toString(), rawResponse);
    });

    testSuite.addTest("Next function test", [](){
        crain::Router router;
        std::string x = "0";

        router.addRoute("GET", "/home", [](handlerArgs){
            request.addParam("new", "1");
            next();
        });
        router.addRoute("GET", "/home", [&x](handlerArgs){
            x = request.getParam("new");
        });

        crain::HttpRequest req("GET /home HTTP/1.1");
        router.executeRoute(req);
        ASSERT_EQ(x, "1");
    });

    testSuite.run();
}