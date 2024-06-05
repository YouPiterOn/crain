#include "HttpRequest.hpp"
#include <sstream>
#include <iostream>

HttpRequest::HttpRequest(const char* request) : rawRequest(request) {
    parseRaw(request);
}

void HttpRequest::parseRaw(const char* request) {
    std::istringstream stream(request);
    std::string line;
    bool isFirstLine = true;

    while (std::getline(stream, line) && !line.empty()) {
        if (line.back() == '\r') {
            line.pop_back();
        }

        if (isFirstLine) {
            parseRequestLine(line);
            isFirstLine = false;
        } else {
            parseHeaderLine(line);
        }
    }
}

void HttpRequest::parseRequestLine(const std::string& line) {
    std::istringstream lineStream(line);
    lineStream >> method >> uri >> version;
}

void HttpRequest::parseHeaderLine(const std::string& line) {
    size_t colonPos = line.find(':');
    if (colonPos != std::string::npos) {
        std::string field = line.substr(0, colonPos);
        std::string value = line.substr(colonPos + 1);

        size_t startPos = value.find_first_not_of(" ");
        if (startPos != std::string::npos) {
            value = value.substr(startPos);
        }

        headers[field] = value;
    }
}

std::string HttpRequest::getMethod() const {
    return method;
}

std::string HttpRequest::getURI() const {
    return uri;
}

std::string HttpRequest::getVersion() const {
    return version;
}

std::string HttpRequest::getHeader(const std::string& field) const {
    auto it = headers.find(field);
    if (it == headers.end()) {
        return "";
    }
    return it->second;
}

std::string HttpRequest::getRaw() const {
    return rawRequest;
}