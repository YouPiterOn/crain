#include "HttpResponse.hpp"
#include <fstream>
#include <sstream>

HttpResponse::HttpResponse() : statusCode(200), reasonPhrase("OK"), version("HTTP/1.1") {}

void HttpResponse::setStatus(int statusCode, const std::string& reasonPhrase) {
    this->statusCode = statusCode;
    this->reasonPhrase = reasonPhrase;
}

void HttpResponse::setVersion(const std::string& version) {
    this->version = version;
}

void HttpResponse::setHeader(const std::string& field, const std::string& value) {
    headers[field] = value;
}

void HttpResponse::setBody(const std::string& body) {
    this->body = body;
    headers["Content-Length"] = std::to_string(body.size());
}

void HttpResponse::setBodyFromFile(const std::string& filePath) {
    std::ifstream file(filePath);
    if (file) {
        std::stringstream buffer;
        buffer << file.rdbuf();
        body = buffer.str();
        headers["Content-Length"] = std::to_string(body.size());
    } else {
        setStatus(404, "Not Found");
        setBody("File not found");
    }
}


std::string HttpResponse::toString() const {
    std::ostringstream responseStream;
    responseStream << version << " " << statusCode << " " << reasonPhrase << "\r\n";

    for (const auto& header : headers) {
        responseStream << header.first << ": " << header.second << "\r\n";
    }
    responseStream << "\r\n";

    responseStream << body;

    return responseStream.str();
}