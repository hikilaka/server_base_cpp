#include "sysd/net/connection_handler.hpp"

#include <algorithm>

#include <boost/log/trivial.hpp>

sysd::net::connection_handler::connection_handler()
    : connections(), connections_mutex()
{  }

void sysd::net::connection_handler::on_connect(connection *conn) {
    BOOST_LOG_TRIVIAL(debug) << "connection from " << conn->endpoint().address().to_string();
    conn->update_activity();

    connections_mutex.lock();
    connections.push_back(conn);
    connections_mutex.unlock();
}

void sysd::net::connection_handler::on_disconnect(connection *conn) {
    BOOST_LOG_TRIVIAL(debug) << "session disconnected";
    
    if (conn->is_open()) {
        conn->close();
    }
}

void sysd::net::connection_handler::on_data(connection *conn, buffer buf) {
    BOOST_LOG_TRIVIAL(debug) << "on_data(*conn, " << buf << ")";

    conn->update_activity();
    conn->write(buf); // echo serv~
}

void sysd::net::connection_handler::on_error(connection *conn,
                                             boost::system::error_code error) {
    BOOST_LOG_TRIVIAL(debug) << "on_error(*conn, " << error << ")";

    if (conn->is_open()) {
        conn->close();
    }
}

void sysd::net::connection_handler::check_timeouts() {
    using namespace std::chrono;

    connections_mutex.lock();

    auto now = clock_type::now();
    connections.erase(std::remove_if(connections.begin(),
                                     connections.end(),
                                     [&now](connection *conn) {
        auto last_activity = duration_cast<minutes>(now - conn->last_active());

        if (!conn->is_open()) {
            delete conn;
            return true;
        }

        if (last_activity.count() >= 1) {        
            buffer buf;
            buf << std::string("disconnecting due to inactivity\n");
            conn->write(buf);
            conn->close();
            delete conn;
            return true;       
        } else {
            return false;
        }                 
    }), connections.end());

    connections_mutex.unlock();
}

