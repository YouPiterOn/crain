#pragma once
#include "HttpRequest.hpp"
#include <string>
#include <map>

class HttpResponse {
public:
    HttpResponse();

    void setStatus(int statusCode, const std::string& reasonPhrase);
    void setVersion(const std::string& version);
    void setHeader(const std::string& field, const std::string& value);
    void setBody(const std::string& body);
    void setBodyFromFile(const std::string& filePath);
    std::string toString() const;

private:
    std::string version;
    int statusCode;
    std::string reasonPhrase;
    std::map<std::string, std::string> headers;
    std::string body;
};