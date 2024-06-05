#pragma once
#include <string>
#include <map>


class HttpRequest {
public:
    HttpRequest(const char* request);
    
    std::string getMethod() const;
    std::string getURI() const;
    std::string getVersion() const;
    std::string getHeader(const std::string& field) const;
    std::string getRaw() const;

private:
    std::string method;
    std::string uri;
    std::string version;
    std::map<std::string, std::string> headers;

    const std::string rawRequest;

    void parseRaw(const char* request);
    void parseRequestLine(const std::string& line);
    void parseHeaderLine(const std::string& line);
};