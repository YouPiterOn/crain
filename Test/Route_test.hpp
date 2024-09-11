#pragma once

#include "Route.hpp"
#include "UnitTests.hpp"

void RouteTests() {
    UnitTests testSuite;
    
    testSuite.addTest("Match route (success)", [](){
        crain::Route route("GET", "/home", crain::RouteHandler());
        crain::HttpRequest request("GET /home HTTP/1.1");
        
        ASSERT_TRUE(route.isRouteMatch(request));
    });

    testSuite.addTest("Match route (failure)", [](){
        crain::Route route("GET", "/home", crain::RouteHandler());
        crain::HttpRequest request("GET /nothome HTTP/1.1");
        
        ASSERT_FALSE(route.isRouteMatch(request));
    });

    testSuite.run();
}