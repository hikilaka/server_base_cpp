#include "sysd/connection.hpp"

#include <utility>

sysd::connection::connection(connection_handler &handler,
                             boost::asio::ip::tcp::socket socket)
    : handler(handler), socket(std::move(socket)),
      read_buffer(), write_queue(), read_mutex(), write_mutex()
{  }

void sysd::connection::read() {
    async_read();
}

void sysd::connection::write(buffer buf) {
    write_mutex.lock();
    write_queue.emplace_back(buf);
    write_mutex.unlock();
    async_write();
}

void sysd::connection::async_read() {
    socket.async_read_some(boost::asio::buffer(read_buffer, 512), // TODO: get rid of magic number
                           [this](const boost::system::error_code &error,
                                  std::size_t bytes_transferred) {
        if (error) {
            handler.on_error(*this, error);    
        } else {
            if (bytes_transferred == 0) {
                handler.on_disconnect(*this);
            } else {
                read_mutex.lock();
                handler.on_data(*this, buffer(read_buffer.begin(),
                                              read_buffer.end()));
                read_buffer.clear();
                read_mutex.unlock();
            }
        }
    });
}

void sysd::connection::async_write() {
    write_mutex.lock();

    if (write_queue.size() == 0) {
        write_mutex.unlock();
        return;
    }

    auto itr = write_queue.begin();
    sysd::buffer buf_copy(*itr);
    write_queue.erase(itr);
    write_mutex.unlock();

    boost::asio::async_write(socket,
                             boost::asio::buffer(buf_copy.data()),
                             [this](const boost::system::error_code &error,
                                    [[maybe_unused]]
                                    std::size_t bytes_transferred) {
        if (error) {
            handler.on_error(*this, error);
        } else {
            // async_write until write_queue is clear
            async_write();
        }
    });
}

