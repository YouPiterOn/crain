#include "../include/utils.hpp"
#include <sstream>

std::vector<std::string> crain::splitPath(const std::string& path) {
    std::vector<std::string> tokens;
    std::string token;
    std::istringstream tokenStream(path);
    while (std::getline(tokenStream, token, '/')) {
        tokens.push_back(token);
    }
    return tokens;
}