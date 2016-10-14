#ifndef SYSD_SERVER_HPP
#define SYSD_SERVER_HPP

#pragma once

#include <cstdint>

#include <boost/asio.hpp>

#include "sysd/connection_handler.hpp"

namespace sysd {
    class server {
    public:
        server(boost::asio::io_service &service, std::uint16_t port);

        void start();
        void stop();
    private:
        void async_accept();

        boost::asio::ip::tcp::acceptor acceptor;
        boost::asio::ip::tcp::socket socket;
        connection_handler handler;
    };
}

#endif // SYSD_SERVER_HPP

