#include "sysd/connection_handler.hpp"

#include <boost/log/trivial.hpp>

void sysd::connection_handler::on_connect(connection &conn) {
    BOOST_LOG_TRIVIAL(info) << "on_connect(&conn)";
}

void sysd::connection_handler::on_disconnect(connection &conn) {
    BOOST_LOG_TRIVIAL(info) << "on_disconnect(&conn)";
    
    if (conn.is_open()) {
        conn.close();
    }

    delete &conn;
}

void sysd::connection_handler::on_data(connection &conn, buffer buf) {
    BOOST_LOG_TRIVIAL(info) << "on_data(&conn, " << buf << ")";

    conn.write(buf); // echo serv~
}

void sysd::connection_handler::on_error(connection &conn,
                                        boost::system::error_code error) {
    BOOST_LOG_TRIVIAL(info) << "on_error(&conn, " << error << ")";

    if (conn.is_open()) {
        conn.close();
    }
}

