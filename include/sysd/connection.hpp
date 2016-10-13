#ifndef SYSD_CONNECTION_HPP
#define SYSD_CONNECTION_HPP

#pragma once

#include <array>
#include <cstdint>
#include <utility>
#include <vector>

#include <boost/asio.hpp>

#include "sysd/buffer.hpp"
#include "sysd/connection_handler.hpp"

namespace sysd {
    class connection_handler;
    
    class connection {
    public:
        connection(connection_handler &handler,
                   boost::asio::ip::tcp::socket socket);

        void read();
        void write(buffer buf);
    private:
        void async_read();
        void async_write();

        sysd::connection_handler &handler;
        boost::asio::ip::tcp::socket socket;
        std::array<std::uint8_t, 512> read_buffer;
        std::vector<buffer> write_queue;
    };
}

#endif // SYSD_CONNECTION_HPP
