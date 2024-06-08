#pragma once
#include "Router.hpp"
#include "HttpRequest.hpp"
#include "HttpResponse.hpp"

class HttpHandler {
public:
    HttpHandler(const Router&);
    static bool parseRequest(std::string rawRequest);
private:
    const Router& router;
};