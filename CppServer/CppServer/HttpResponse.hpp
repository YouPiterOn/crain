#pragma once
#include "HttpRequest.hpp"
#include <string>
#include <map>

class HttpResponse {
public:
    HttpResponse();

    void setStatus(const std::string& version, int statusCode, const std::string& reasonPhrase);
    void setHeader(const std::string& field, const std::string& value);
    void setBody(const std::string& body);
    std::string toString() const;

private:
    std::string version;
    int statusCode;
    std::string reasonPhrase;
    std::map<std::string, std::string> headers;
    std::string body;
};