#pragma once

#include "HttpResponse.hpp"
#include "UnitTests.hpp"

void HttpResponseTests() {
    UnitTests testSuite;

    testSuite.addTest("Response to string (success)", [](){
        std::string rawResponse = "HTTP/1.1 200 OK\r\nContent-Length: 4\r\n\r\ngood";
        crain::HttpResponse response;
        response.setBody("good");
        ASSERT_EQ(response.toString(), rawResponse);
    });

    testSuite.addTest("Set response body from file (failure)", [](){
        std::string rawResponse = "HTTP/1.1 404 File not found\r\n\r\n";
        crain::HttpResponse response;
        response.setBodyFromFile("NotExisting.file");
        ASSERT_EQ(response.toString(), rawResponse);
    });

    testSuite.run();
}