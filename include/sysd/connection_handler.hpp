#ifndef SYSD_CONNECTION_HANDLER_HPP
#define SYSD_CONNECTION_HANDLER_HPP

#pragma once

#include <tuple>
#include <unordered_map>

#include <boost/system/error_code.hpp>

#include "sysd/buffer.hpp"
#include "sysd/connection.hpp"
#include "sysd/util.hpp"

namespace sysd {
    class connection;
    class connection_handler {
    public:
        using clock_type = std::chrono::steady_clock;
        using time_point_type = sysd::time_point<clock_type>;

        void on_connect(connection &conn);
        void on_disconnect(connection &conn);
        void on_data(connection &conn, buffer buf);
        void on_error(connection &conn, boost::system::error_code error);

        void check_connection_timeouts();
    private:
        std::vector<connection> connections;
    };
}

#endif // SYSD_CONNECTION_HANDLER_HPP
