#pragma once
#include "Server.hpp"
#include "Router.hpp"
#include <atomic>
#include <thread>

namespace crain {
    class App {
        public:
            App(std::string ip, int port);
            void run();

            template<ConvertibleToHandler ...Handlers>
            void use(Handlers&& ...handlers) {
                use("ANY", "/*", RouteHandler(handlers)...);
            }
            template<ConvertibleToHandler ...Handlers>
            void use(std::string method, std::string route, Handlers&& ...handlers) {
                (router.addRoute(method, route, RouteHandler(handlers)), ...);
            }
            
            void setPathToStatic(const std::string& path);
        private:
            std::mutex router_m;
            Server server;
            Router router;
            std::string requestHandler(std::string request);
    };
}