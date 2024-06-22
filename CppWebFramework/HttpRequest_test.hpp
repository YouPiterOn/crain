#pragma once

#include "HttpRequest.hpp"
#include "UnitTests.hpp"

void HttpRequestTests() {
    UnitTests testSuite;

    testSuite.addTest("Parse raw request (success)", [](){
        std::string rawRequest = "GET /home HTTP/1.1\nHost: cool.host\nUser-Agent: Python-requests/2.25.1\nAccept-Encoding: gzip, deflate\nAccept: */*\nConnection: keep-alive";

        std::string method = "GET";
        std::string uri = "/home";
        std::string version = "HTTP/1.1";
        std::string host = "cool.host";
        std::string connection = "keep-alive";

        HttpRequest request(rawRequest);
        ASSERT_EQ(request.getMethod(), method);
        ASSERT_EQ(request.getURI(), uri);
        ASSERT_EQ(request.getVersion(), version);
        ASSERT_EQ(request.getHeader("Host"), host);
        ASSERT_EQ(request.getHeader("Connection"), connection);
    });

    testSuite.run();
}