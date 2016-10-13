#include <exception>
#include <iostream>

#include "sysd/server.hpp"
#include "sysd/connection_handler.hpp"

int main() {
    try {
        boost::asio::io_service service;
        sysd::server server(service, 34567);
    
        server.start();
        service.run();
    } catch (std::exception &e) {
        std::cout << "exception: " << e.what() << std::endl;
    }
    return 0;
}

