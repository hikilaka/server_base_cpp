#include "sysd/server.hpp"

#include <functional>
#include <iostream>
#include <utility>

sysd::server::server(boost::asio::io_service &service, std::uint16_t port)
    : acceptor(service,
               boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(),
                                              port)),
      socket(service),
      handler()
{  }

void sysd::server::start() {
    std::cout << "starting accept at " << acceptor.local_endpoint() << std::endl;
    async_accept();
}

void sysd::server::stop() {
    acceptor.cancel();
    acceptor.close();
}

void sysd::server::async_accept() {
    acceptor.async_accept(socket, [this](const boost::system::error_code &error) {
        if (error) {
            std::cout << "accept error: " << error << std::endl;        
        } else {
            connection *conn = new connection(handler, std::move(socket));
            handler.on_connect(*conn);
            conn->read();

            async_accept();
        }
    });
}

