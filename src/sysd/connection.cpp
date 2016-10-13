#include "sysd/connection.hpp"

#include <utility>

sysd::connection(connection_handler &handler,
                 boost::asio::tcp::ip::socket socket)
    : handler(handler), socket(std::move(socket)),
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
    socket.async_read_some(boost::asio::buffer(read_buffer),
                           [this](boost::system::error_code &error,
                                  std::size_t bytes_transferred) {
        if (error) {
            handler.on_error(*this, error);    
        } else {
            if (bytes_transferred == 0) {
                handler.on_disconnect(*this);
            } else {
                handler.on_data(*this, buffer(read_buffer.begin(),
                                              read_buffer.end()));
            }
        }
    });
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

    boost::asio::async_write(socket,
                             boost::asio::buffer(buf_copy.data()),
                             [this](boost::system::error_code &error,
                                    std::size_t bytes_transferred) {
        if (error) {
            handler.on_error(*this, error);
        } else {
            // async_write until write_queue is clear ??
            // async_write();
        }
    });
}
