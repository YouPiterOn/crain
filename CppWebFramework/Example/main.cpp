#include "Server.hpp"

int main() {
    Server server("0.0.0.0", 4000);
    server.run();
    return 0;
}