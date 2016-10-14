#include "sysd/connection_handler.hpp"

#include <algorithm>

#include <boost/log/trivial.hpp>

sysd::connection_handler::connection_handler()
    : connections(), connections_mutex()
{  }

void sysd::connection_handler::on_connect(connection *conn) {
    BOOST_LOG_TRIVIAL(info) << "on_connect(*conn)";

    BOOST_LOG_TRIVIAL(info) << "new connection from " << conn->endpoint();
    conn->update_activity();

    connections_mutex.lock();
    connections.push_back(conn);
    connections_mutex.unlock();
}

void sysd::connection_handler::on_disconnect(connection *conn) {
    BOOST_LOG_TRIVIAL(info) << "on_disconnect(*conn)";
    
    if (conn->is_open()) {
        // is this necessary??
        // NEED TO TEST
        conn->close();
    }

    delete conn;
}

void sysd::connection_handler::on_data(connection *conn, buffer buf) {
    BOOST_LOG_TRIVIAL(info) << "on_data(*conn, " << buf << ")";

    conn->update_activity();
    conn->write(buf); // echo serv~
}

void sysd::connection_handler::on_error(connection *conn,
                                        boost::system::error_code error) {
    BOOST_LOG_TRIVIAL(info) << "on_error(*conn, " << error << ")";

    if (conn->is_open()) {
        conn->close();
    }
}

void sysd::connection_handler::check_timeouts() {
    using namespace std::chrono;

    connections_mutex.lock();

    auto now = clock_type::now();

    connections.erase(std::remove_if(connections.begin(),
                                     connections.end(),
                                     [&now](connection *conn) {
        auto last_activity = duration_cast<minutes>(now - conn->last_active());
        
        if (!conn->is_open()) {
            conn->close(); // clean up
            return true;
        }

        if (last_activity.count() >= 1) {
            sysd::buffer buf;
            buf << std::string("closed due to inactivity");

            conn->write(buf);
            conn->close();
            return true;       
        } else {
            return false;
        }                 
    }), connections.end());

    connections_mutex.unlock();
}

