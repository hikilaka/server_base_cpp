#include "sysd/connection_handler.hpp"

#include <iostream>

void sysd::connection_handler::on_connect(connection &conn) {
    std::cout << "on_connect()" << std::endl;
}

void sysd::connection_handler::on_disconnect(connection &conn) {
    std::cout << "on_disconnect()" << std::endl;
    conn.stop();
    delete &conn;
}

void sysd::connection_handler::on_data(connection &conn, buffer buf) {
    std::cout << "on_data(" << buf << ")" << std::endl;
    conn.write(buf); // echo serv~
}

void sysd::connection_handler::on_error(connection &conn,
                                        boost::system::error_code error) {
    std::cout << "on_error(" << error << ")" << std::endl;
}

