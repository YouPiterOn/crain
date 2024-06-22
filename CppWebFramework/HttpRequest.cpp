#include "HttpRequest.hpp"
#include <sstream>
#include <iostream>

HttpRequest::HttpRequest(const std::string& request) {
    parseRaw(request);
}

void HttpRequest::parseRaw(const std::string& request) {
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
    parseUriParams();
}

void HttpRequest::parseRequestLine(const std::string& line) {
    std::istringstream lineStream(line);
    lineStream >> method >> uri >> version;
}

void HttpRequest::parseHeaderLine(const std::string& line) {
    size_t splitPos = line.find(':');
    if (splitPos != std::string::npos) {
        std::string field = line.substr(0, splitPos);
        std::string value = line.substr(splitPos + 1);

        size_t startPos = value.find_first_not_of(" ");
        if (startPos != std::string::npos) {
            value = value.substr(startPos);
        }

        headers[field] = value;
    }
}

void HttpRequest::parseUriParams() {
    size_t queryStart = uri.find('?');
    if (queryStart == std::string::npos) {
        return;
    }

    std::string query = uri.substr(queryStart + 1);
    uri = uri.substr(0, queryStart);
    std::stringstream queryStream(query);
    std::string pair;

    while (std::getline(queryStream, pair, '&')) {
        size_t splitPos = pair.find('=');
        if (splitPos != std::string::npos) {
            std::string key = pair.substr(0, splitPos);
            std::string value = pair.substr(splitPos + 1);
            params[key] = value;
        } else {
            params[pair] = "";
        }
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

std::string HttpRequest::getParam(const std::string& field) const {
    auto it = params.find(field);
    if (it == params.end()) {
        return "";
    }
    return it->second;
}

void HttpRequest::setMethod(const std::string& method) {
    this->method = method;
}
void HttpRequest::setURI(const std::string& uri) {
    this->uri = uri;
}
void HttpRequest::setVersion(const std::string& version) {
    this->version = version;
}
void HttpRequest::addHeader(const std::string& field, const std::string& value) {
    headers[field] = value;
}
void HttpRequest::addParam(const std::string& field, const std::string& value) {
    params[field] = value;
}