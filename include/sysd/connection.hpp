#ifndef SYSD_CONNECTION_HPP
#define SYSD_CONNECTION_HPP

#pragma once

#include <array>
#include <chrono>
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
        using clock_type = std::chrono::steady_clock;
        using time_point_type = std::chrono::time_point<clock_type>;

        connection(connection_handler &handler,
                   boost::asio::ip::tcp::socket socket);

        void run_async();
        void close();
        void write(buffer buf);
        boost::asio::ip::tcp::endpoint endpoint();
        bool is_open();
        void update_activity();
        time_point_type last_active();
    private:
        void async_read();
        void async_write();

        sysd::connection_handler &handler;
        boost::asio::ip::tcp::socket socket;
        std::array<std::uint8_t, 256> read_buffer;
        std::vector<buffer> write_queue;
        std::mutex read_mutex, write_mutex;
        time_point_type last_activity;
    };
}

#endif // SYSD_CONNECTION_HPP
