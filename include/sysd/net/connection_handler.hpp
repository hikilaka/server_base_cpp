#ifndef SYSD_NET_CONNECTION_HANDLER_HPP
#define SYSD_NET_CONNECTION_HANDLER_HPP

#pragma once

#include <chrono>
#include <mutex>
#include <vector>

#include <boost/system/error_code.hpp>

#include "sysd/net/buffer.hpp"
#include "sysd/net/connection.hpp"

namespace sysd::net {
    class connection;
    class connection_handler {
    public:
        using clock_type = std::chrono::steady_clock;
        using time_point_type = std::chrono::time_point<clock_type>;

        connection_handler();

        void on_connect(connection *conn);
        void on_disconnect(connection *conn);
        void on_data(connection *conn, buffer buf);
        void on_error(connection *conn, boost::system::error_code error);

        void check_timeouts();
    private:
        std::vector<connection*> connections;
        std::mutex connections_mutex;
    };
}

#endif // SYSD_NET_CONNECTION_HANDLER_HPP

