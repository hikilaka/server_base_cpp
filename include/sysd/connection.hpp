#ifndef SYSD_CONNECTION_HPP
#define SYSD_CONNECTION_HPP

#pragma once

#include <cstdint>
#include <mutex>
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
        std::vector<std::uint8_t> read_buffer;
        std::vector<buffer> write_queue;
        std::mutex read_mutex, write_mutex;
    };
}

#endif // SYSD_CONNECTION_HPP
