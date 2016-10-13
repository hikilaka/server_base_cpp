#ifndef SYSD_CONNECTION_HANDLER_HPP
#define SYSD_CONNECTION_HANDLER_HPP

#pragma once

#include <boost/system/error_code.hpp>

#include "sysd/buffer.hpp"
#include "sysd/connection.hpp"

namespace sysd {
    class connection;
    class connection_handler {
    public:
        void on_connect(connection &conn);
        void on_disconnect(connection &conn);
        void on_data(connection &conn, buffer buf);
        void on_error(connection &conn, boost::system::error_code error);
    };
}

#endif // SYSD_CONNECTION_HANDLER_HPP
