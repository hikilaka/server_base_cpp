#include "sysd/connection.hpp"

#include <algorithm>
#include <iterator>
#include <utility>

sysd::connection::connection(connection_handler &handler,
                             boost::asio::ip::tcp::socket socket)
    : handler(handler), socket(std::move(socket)),
      read_buffer(), write_queue(), read_mutex(), write_mutex(),
      last_activity()
{  }

void sysd::connection::run_async() {
    async_read();
}

void sysd::connection::close() {
    socket.cancel();
    socket.close();
}

void sysd::connection::write(buffer buf) {
    write_mutex.lock();
    write_queue.emplace_back(buf);
    write_mutex.unlock();
    async_write();
}

bool sysd::connection::is_open() {
    return socket.is_open();
}

void sysd::connection::update_activity() {
    last_activity = clock_type::now();
}

sysd::connection::time_point_type sysd::connection::last_active() {
    return last_activity;
}

void sysd::connection::async_read() {
    socket.async_read_some(boost::asio::buffer(read_buffer),
                           [this](const boost::system::error_code &error,
                                  std::size_t bytes_transferred) {
        if (error) {
            if (boost::asio::error::eof == error ||
                    boost::asio::error::connection_reset == error) {
                handler.on_disconnect(this);
            } else {
                handler.on_error(this, error);
            }
        } else {
            read_mutex.lock();
            auto begin = std::begin(read_buffer);
            auto end = std::begin(read_buffer);
            std::advance(end, bytes_transferred);

            handler.on_data(this, buffer(begin, end));
            read_mutex.unlock();
            
            async_read();
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
            if (boost::asio::error::eof == error ||
                    boost::asio::error::connection_reset == error) {
                handler.on_disconnect(this);
            } else {
                handler.on_error(this, error);
            }
        } else {
            async_write();
        }
    });
}

