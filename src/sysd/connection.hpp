#include "connection.hpp"

#include <functional>
#include <utility>

sysd::connection(connection_handler &conn_handler,
                 boost::asio::tcp::ip::socket socket)
    : conn_handler(conn_handler), socket(std::move(socket)),
      read_buffer(), write_queue()
{  }

void sysd::connection::read() {
    async_read();
}

void sysd::connection::write(buffer buf) {
    // mutex.lock
    // emplace_back(buf)
    // mutex.unlock
    async_write();
}

void sysd::connection::async_read() {
    using namespace std::placeholders;

    socket.async_read_some(boost::asio::buffer(read_buffer),
                           std::bind(read_handler, *this, _1, _2));
}

void sysd::connection::async_write() {
    // mutex.lock
    if (write_queue.size() == 0) {
        // mutex.unlock
        return;
    }

    auto itr = write_queue.begin();
    sysd::buffer buf_copy(*itr);
    write_queue.erase(itr);
    // mutex.unlock
    
    using namespace std::placeholders;

    boost::asio::async_write(socket,
                             boost::asio::buffer(buf_copy.data()),
                             std::bind(write_handler, *this, _1, _2));
}

void read_handler(sysd::connection &conn,
                  boost::system::error_code &error,
                  std::size_t bytes_transferred) {
    if (error) {
        conn.handler().on_error(conn, error); // std::forward  ???
    } else {
        conn.handler().on_data(conn, buffer(read_buffer.begin(),
                                            read_buffer.end()));
    }
}

void write_handler(sysd::connection &conn,
                  boost::system::error_code &error,
                  std::size_t bytes_transferred) {
    if (error) {
        conn.handler().on_error(conn, error);
    } else {
        // async_write until write_queue is clear
        // async_write();
        // TODO: figure this out
    }
}

