#pragma once
#include "Server.hpp"
#include "Router.hpp"
#include <atomic>
#include <thread>

class App {
    public:
        App(std::string ip, int port);
        void run();
        
        Server server;
        Router router;
    private:
        void appLoop();
};