#include "sysd/server.hpp"

#include <functional>
#include <iostream>
#include <utility>

#include <boost/log/trivial.hpp>

sysd::server::server(boost::asio::io_service &service, std::uint16_t port)
    : acceptor(service,
               boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(),
                                              port)),
      socket(service),
      timeout_timer(service, boost::posix_time::seconds(10)),
      handler()
{  }

void sysd::server::start() {
    BOOST_LOG_TRIVIAL(info) << "starting accept at " << acceptor.local_endpoint().port();
    
    async_timer();
    async_accept();
}

void sysd::server::stop() {
    timeout_timer.cancel();
    // close all connections first?
    acceptor.cancel();
    acceptor.close();
}

void sysd::server::async_accept() {
    acceptor.async_accept(socket, [this](const boost::system::error_code &error) {
        if (error) {
            std::cout << "accept error: " << error << std::endl;        
        } else {
            connection *conn = new connection(handler, std::move(socket));
            handler.on_connect(conn);
            conn->run_async();

            async_accept();
        }
    });
}

void sysd::server::async_timer() {
    handler.check_timeouts();

    timeout_timer.expires_at(timeout_timer.expires_at() + boost::posix_time::seconds(10));
    timeout_timer.async_wait(std::bind(&server::async_timer, this));
}

