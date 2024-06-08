#pragma once
#include "Server.hpp"
#include "HttpHandler.hpp"
#include "Router.hpp"

class App {
    public:
        Server server;
        Router router;
        HttpHandler HttpHandler;
        void Run();
    private:
        
};