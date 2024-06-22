#pragma once

#include "Route.hpp"
#include "UnitTests.hpp"

void RouteTests() {
    UnitTests testSuite;
    
    testSuite.addTest("Match route (success)", [](){
        Route route("GET", "/home", Route::RouteHandler());
        HttpRequest request("GET /home HTTP/1.1");
        
        ASSERT_TRUE(route.isRouteMatch(request));
    });

    testSuite.addTest("Match route (failure)", [](){
        Route route("GET", "/home", Route::RouteHandler());
        HttpRequest request("GET /nothome HTTP/1.1");
        
        ASSERT_FALSE(route.isRouteMatch(request));
    });

    testSuite.run();
}