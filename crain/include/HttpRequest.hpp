#pragma once
#include <string>
#include <map>

namespace crain {
    class HttpRequest {
    public:
        HttpRequest(const std::string& request);
        
        std::string getMethod() const;
        std::string getURI() const;
        std::string getVersion() const;
        std::string getHeader(const std::string& field) const;
        std::string getParam(const std::string& field) const;

        void setMethod(const std::string& method);
        void setURI(const std::string& uri);
        void setVersion(const std::string& version);
        void addHeader(const std::string& field, const std::string& value);
        void addParam(const std::string& field, const std::string& value);

    private:
        std::string method;
        std::string uri;
        std::string version;
        std::map<std::string, std::string> headers;
        std::map<std::string, std::string> params;

        void parseRaw(const std::string& request);
        void parseRequestLine(const std::string& line);
        void parseHeaderLine(const std::string& line);
        void parseUriParams();
    };
}