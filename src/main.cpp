#include <exception>
#include <iostream>

#include <typeinfo>

#include <boost/program_options.hpp>
#include <boost/log/trivial.hpp>

#include "sysd/connection_handler.hpp"
#include "sysd/server.hpp"
#include "sysd/log/extension.hpp"

auto parse_arguments(int argc, char *argv[]) {
    namespace opts = boost::program_options;

    opts::options_description desc("allowed arguments");
    desc.add_options()
        ("help", "displays help and usage information")
        ("port", opts::value<std::uint16_t>(),
            "the port the server will run on - defaults to 34567");

    opts::variables_map args;
    opts::store(opts::parse_command_line(argc, argv, desc), args);
    opts::notify(args);
    return args;
}

int main(int argc, char *argv[]) {
    try {
        sysd::log::add_extensions();

        BOOST_LOG_TRIVIAL(info) << "test";

        /*std::uint16_t port = 34567;

        auto args = parse_arguments(argc, argv);

        if (args.count("help")) {
            std::cout << "you aren't getting help." << std::endl;
            return 0;
        }
        if (args.count("port")) {
            port = args["port"].as<std::uint16_t>();
        }

        boost::asio::io_service service;
        sysd::server server(service, port);
    
        server.start();
        service.run();*/
    } catch (std::exception &e) {
        std::cout << "exception: " << e.what() << std::endl;
    }
    return 0;
}

