#include "sysd/server.hpp"

#include <functional>
#include <utility>

sysd::server(boost::asio::io_service &service, std::uint16_t port)
    : acceptor(service,
               boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(),
                                              port)),
      socket(service),
      conn_handler()
{  }

void sysd::server::start() {
    async_accept();
}

void sysd::server::async_accept() {
    acceptor.async_accept(socket, [this](boost::system::error_code &error) {
        if (error) {
            std::cout << "accept error: " << error << std::endl;        
        } else {
            connection *conn = new connection(std::move(socket), conn_handler);
            conn_handler.on_connect(*conn);
            conn.read();

            async_accept();
        }
    });
}

