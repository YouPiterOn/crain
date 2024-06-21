#pragma once
#include "Router.hpp"
#include "UnitTests.hpp"

void RouterTests() {
    
    UnitTests testSuite;

    testSuite.addTest("Add and execute route", [](){
        Router router;
        int x = 0;
        router.addRoute("GET", "/home", [&x](HttpRequest req, HttpResponse res, NextFunction next){
            x = 1;
        });

        HttpRequest req("GET /home HTTP/1.1");
        router.executeRoute(req);
        ASSERT_EQ(x, 1);
    });

    testSuite.run();
}