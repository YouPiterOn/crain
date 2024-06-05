#include "HttpResponse.hpp"
#include <sstream>

HttpResponse::HttpResponse() : statusCode(200), reasonPhrase("OK"), version("HTTP/1.1") {}

void HttpResponse::setStatus(const std::string& version, int statusCode, const std::string& reasonPhrase) {
    this->version = version;
    this->statusCode = statusCode;
    this->reasonPhrase = reasonPhrase;
}

void HttpResponse::setHeader(const std::string& field, const std::string& value) {
    headers[field] = value;
}

void HttpResponse::setBody(const std::string& body) {
    this->body = body;
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